#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "webcamdialog.h"
#include "imagedialog.h"
#include "addpersondialog.h"
#include "persondialog.h"
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

    editMapper = new QSignalMapper(this);
    connect(editMapper, SIGNAL(mapped(int)), this, SLOT(editPerson(int)));

    for (Person *person : persons)
        tableAddPerson(person);

    ui->tablePersons->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tablePersons->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tablePersons->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tablePersons->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    qDeleteAll(persons);
    delete ui;
}

void MainWindow::openImageDialog()
{
    QString file = QFileDialog::getOpenFileName(this);
    ImageDialog *imageDialog = new ImageDialog(file, &persons, this);
    imageDialog->show();
}
void MainWindow::openWebcamDialog()
{
    WebcamDialog *webcamDialog = new WebcamDialog(&persons, this);
    webcamDialog->show();
}
void MainWindow::editPerson(int id)
{
    Person *person = nullptr;
    for (Person *p : persons)
        if (p->id() == id)
            person = p;
    if (person == nullptr)
        return;

    PersonDialog *personDialog = new PersonDialog(person, &persons, this);
    connect(personDialog, &PersonDialog::pictureCountChanged, this, &MainWindow::pictureCountChanged);
    personDialog->show();
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
void MainWindow::pictureCountChanged(Person *person)
{
    for (int i = 0; i < ui->tablePersons->rowCount(); ++i)
        if (ui->tablePersons->item(i, 0)->text().toInt() == person->id())
            ui->tablePersons->item(i, 2)->setText(QString::number(person->faces()->size()));
}

void MainWindow::tableAddPerson(Person *person)
{
    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(person->id()));
    itemID->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QPushButton *buttonEdit = new QPushButton("Edit");
    connect(buttonEdit, SIGNAL(clicked()), editMapper, SLOT(map()));
    editMapper->setMapping(buttonEdit, person->id());
    editButtons.append(buttonEdit);

    int i = ui->tablePersons->rowCount();
    ui->tablePersons->insertRow(i);
    ui->tablePersons->setItem(i, 0, itemID);
    ui->tablePersons->setItem(i, 1, new QTableWidgetItem(person->name()));
    ui->tablePersons->setItem(i, 2, new QTableWidgetItem(QString::number(person->faces()->size())));
    ui->tablePersons->setCellWidget(i, 3, buttonEdit);
    ui->tablePersons->item(i, 0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tablePersons->item(i, 1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tablePersons->item(i, 2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void MainWindow::closeEvent(QCloseEvent*)
{
    QFile f("data/persons.csv");
    f.open(QFile::WriteOnly | QFile::Text);
    for (Person *person : persons)
        f.write((QString::number(person->id()) + ";" + person->name() + "\n").toUtf8());
    f.close();
}
