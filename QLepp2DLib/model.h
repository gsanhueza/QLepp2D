#ifndef MODEL_H
#define MODEL_H

#include <QUrl>
#include "offhandler.h"

class Model
{
public:
    Model();
    ~Model();
    bool loadOFF(QUrl filepath) const;
    bool saveOFF(QUrl filepath) const;

private:
    OFFHandler *m_offhandler;
};

#endif // MODEL_H
