#ifndef ADDGROUPDIALOG_H
#define ADDGROUPDIALOG_H

#include <QDialog>
#include "person.h"
#include "group.h"

namespace Ui {
    class AddGroupDialog;
}

class AddGroupDialog : public QDialog
{
    Q_OBJECT

	public:
        explicit AddGroupDialog(QList<Group*> *groups, QWidget *parent = 0);
		~AddGroupDialog();

    public slots:
        void openColorWindow();
        void updateColorLine();

	protected:
        void emitGroup();

	signals:
        void addGroup(Group *group);

	private:
        Ui::AddGroupDialog *ui;
};

#endif // ADDGROUPDIALOG_H
