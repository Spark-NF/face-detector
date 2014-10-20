#ifndef ADDPICTUREDIALOG_H
#define ADDPICTUREDIALOG_H

#include <QDialog>
#include "person.h"
#include "aspectratiopixmaplabel.h"
#include <opencv2/contrib/contrib.hpp>

namespace Ui {
    class AddPictureDialog;
}

class AddPictureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPictureDialog(QString file, QList<Person*> *persons, QWidget *parent = 0);
    ~AddPictureDialog();

protected:
    void emitPicture();

signals:
    void addPicture(Person *person, QString file);

private:
    Ui::AddPictureDialog *ui;
    AspectRatioPixmapLabel *imageLabel;
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyesCascade;
    cv::CascadeClassifier eyesGlassesCascade;
    QString _file;
    QList<Person*> *_persons;
    QRect _face;
};

#endif // ADDPICTUREDIALOG_H
