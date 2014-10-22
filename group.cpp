#include "group.h"

Group::Group(int id, QString name, QColor color)
    : _id(id), _name(name), _color(color)
{}

int Group::id() const
{
    return _id;
}
QString Group::name() const
{
    return _name;
}
QColor Group::color() const
{
    return _color;
}
QList<Person*> *Group::persons()
{
    return &_persons;
}

void Group::setName(QString name)
{
    _name = name;
}
void Group::setColor(QColor color)
{
    _color = color;
}
