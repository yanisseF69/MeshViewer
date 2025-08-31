#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QVector2D>

struct Vertex
{
    QVector3D position;
    QVector3D normal;
    QVector2D texCoords;
};

#endif // VERTEX_H
