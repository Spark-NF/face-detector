#ifndef WEBCAMDIALOG_H
#define WEBCAMDIALOG_H

#include <QDialog>
#include <QTimer>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include "person.h"

namespace Ui {
    class WebcamDialog;
}

class WebcamDialog : public QDialog
{
    Q_OBJECT

public:
    WebcamDialog(QList<Person*> *persons, QWidget *parent = 0);
    ~WebcamDialog();

public slots:
    void setInterval(int interval);
    void setWebcam(int webcam);
    void updatePicture();
    void screenshot();

private:
    Ui::WebcamDialog *ui;
    QTimer *timer;
    cv::VideoCapture captureDevice;
    cv::CascadeClassifier faceCascade;
    cv::Ptr<cv::FaceRecognizer> faceRecognizer;
    QList<Person*> *_persons;
    QSize imageSize;
};

#endif // WEBCAMDIALOG_H
