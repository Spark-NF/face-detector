#ifndef GROUP_H
#define GROUP_H

#include <QString>
#include <QColor>
#include "person.h"

class Person;

class Group
{
    public:
        Group(int id, QString name, QColor color);
        int id() const;
        QString name() const;
        QColor color() const;
        QList<Person*> *persons();
        void setName(QString name);
        void setColor(QColor color);

    private:
        int _id;
        QString _name;
        QColor _color;
        QList<Person*> _persons;
};

#endif // GROUP_H
