#ifndef PERSONDIALOG_H
#define PERSONDIALOG_H

#include <QDialog>
#include <QSignalMapper>
#include "person.h"

namespace Ui {
    class PersonDialog;
}

class PersonDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit PersonDialog(Person *person, QList<Person*> *persons, QWidget *parent = 0);
        ~PersonDialog();
        void closeEvent(QCloseEvent *e);

    public slots:
        void addPicture();
        void addPictureDone(Person *person, QString file);
        void tableAddPicture(QString file);
        void deleteImage(QString file);

    signals:
        void addedPicture(Person *person, QString file);

    private:
        Ui::PersonDialog *ui;
        Person *_person;
        QList<Person*> *_persons;
        QList<QPushButton*> deleteButtons;
        QSignalMapper *deleteMapper;
};

#endif // PERSONDIALOG_H
