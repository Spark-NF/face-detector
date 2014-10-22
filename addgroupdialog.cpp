#include "addgroupdialog.h"
#include "ui_addgroupdialog.h"
#include <QColorDialog>

AddGroupDialog::AddGroupDialog(QList<Group *> *groups, QWidget *parent)
    : QDialog(parent), ui(new Ui::AddGroupDialog)
{
    setModal(true);
    ui->setupUi(this);

    int maxID = 0;
    for (Group *group : *groups)
        if (group->id() > maxID)
            maxID = group->id();
    ui->lineID->setText(QString::number(maxID + 1));

    connect(this, &QDialog::accepted, this, &AddGroupDialog::emitGroup);
}

AddGroupDialog::~AddGroupDialog()
{
    delete ui;
}

void AddGroupDialog::emitGroup()
{
    emit addGroup(new Group(ui->lineID->text().toInt(), ui->lineName->text(), QColor(ui->lineColor->text())));
}

void AddGroupDialog::openColorWindow()
{
    ui->lineColor->setText(QColorDialog::getColor().name());
}
void AddGroupDialog::updateColorLine()
{
    QPalette palette = ui->lineColor->palette();
    palette.setColor(QPalette::Text, QColor(ui->lineColor->text()));
    ui->lineColor->setPalette(palette);

}
