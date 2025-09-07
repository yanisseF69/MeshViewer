#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QOpenGLFunctions>

#include "vertex.h"
#include "triangle.h"

/**
 * @brief The MeshError enum who returns error int type.
 */
enum MeshError {
    OK=0,
    FORMAT=1,
    READ=2,
    UNKNOWN
};

/**
 * @brief The Mesh class, used for mesh loading and processing.
 */
class Mesh : public QOpenGLFunctions
{
public:
    Mesh();

    const std::vector<Vertex> &getVertices() const;
    const std::vector<unsigned int> getIndices() const;

    /**
     * @brief Clear vertices and triangles vectors.
     */
    void clear();

    /**
     * @brief Connect all triangles with its neighbors.
     */
    void sew();

    /**
     * @brief Loading function who handle the file type
     * @param link
     * @return MeshError::OK if the function terminates correctly, other else.
     */
    int loadFile(const char* link);

    /**
     * @brief Loading .off file function.
     * @param link
     * @return MeshError::OK if the function terminates correctly, other else.
     */
    int loadOFF(const char* link);

    /**
     * @brief Loading .obj file function.
     * @param link
     * @return MeshError::OK if the function terminates correctly, other else.
     */
    int loadOBJ(const char* link);

    /**
     * @brief Loading .txt file function.
     * @param link
     * @return MeshError::OK if the function terminates correctly, other else.
     */
    int loadTXT(const char* link);

    void saveOFF(const char* file) const;

    float faceArea(int faceIndex) const;
    void computeNormals();
    QVector3D getCenter() const;
    float getBoundingRadius() const;
    void removeSuperTriangle();

    void triangleSplit(int p, int triIndex);
    void edgeFlip(int t1, int t2);
    void edgeSplit(int p, int t1, int t2);
    float orientationTest(int p, int q, int r) const;
    int pointInTriangle(int p, int triIndex) const;
    int insert(float x, float y, float z);
    bool isLocallyDelaunay(int t1, int t2) const;
    bool isInCircumcircleNorm(int a, int b, int c, int d) const;

    void lawsonAlgorithm();
    void lawsonLocalUpdate(int p);

private:

    std::vector<Vertex> vertices;
    std::vector<Triangle> faces;
};

#endif // MESH_H
