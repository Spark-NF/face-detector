#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QPushButton>
#include "person.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void closeEvent(QCloseEvent *event);

    protected:
        void tableAddPerson(Person *person);

    public slots:
        void openWebcamDialog();
        void openImageDialog();
        void addPerson();
        void addPersonDone(Person *person);
        void addPictureDone(Person *person, QString file);
        void editPerson(int id);

    private:
        Ui::MainWindow *ui;
        QList<Person*> persons;
        QList<QPushButton*> editButtons;
        QSignalMapper *editMapper;
};

#endif // MAINWINDOW_H
