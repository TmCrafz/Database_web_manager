#include "xmlhandler.h"

const QString XmlHandler::INTERNALDBNAMEATTR = "internalDbName";
const QString XmlHandler::DBNAMEATTR = "dbName";
const QString XmlHandler::HOSTNAMEATTR = "hostName";
const QString XmlHandler::USERNAMEATTR = "userName";
const QString XmlHandler::COMMENTATTR = "comment";

XmlHandler::XmlHandler():
    FILENAME("databases.xml"),
    ROOTELEMENT("databases"),
    DBELEMENT("database")
    //IDATTR("id")
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

void XmlHandler::saveDbs(QList<Database> &dbList)
{    
    QFile file(FILENAME);
    //Try to open file, if there is no file then try to create it.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))    
        qDebug() << "Unable to open " + FILENAME;


    QDomDocument document;
    QDomProcessingInstruction declaration = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    document.insertBefore(declaration, QDomNode());
    QDomElement rootElement = document.createElement(ROOTELEMENT);
    document.appendChild(rootElement);
    qDebug() << "Before foreach in Save";
    for (Database db : dbList)
    {
        qDebug() << "Before create element";
        QDomElement dbElement = document.createElement(DBELEMENT);       
        qDebug() << "After create Element";
        dbElement.setAttribute(INTERNALDBNAMEATTR, db.getComment());
        qDebug() << "After save firstEle";
        dbElement.setAttribute(DBNAMEATTR, db.getDbName());
        dbElement.setAttribute(HOSTNAMEATTR, db.getHostName());
        dbElement.setAttribute(USERNAMEATTR, db.getUserName());
        dbElement.setAttribute(COMMENTATTR, db.getComment());
        rootElement.appendChild(dbElement);
    }
    qDebug() << "After foreach in Save";

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << document.toString();
    file.flush();
    file.close();
}

QList<Database> XmlHandler::getAllDbs()
{
    QList<Database> databasesList;
    QDomDocument document;
    QFile file(FILENAME);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file";
        return databasesList;
    }
    else
    {
        if (!document.setContent(&file))
        {
            file.close();
            return databasesList;
        }
        file.close();
    }
    QDomElement root = document.firstChildElement();
    QDomNodeList rootNodeList = root.elementsByTagName(DBELEMENT);
    for (unsigned i = 0; i != rootNodeList.count(); ++i)
    {
        QDomNode databaseNode = rootNodeList.at(i);
        if (databaseNode.isElement())
        {
            QDomElement databaseElement = databaseNode.toElement();
            QString internalDbName = databaseElement.attribute(INTERNALDBNAMEATTR);
            QString dbName = databaseElement.attribute(DBNAMEATTR);
            QString hostName = databaseElement.attribute(HOSTNAMEATTR);
            QString userName = databaseElement.attribute(USERNAMEATTR);
            QString comment = databaseElement.attribute(COMMENTATTR);
            Database database(Database::cntID++, internalDbName, dbName, hostName, userName, "", comment);
            databasesList.append(database);
        }
    }
    file.close();
    return databasesList;
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
            xmlFile.close();
            return databasesNameList;
        }
        xmlFile.close();
    }
    QDomElement root = xmlDocument.firstChildElement();
    QDomNodeList rootNodeList = root.elementsByTagName(DBELEMENT);
    for (unsigned i = 0; i != rootNodeList.count(); ++i)
    {        
        QDomNode databaseNode = rootNodeList.at(i);
        if (databaseNode.isElement())
        {
            QDomElement databaseElement = databaseNode.toElement();
            QString databaseName = databaseElement.attribute(INTERNALDBNAMEATTR);
            databasesNameList.append(databaseName);            
        }
    }   
    xmlFile.close();
    return databasesNameList;
}

QString XmlHandler::loadQStringAttribute(const QString InternDbName, const QString ATTR)
{
    QString loadedAttr;
    QDomDocument xmlDocument;
    QFile xmlFile(FILENAME);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file";
        return loadedAttr;
    }
    else
    {
        if (!xmlDocument.setContent(&xmlFile))
            return loadedAttr;
        xmlFile.close();
    }
    qDebug() << "after xmlDoc";
    QDomElement root = xmlDocument.firstChildElement();
    QDomNodeList rootNodeList = root.elementsByTagName(DBELEMENT);
    for (unsigned i = 0; i != rootNodeList.count(); ++i)
    {
        qDebug() << "loop loadQSting";
        QDomNode databaseNode = rootNodeList.at(i);
        if (databaseNode.isElement())
        {
            QDomElement databaseElement = databaseNode.toElement();
            if (databaseElement.attribute(INTERNALDBNAMEATTR) == InternDbName)
                loadedAttr = databaseElement.attribute(ATTR);
        }
    }
    xmlFile.close();
    return loadedAttr;
}



