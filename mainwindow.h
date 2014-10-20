#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void addPerson();
    void addPersonDone(Person *person);
    void addPicture();
    void addPictureDone(Person *person, QString file);

private:
    Ui::MainWindow *ui;
    QList<Person*> persons;
};

#endif // MAINWINDOW_H
