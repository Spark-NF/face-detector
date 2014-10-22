#ifndef ADDPERSONDIALOG_H
#define ADDPERSONDIALOG_H

#include <QDialog>
#include "person.h"
#include "group.h"

namespace Ui {
    class AddPersonDialog;
}

class AddPersonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPersonDialog(QList<Person*> *persons, QList<Group*> *groups, QWidget *parent = 0);
    ~AddPersonDialog();

protected:
    void emitPerson();

signals:
    void addPerson(Person *person);

private:
    Ui::AddPersonDialog *ui;
    QList<Group*> *_groups;
};

#endif // ADDPERSONDIALOG_H
