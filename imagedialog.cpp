#include "imagedialog.h"
#include "ui_imagedialog.h"
#include "cvqt.h"
#include <QFileDialog>
#include <QDebug>
#include <opencv2/objdetect/objdetect.hpp>

ImageDialog::ImageDialog(QString file, QList<Person*> *persons, QWidget *parent)
    : QDialog(parent), ui(new Ui::ImageDialog), _persons(persons)
{
    ui->setupUi(this);

    std::vector<cv::Mat> images;
    std::vector<int> labels;
    for (int i = 0; i < persons->size(); ++i)
    {
        for (QString file : *persons->at(i)->faces())
        {
            images.push_back(cv::imread(("data/modified/" + QString::number(persons->at(i)->id()) + "/" + file).toStdString(), 0));
            labels.push_back(i);
        }
    }
    if (!images.empty())
    {
        imageSize = QSize(images[0].cols, images[0].rows);
        faceRecognizer = cv::createFisherFaceRecognizer();
        faceRecognizer->train(images, labels);
    }

	imageLabel = new AspectRatioPixmapLabel(this, false);
	ui->verticalLayout->insertWidget(0, imageLabel, 2);

    faceCascade.load("data/haarcascade_frontalface_alt.xml");
    updatePicture(file);
}

ImageDialog::~ImageDialog()
{
    delete ui;
}

void ImageDialog::save()
{
    QString file = QFileDialog::getSaveFileName(this);
	imageLabel->pixmap()->save(file);
}
void ImageDialog::load()
{
    QString file = QFileDialog::getOpenFileName(this);
    updatePicture(file);
}

void ImageDialog::updatePicture(QString file)
{
    cv::Mat captureFrame, grayscaleFrame;
    captureFrame = cv::imread(file.toStdString(), CV_LOAD_IMAGE_COLOR);

    if (!captureFrame.empty())
    {
        cv::cvtColor(captureFrame, grayscaleFrame, CV_BGR2GRAY);
        cv::equalizeHist(grayscaleFrame, grayscaleFrame);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(grayscaleFrame, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
        for (size_t i = 0; i < faces.size(); i++)
        {
            cv::Rect face_i = faces[i];
            cv::Mat face = grayscaleFrame(face_i);
            cv::Mat face_resized;
            cv::resize(face, face_resized, cv::Size(imageSize.width(), imageSize.height()), 1.0, 1.0, cv::INTER_CUBIC);
            int prediction = faceRecognizer->predict(face_resized);

            auto col = CV_RGB(0, 255, 0);
            if (_persons->at(prediction)->group() != nullptr)
            {
                col = CV_RGB(_persons->at(prediction)->group()->color().red(), _persons->at(prediction)->group()->color().green(), _persons->at(prediction)->group()->color().blue());
                cv::putText(captureFrame,
                            _persons->at(prediction)->group()->name().toStdString(),
                            cv::Point(qMax(face_i.tl().x, 0), qMax(face_i.br().y + 15, 0)),
                            cv::FONT_HERSHEY_PLAIN,
                            1.0,
                            col,
                            1.0);
            }

            cv::putText(captureFrame,
                        _persons->at(prediction)->name().toStdString(),
                        cv::Point(qMax(face_i.tl().x, 0), qMax(face_i.tl().y - 10, 0)),
                        cv::FONT_HERSHEY_PLAIN,
                        1.0,
                        col,
                        1.0);
            cv::rectangle(captureFrame, face_i, col, 1, 8, 0);
        }

		imageLabel->setPixmap(cvMatToQPixmap(captureFrame));
    }
}
