#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include "offhandler.h"

class Model
{
public:
    Model();
    ~Model();
    bool loadOFF(QString filepath) const;
    bool saveOFF(QString filepath) const;

private:
    OFFHandler *m_offhandler;
};

#endif // MODEL_H
