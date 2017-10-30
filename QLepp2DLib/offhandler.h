#ifndef OFFHANDLER_H
#define OFFHANDLER_H

#include <QString>
#include "qlepp2dlib_global.h"

class QLEPP2DLIBSHARED_EXPORT OFFHandler
{

public:
    OFFHandler();
    bool loadOffFile(QString filepath) const;
    bool saveOffFile(QString filepath) const;
};

#endif // OFFHANDLER_H
