#include "addpersondialog.h"
#include "ui_addpersondialog.h"

AddPersonDialog::AddPersonDialog(QList<Person*> *persons, QWidget *parent)
    : QDialog(parent), ui(new Ui::AddPersonDialog)
{
    setModal(true);
    ui->setupUi(this);

    int maxID = 0;
    for (Person *person : *persons)
        if (person->id() > maxID)
            maxID = person->id();
    ui->lineID->setText(QString::number(maxID + 1));

    connect(this, &QDialog::accepted, this, &AddPersonDialog::emitPerson);
}

AddPersonDialog::~AddPersonDialog()
{
    delete ui;
}

void AddPersonDialog::emitPerson()
{
    emit addPerson(new Person(ui->lineID->text().toInt(), ui->lineName->text()));
}
