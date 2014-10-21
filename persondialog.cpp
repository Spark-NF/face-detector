#include "persondialog.h"
#include "ui_persondialog.h"
#include "addpicturedialog.h"
#include <QFileDialog>
#include <QDebug>

PersonDialog::PersonDialog(Person *person, QList<Person*> *persons, QWidget *parent)
    : QDialog(parent), ui(new Ui::PersonDialog), _person(person), _persons(persons)
{
    ui->setupUi(this);

    ui->lineId->setText(QString::number(person->id()));
    ui->lineName->setText(person->name());

    ui->tablePictures->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tablePictures->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tablePictures->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tablePictures->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tablePictures->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    deleteMapper = new QSignalMapper(this);
    connect(deleteMapper, SIGNAL(mapped(QString)), this, SLOT(deleteImage(QString)));

    for (QString file : *person->faces())
        tableAddPicture(file);
}

PersonDialog::~PersonDialog()
{
    delete ui;
}

void PersonDialog::tableAddPicture(QString file)
{
    int id = file.left(file.length() - 4).toInt();

    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(id));
    itemID->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QLabel *imageOriginal = new QLabel(this);
    imageOriginal->setPixmap(QPixmap("data/original/" + QString::number(_person->id()) + "/" + file).scaled(QSize(24, 24), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QLabel *imageModifed = new QLabel(this);
    imageModifed->setPixmap(QPixmap("data/modified/" + QString::number(_person->id()) + "/" + file).scaled(QSize(24, 24), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPushButton *buttonDelete = new QPushButton("Delete");
    connect(buttonDelete, SIGNAL(clicked()), deleteMapper, SLOT(map()));
    deleteMapper->setMapping(buttonDelete, file);
    deleteButtons.append(buttonDelete);

    int i = ui->tablePictures->rowCount();
    ui->tablePictures->insertRow(i);
    ui->tablePictures->setItem(i, 0, itemID);
    ui->tablePictures->setItem(i, 1, new QTableWidgetItem(file));
    ui->tablePictures->setCellWidget(i, 2, imageOriginal);
    ui->tablePictures->setCellWidget(i, 3, imageModifed);
    ui->tablePictures->setCellWidget(i, 4, buttonDelete);
    ui->tablePictures->item(i, 0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tablePictures->item(i, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void PersonDialog::deleteImage(QString file)
{
    QFile::remove("data/original/" + QString::number(_person->id()) + "/" + file);
    QFile::remove("data/modified/" + QString::number(_person->id()) + "/" + file);
    _person->faces()->removeAll(file);

    for (int i = 0; i < ui->tablePictures->rowCount(); ++i)
    {
        if (ui->tablePictures->item(i, 1)->text() == file)
        {
            ui->tablePictures->removeRow(i);
            break;
        }
    }

    emit pictureCountChanged(_person);
}

void PersonDialog::addPicture()
{
    //QString file = QFileDialog::getOpenFileName(this);
    QStringList files = QFileDialog::getOpenFileNames(this);
    for (QString file : files)
    {
        AddPictureDialog *addPictureDialog = new AddPictureDialog(file, _person, _persons, this);
        connect(addPictureDialog, &AddPictureDialog::addPicture, this, &PersonDialog::addPictureDone);
        addPictureDialog->show();
    }
}
void PersonDialog::addPictureDone(Person *person, QString file)
{
    _person->faces()->append(file);
    tableAddPicture(file);
    emit pictureCountChanged(person);
}

void PersonDialog::closeEvent(QCloseEvent *e)
{
    _person->setName(ui->lineName->text());
    QDialog::closeEvent(e);
}
