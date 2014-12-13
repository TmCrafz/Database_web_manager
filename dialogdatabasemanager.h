#ifndef DIALOGDATABASEMANAGER_H
#define DIALOGDATABASEMANAGER_H

#include <QDialog>
#include <QTreeWidget>
#include "xmlhandler.h"

namespace Ui {
class DialogDatabaseManager;
}

class DialogDatabaseManager : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDatabaseManager(QWidget *parent = 0);
    ~DialogDatabaseManager();

    void teeWidgetStartSelection();
    void addChildToTree(QTreeWidgetItem *parent, QString name);


private slots:
    void on_pushBtnNewDb_clicked();

    void on_treeWidgetDatabases_itemSelectionChanged();

private:
    Ui::DialogDatabaseManager *ui;
    XmlHandler xmlHandler;
};

#endif // DIALOGDATABASEMANAGER_H
