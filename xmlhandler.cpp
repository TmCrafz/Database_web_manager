#include "xmlhandler.h"


XmlHandler::XmlHandler():
    FILENAME("databases.xml"),
    ROOTELEMENT("databases"),
    DBELEMENT("database"),
    IDATTR("id"),
    DBNAMEATTR("dbName"),
    HOSTNAMEATTR("hostName"),
    USERNAMEATTR("userName"),
    COMMENTATTR("comment")
{
}

bool XmlHandler::createXmlFile()
{
    QDomDocument document;
    QDomProcessingInstruction declaration = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.insertBefore(declaration, QDomNode());
    QDomElement rootElement = document.createElement(ROOTELEMENT);
    document.appendChild(rootElement);

    QFile file(FILENAME);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    else
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << document.toString();
        file.flush();
        file.close();
    }
    return true;
}

bool XmlHandler::saveDbChildIntoXml(int id, QString dbName, QString hostName, QString userName, QString comment)
{
    QDomDocument xmlDocument;
    QFile file(FILENAME);
    //Try to open file, if there is no file then try to create it.
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
           qDebug() << "Unable to open " + FILENAME;
           if (!createXmlFile())
               return false;
           if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           {
                qDebug() << "Unable to create file: " + FILENAME;
                return false;
           }
    }
    if (!xmlDocument.setContent(&file))
    {
        qDebug() << "Unable to load xml document";
        return false;
    }
    file.close();

    QDomElement rootElement = xmlDocument.firstChildElement();
    QDomElement databaseElement = xmlDocument.createElement(DBELEMENT);
    databaseElement.setAttribute(IDATTR, id);
    databaseElement.setAttribute(DBNAMEATTR, dbName);
    databaseElement.setAttribute(HOSTNAMEATTR, hostName);
    databaseElement.setAttribute(USERNAMEATTR, userName);
    databaseElement.setAttribute(COMMENTATTR, comment);
    rootElement.appendChild(databaseElement);

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << xmlDocument.toString();
    file.flush();
    file.close();
    return true;
}

QList<QString> XmlHandler::getAllDbNames()
{
    QList<QString> databasesNameList;
    QDomDocument xmlDocument;
    QFile xmlFile(FILENAME);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file";
        return databasesNameList;
    }
    else
    {
        if (!xmlDocument.setContent(&xmlFile))
        {
            qDebug() << "Unable to load xml document";
            return databasesNameList;
        }
        xmlFile.close();
    }
    QDomElement root = xmlDocument.firstChildElement();
    QDomNodeList rootNodeList = root.elementsByTagName(ROOTELEMENT);
    for (unsigned i {0}; i != rootNodeList.count(); ++i)
    {
        QDomNode databaseNode = rootNodeList.at(i);
        if (databaseNode.isElement())
        {
            QDomElement databaseElement = databaseNode.toElement();
            QString databaseName = databaseElement.attribute(DBNAMEATTR);
            databasesNameList.append(databaseName);
        }
    }
    return databasesNameList;
}



