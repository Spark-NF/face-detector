#include "addpersondialog.h"
#include "ui_addpersondialog.h"

AddPersonDialog::AddPersonDialog(QList<Person*> *persons, QList<Group *> *groups, QWidget *parent)
    : QDialog(parent), ui(new Ui::AddPersonDialog), _groups(groups)
{
    setModal(true);
    ui->setupUi(this);

    for (Group *group : *groups)
        ui->comboGroup->addItem(group->name(), group->id());

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
    Group *group = nullptr;
    for (Group *gr : *_groups)
        if (gr->id() == ui->comboGroup->currentData())
            group = gr;

    emit addPerson(new Person(ui->lineID->text().toInt(), ui->lineName->text(), group));
}
