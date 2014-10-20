#include "addpicturedialog.h"
#include "ui_addpicturedialog.h"
#include "cvqt.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <QMessageBox>
#include <QDir>

AddPictureDialog::AddPictureDialog(QString file, QList<Person*> *persons, QWidget *parent)
    : QDialog(parent), ui(new Ui::AddPictureDialog), _file(file), _persons(persons)
{
    setModal(true);
    ui->setupUi(this);

    faceCascade.load("data/haarcascade_frontalface_alt.xml");
    eyesCascade.load("data/haarcascade_eye.xml");
    eyesGlassesCascade.load("data/haarcascade_eye_tree_eyeglasses.xml");

    for (int i = 0; i < persons->size(); ++i)
        ui->comboPerson->addItem(persons->at(i)->name(), i);

    imageLabel = new AspectRatioPixmapLabel(this);
    ui->formLayout->setWidget(1, QFormLayout::FieldRole, imageLabel);

    cv::Mat image = cv::imread(file.toStdString(), CV_LOAD_IMAGE_COLOR);
    if (!image.empty())
    {
        std::vector<cv::Rect> faces;
        cv::Mat frame_gray;
        cv::cvtColor(image, frame_gray, CV_BGR2GRAY);
        cv::equalizeHist(frame_gray, frame_gray);
        faceCascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
        if (faces.size() == 1)
        {
            _face = QRect(faces[0].x, faces[0].y, faces[0].width, faces[0].height);
            cv::rectangle(image, cv::Point(_face.x(), _face.y()), cv::Point(_face.x() + _face.width(), _face.y() + _face.height()), cvScalar(0, 255, 0, 0), 2, 8, 0);

            cv::Point2f eyeLeft, eyeRight;
            cv::Mat faceROI = frame_gray(faces[0]);
            std::vector<cv::Rect> eyes;
            eyesGlassesCascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
            if (eyes.size() != 2)
            {
                eyes.clear();
                eyesCascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
            }

            eyeLeft = cv::Point2f(float(faces[0].x + eyes[0].x + eyes[0].width * 0.5), float(faces[0].y + eyes[0].y + eyes[0].height * 0.5));
            eyeRight = cv::Point2f(float(faces[0].x + eyes[1].x + eyes[1].width * 0.5), float(faces[0].y + eyes[1].y + eyes[1].height * 0.5));

            int r1 = cvRound((eyes[0].width + eyes[0].height) * 0.15);
            int r2 = cvRound((eyes[1].width + eyes[1].height) * 0.15);
            imageLabel->setPixmap(cvMatToQPixmap(image));
            imageLabel->setEyes(QPoint(eyeLeft.x, eyeLeft.y), r1, QPoint(eyeRight.x, eyeRight.y), r2);

            if (eyeLeft.x > eyeRight.x)
            {
                cv::Point2f tmp = eyeRight;
                eyeRight = eyeLeft;
                eyeLeft = tmp;
            }
        }
    }

    connect(this, &QDialog::accepted, this, &AddPictureDialog::emitPicture);
}

AddPictureDialog::~AddPictureDialog()
{
    delete ui;
}

void AddPictureDialog::emitPicture()
{
    Person *person = _persons->at(ui->comboPerson->currentData().toInt());
    int id = person->nextId();
    person->setNextId(id + 1);

    QDir().mkpath("data/original/" + QString::number(person->id()));
    QString oPath = "data/original/" + QString::number(person->id()) + "/" + QString::number(id) + ".jpg";
    QFile::copy(_file, oPath);

    cv::Mat image = cv::imread(_file.toStdString(), CV_LOAD_IMAGE_COLOR);
    QPoint left = imageLabel->leftEye();
    QPoint right = imageLabel->rightEye();
    image = CropFace(image, cv::Point2f(left.x(), left.y()), cv::Point2f(right.x(), right.y()), cv::Point2f(0.25f, 0.25f), cv::Point(100, 100));

    if (image.cols > 0 && image.rows > 0)
    {
        QDir().mkpath("data/modified/" + QString::number(person->id()));
        imwrite(QString("data/modified/" + QString::number(person->id()) + "/" + QString::number(id) + ".jpg").toStdString(), image);

        QFile f("data/pictures.csv");
        f.open(QFile::WriteOnly | QFile::Text | QFile::Append);
        f.write((QString::number(person->id()) + ";" +
                 QString::number(id) + ";" +
                 (imageLabel->changed() ? "manual" : "checked") + ";" +
                 QString::number(left.x()) + ";" +
                 QString::number(left.y()) + ";" +
                 QString::number(right.x()) + ";" +
                 QString::number(right.y()) + "\n").toUtf8());
        f.close();

        emit addPicture(person, _file);
    }
    else
        QMessageBox::critical(0, "Error", "Error treating the image.");
}
