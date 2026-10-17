#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include <QString>

struct ObjectData
{
    QString name;
    QString type;
    double x;
    double y;
    double rotation;
    double scale;
    bool visible;
};//for storing object details like cube, rectangle, ellipse

#endif