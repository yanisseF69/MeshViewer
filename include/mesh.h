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
    void loadOFF(const char* link);
    void computeNormals();

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

#endif // MESH_H
