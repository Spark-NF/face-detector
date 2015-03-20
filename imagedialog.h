#ifndef ImageDialog_H
#define ImageDialog_H

#include <QDialog>
#include <QTimer>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include "aspectratiopixmaplabel.h"
#include "person.h"

namespace Ui {
    class ImageDialog;
}

class ImageDialog : public QDialog
{
    Q_OBJECT

    public:
        ImageDialog(QString file, QList<Person*> *persons, QWidget *parent = 0);
        ~ImageDialog();

    public slots:
        void save();
        void load();
        void updatePicture(QString file);

    private:
        Ui::ImageDialog *ui;
		AspectRatioPixmapLabel *imageLabel;
        cv::CascadeClassifier faceCascade;
        cv::Ptr<cv::FaceRecognizer> faceRecognizer;
        QList<Person*> *_persons;
        QSize imageSize;
};

#endif // ImageDialog_H
