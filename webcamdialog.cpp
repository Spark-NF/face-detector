#include "webcamdialog.h"
#include "ui_webcamdialog.h"
#include "cvqt.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/objdetect/objdetect.hpp>

WebcamDialog::WebcamDialog(QList<Person*> *persons, QWidget *parent)
    : QDialog(parent), ui(new Ui::WebcamDialog), _persons(persons), _recording(false), _started(false)
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

    cv::VideoCapture tmp;
    for (int i = 0; i < 10; ++i)
    {
        tmp.open(i);
        bool res = tmp.isOpened();
        tmp.release();
        if (res)
            ui->comboWebcams->addItem(QString::number(i));
    }

    faceCascade.load("data/haarcascade_frontalface_alt.xml");
    captureDevice.open(0);
    setStarted(false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WebcamDialog::updatePicture);
    timer->start(33);
}

WebcamDialog::~WebcamDialog()
{
    delete ui;
}

void WebcamDialog::setStarted(bool started)
{
    _started = started;
    ui->buttonRecord->setEnabled(_started);
    ui->buttonScreenshot->setEnabled(_started);
}

void WebcamDialog::setInterval(int interval)
{
    timer->setInterval(interval);
}
void WebcamDialog::setWebcam(int webcam)
{
    captureDevice.release();
    captureDevice.open(webcam);
    setStarted(false);
}

void WebcamDialog::screenshot()
{
    if (!_started)
        return;
    QPixmap screenshot = ui->webcamLabel->pixmap()->copy();
    QString file = QFileDialog::getSaveFileName(this);
    screenshot.save(file);
}
void WebcamDialog::toggleRecord()
{
    if (!_started)
        return;
    if (_recording)
    {
        _recording = false;
        _record.release();
        QString file = QFileDialog::getSaveFileName(this);
        QFile::rename("out.avi", file);
        ui->buttonRecord->setText("Record");
        ui->comboWebcams->setEnabled(true);
        ui->spinInterval->setEnabled(true);
    }
    else
    {
        int frame_width = captureDevice.get(CV_CAP_PROP_FRAME_WIDTH);
        int frame_height = captureDevice.get(CV_CAP_PROP_FRAME_HEIGHT);
        _record = cv::VideoWriter("out.avi", CV_FOURCC('i','Y','U','V'), 20, cv::Size(frame_width, frame_height), true);
        if (!_record.isOpened())
            _record = cv::VideoWriter("out.avi", -1, 20, cv::Size(frame_width, frame_height), true);
        if (_record.isOpened())
        {
            _recording = true;
            ui->buttonRecord->setText("Stop");
            ui->comboWebcams->setEnabled(false);
            ui->spinInterval->setEnabled(false);
        }
        else
            QMessageBox::critical(this, "Error", "Could not open destination file with selected codec.");
    }
}

void WebcamDialog::updatePicture()
{
    cv::Mat captureFrame, grayscaleFrame;
    captureDevice >> captureFrame;

    if (!captureFrame.empty())
    {
        if (!_started)
            setStarted(true);

        cv::cvtColor(captureFrame, grayscaleFrame, CV_BGR2GRAY);
        cv::equalizeHist(grayscaleFrame, grayscaleFrame);

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(grayscaleFrame, faces, 1.1, 3, CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
        for (size_t i = 0; i < faces.size(); i++)
        {
            cv::Rect face_i = faces[i];
            if (!imageSize.isNull())
            {
                cv::Mat face = grayscaleFrame(face_i);
                cv::Mat face_resized;
                cv::resize(face, face_resized, cv::Size(imageSize.width(), imageSize.height()), 1.0, 1.0, cv::INTER_CUBIC);
                int prediction = faceRecognizer->predict(face_resized);

                cv::putText(captureFrame,
                            _persons->at(prediction)->name().toStdString(),
                            cv::Point(qMax(face_i.tl().x, 0), qMax(face_i.tl().y - 10, 0)),
                            cv::FONT_HERSHEY_PLAIN,
                            1.0,
                            cvScalar(0, 255, 0, 0),
                            1.0);
            }
            cv::rectangle(captureFrame, face_i, cvScalar(0, 255, 0, 0), 1, 8, 0);
        }

        if (_recording)
            _record.write(captureFrame);

        ui->webcamLabel->setPixmap(cvMatToQPixmap(captureFrame));
    }
}
