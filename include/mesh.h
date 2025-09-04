#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QOpenGLFunctions>

#include "vertex.h"
#include "triangle.h"

enum MeshError {
    OK=0,
    FORMAT=1,
    READ=2,
    UNKNOWN
};

class Mesh : public QOpenGLFunctions
{
public:
    Mesh();

    const std::vector<Vertex> &getVertices() const;
    const std::vector<unsigned int> getIndices() const;

    void clear();
    void sew();

    int loadFile(const char* link);
    int loadOFF(const char* link);
    int loadOBJ(const char* link);
    int loadTXT(const char* link);

    float faceArea(int faceIndex) const;
    void computeNormals();
    QVector3D getCenter() const;
    float getBoundingRadius() const;

private:

    std::vector<Vertex> vertices;
    std::vector<Triangle> faces;
};

#endif // MESH_H
