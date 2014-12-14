#ifndef XMLHANDLER_H
#define XMLHANDLER_H
#include <QString>
#include <QTreeWidgetItem>
#include <QList>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "database.h"

class XmlHandler
{
private:
    const QString FILENAME;

    const QString ROOTELEMENT;
    const QString DBELEMENT;

public:
    //const QString IDATTR;
    static const QString INTERNALDBNAMEATTR;
    static const QString DBNAMEATTR;
    static const QString HOSTNAMEATTR;
    static const QString USERNAMEATTR;
    static const QString COMMENTATTR;

    XmlHandler();

    bool createXmlFile();

    void saveAllDbs(QList<Database> &dbList);
    QList<Database> getAllDbs();
    QList<QString> getAllDbNames();
    QString loadQStringAttribute(const QString InternDbName, const QString Attr);

};

#endif // XMLHANDLER_H
