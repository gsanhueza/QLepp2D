#ifndef OFFHANDLER_H
#define OFFHANDLER_H

#include <QUrl>
#include "qlepp2dlib_global.h"

class QLEPP2DLIBSHARED_EXPORT OFFHandler
{

public:
    OFFHandler();
    bool loadOffFile(QUrl filepath) const;
    bool saveOffFile(QUrl filepath) const;
};

#endif // OFFHANDLER_H
