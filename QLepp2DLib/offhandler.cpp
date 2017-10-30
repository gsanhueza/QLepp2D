#include <QDebug>
#include "offhandler.h"

OFFHandler::OFFHandler()
{
}

bool OFFHandler::loadOffFile(QUrl filepath) const
{
    qDebug() << "TODO: load from " << filepath << endl;
    return false;
}

bool OFFHandler::saveOffFile(QUrl filepath) const
{
    qDebug() << "TODO: save to " << filepath << endl;
    return false;
}
