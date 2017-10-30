#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include "offhandler.h"

class Model
{
public:
    Model();
    ~Model();
    bool loadOFF(QString &filepath);
    bool saveOFF(QString &filepath);

private:
    OFFHandler *m_offhandler;
    std::vector<float> m_vertices;
    std::vector<int> m_indices;
};

#endif // MODEL_H
