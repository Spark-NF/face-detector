#include "person.h"
#include <QDir>
#include <QDebug>

Person::Person(int id, QString name, Group *group)
    : _id(id), _name(name), _group(group)
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

    if (_group != nullptr)
        _group->persons()->append(this);
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
Group *Person::group()
{
    return _group;
}

void Person::setName(QString name)
{
    _name = name;
}
void Person::setNextId(int id)
{
    _nextId = id;
}
void Person::setGroup(Group *group)
{
    if (_group != nullptr)
        _group->persons()->removeAll(this);

    _group = group;
    if (_group != nullptr)
        _group->persons()->append(this);
}
