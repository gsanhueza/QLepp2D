#include <QDebug>
#include <QFile>
#include "offhandler.h"

OFFHandler::OFFHandler()
{
}

bool OFFHandler::loadOffFile(QString filepath) const
{
    qDebug() << "TODO: load from " << filepath << endl;

    QFile inputFile(filepath);

    if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&inputFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            qDebug() << line;
        }
        inputFile.close();

        return true;
    }
    return false;
}

bool OFFHandler::saveOffFile(QString filepath) const
{
    qDebug() << "TODO: save to " << filepath << endl;

    QFile outputFile(filepath);

    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&outputFile);
        out << "TODO: Fill OFF file" << endl;

        outputFile.close();

        return true;
    }
    return false;
}
