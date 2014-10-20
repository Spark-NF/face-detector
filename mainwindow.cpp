#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "webcamdialog.h"
#include "addpersondialog.h"
#include "addpicturedialog.h"
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile f("data/persons.csv");
    f.open(QFile::ReadOnly | QFile::Text);
    while (!f.atEnd())
    {
        QString line = f.readLine().trimmed();
        QStringList infos = line.split(';');
        if (infos.count() != 2)
            continue;

        persons.append(new Person(infos[0].toInt(), infos[1]));
    }
    f.close();

    for (Person *person : persons)
        tableAddPerson(person);

    ui->tablePersons->verticalHeader()->setDefaultSectionSize(24);
    ui->tablePersons->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tablePersons->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tablePersons->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    qDeleteAll(persons);
    delete ui;
}

void MainWindow::openWebcamDialog()
{
    WebcamDialog *webcamDialog = new WebcamDialog(&persons, this);
    webcamDialog->show();
}
void MainWindow::addPerson()
{
    AddPersonDialog *addPersonDialog = new AddPersonDialog(&persons, this);
    connect(addPersonDialog, &AddPersonDialog::addPerson, this, &MainWindow::addPersonDone);
    addPersonDialog->show();
}
void MainWindow::addPersonDone(Person *person)
{
    persons.append(person);
    tableAddPerson(person);
}
void MainWindow::addPicture()
{
    QString file = QFileDialog::getOpenFileName(this);
    AddPictureDialog *addPictureDialog = new AddPictureDialog(file, &persons, this);
    connect(addPictureDialog, &AddPictureDialog::addPicture, this, &MainWindow::addPictureDone);
    addPictureDialog->show();
}
void MainWindow::addPictureDone(Person *person, QString file)
{
    for (int i = 0; i < ui->tablePersons->rowCount(); ++i)
        if (ui->tablePersons->item(i, 0)->text().toInt() == person->id())
            ui->tablePersons->item(i, 2)->setText(QString::number(ui->tablePersons->item(i, 2)->text().toInt() + 1));
}

void MainWindow::tableAddPerson(Person *person)
{
    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(person->id()));
    itemID->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    ui->tablePersons->insertRow(ui->tablePersons->rowCount());
    ui->tablePersons->setItem(ui->tablePersons->rowCount() - 1, 0, itemID);
    ui->tablePersons->setItem(ui->tablePersons->rowCount() - 1, 1, new QTableWidgetItem(person->name()));
    ui->tablePersons->setItem(ui->tablePersons->rowCount() - 1, 2, new QTableWidgetItem(QString::number(person->faces()->size())));
}

void MainWindow::closeEvent(QCloseEvent*)
{
    QFile f("data/persons.csv");
    f.open(QFile::WriteOnly | QFile::Text);
    for (Person *person : persons)
        f.write((QString::number(person->id()) + ";" + person->name() + "\n").toUtf8());
    f.close();
}
