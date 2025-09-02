#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QOpenGLFunctions>

#include "vertex.h"

class Mesh : public QOpenGLFunctions
{
public:
    Mesh();

    const std::vector<Vertex> &getVertices() const;
    const std::vector<unsigned int> &getIndices() const;

    void clear();
    int loadFile(const char* link);
    void loadOFF(const char* link);
    void loadOBJ(const char* link);

    QVector3D getCenter() const;
    float getBoundingRadius() const;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

#endif // MESH_H
