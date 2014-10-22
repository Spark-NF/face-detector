#ifndef PERSONDIALOG_H
#define PERSONDIALOG_H

#include <QDialog>
#include <QSignalMapper>
#include "person.h"
#include "group.h"

namespace Ui {
    class PersonDialog;
}

class PersonDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit PersonDialog(Person *person, QList<Group*> *groups, QWidget *parent = 0);
        ~PersonDialog();

    public slots:
        void addPicture();
        void addPictureDone(Person *person, QString file);
        void tableAddPicture(QString file);
        void deleteImage(QString file);
        void accept();

    signals:
        void pictureCountChanged(Person *person);

    private:
        Ui::PersonDialog *ui;
        Person *_person;
        QList<Group*> *_groups;
        QList<QPushButton*> deleteButtons;
        QSignalMapper *deleteMapper;
};

#endif // PERSONDIALOG_H
