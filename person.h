#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QStringList>
#include "group.h"

class Group;

class Person
{
    public:
        Person(int id, QString name, Group *group = nullptr);
        int id() const;
        QString name() const;
        int nextId() const;
        QStringList *faces();
        Group *group();
        void setName(QString name);
        void setNextId(int id);
        void setGroup(Group *group);

    private:
        int _id;
        QString _name;
        QStringList _faces;
        int _nextId;
        Group *_group;
};

#endif // PERSON_H
