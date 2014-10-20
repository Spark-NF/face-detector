#include "mainwindow.h"
#include <QApplication>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    cv::CascadeClassifier face_cascade;
    face_cascade.load("data/haarcascade_frontalface_alt.xml");

    return a.exec();
}
