#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QVector2D>

struct Vertex
{
    Vertex(const Vertex &v);
    Vertex(const float &x, const float &y, const float &z);
    Vertex(const QVector3D &pos);

    QVector3D position;
    QVector3D normal;
    QVector2D texCoords;
};

#endif // VERTEX_H
