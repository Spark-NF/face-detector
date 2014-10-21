#include "person.h"
#include <QDir>
#include <QDebug>

Person::Person(int id, QString name)
    : _id(id), _name(name)
{
    QDir dir;
    if (dir.cd("data/original/" + QString::number(id)))
        _faces = dir.entryList(QDir::Files);

    _nextId = 1;
    for (QString face : _faces)
    {
        int id = face.left(face.length() - 4).toInt();
        if (id >= _nextId)
            _nextId = id + 1;
    }
}

int Person::id() const
{
    return _id;
}
QString Person::name() const
{
    return _name;
}
QStringList *Person::faces()
{
    return &_faces;
}
int Person::nextId() const
{
    return _nextId;
}

void Person::setName(QString name)
{
    _name = name;
}
void Person::setNextId(int id)
{
    _nextId = id;
}
