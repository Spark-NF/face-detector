#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QPushButton>
#include "person.h"
#include "group.h"

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
        void tableAddGroup(Group *group);

    public slots:
        void openWebcamDialog();
        void openImageDialog();
        void addPerson();
        void addPersonDone(Person *person);
        void pictureCountChanged(Person *person);
        void editPerson(int id);
        void editGroup(int id);

    private:
        Ui::MainWindow *ui;
        QList<Person*> persons;
        QList<QPushButton*> editButtons, editGroupButtons;
        QSignalMapper *editMapper, *editGroupMapper;
        QList<Group*> groups;
};

#endif // MAINWINDOW_H
