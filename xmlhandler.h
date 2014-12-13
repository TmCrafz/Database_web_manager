#ifndef XMLHANDLER_H
#define XMLHANDLER_H
#include <QString>
#include <QTreeWidgetItem>
#include <QList>
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class XmlHandler
{
private:
    const QString FILENAME;

    const QString ROOTELEMENT;
    const QString DBELEMENT;

    const QString IDATTR;
    const QString DBNAMEATTR;
    const QString HOSTNAMEATTR;
    const QString USERNAMEATTR;
    const QString COMMENTATTR;

public:
    XmlHandler();

    bool createXmlFile();
    bool saveDbChildIntoXml(int id, QString dbName, QString hostName, QString userName, QString comment);
    QList<QString> getAllDbNames();
};

#endif // XMLHANDLER_H
