#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QStringList>

class Person
{
    public:
        Person(int id, QString name);
        int id() const;
        QString name() const;
        int nextId() const;
        QStringList *faces();
        void setName(QString name);
        void setNextId(int id);

    private:
        int _id;
        QString _name;
        QStringList _faces;
        int _nextId;
};

#endif // PERSON_H
