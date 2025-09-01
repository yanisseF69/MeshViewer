#include "mesh.h"

#include <iostream>
#include <fstream>

Mesh::Mesh() {}

const std::vector<Vertex> &Mesh::getVertices() const{
    return vertices;
}

const std::vector<unsigned int> &Mesh::getIndices() const{
    return indices;
}

void Mesh::clear() {
    vertices.clear();
    indices.clear();
}

void Mesh::loadOFF(const char* link) {
    std::ifstream meshFile(link);
    if (!meshFile.is_open()) {
        std::cerr << "Can't open file \"" << link << "\"\n";
        exit(EXIT_FAILURE);
    }

    std::string header;
    size_t numVertices, numFaces, numEdges;
    meshFile >> header;
    meshFile >> numVertices >> numFaces >> numEdges;

    clear();

    for (size_t i = 0; i < numVertices; ++i) {
        float x, y, z;
        meshFile >> x >> y >> z;
        vertices.push_back(Vertex(x, y, z));
        vertices.back().normal = QVector3D(0,0,0);
    }

    for (size_t i = 0; i < numFaces; ++i) {
        int nVerts;
        meshFile >> nVerts;
        std::vector<unsigned int> faceIndices(nVerts);
        for (int j = 0; j < nVerts; ++j) {
            meshFile >> faceIndices[j];
        }

        for (int j = 1; j < nVerts - 1; ++j) {
            unsigned int i0 = faceIndices[0];
            unsigned int i1 = faceIndices[j];
            unsigned int i2 = faceIndices[j+1];

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            QVector3D &v0 = vertices[i0].position;
            QVector3D &v1 = vertices[i1].position;
            QVector3D &v2 = vertices[i2].position;

            QVector3D normal = QVector3D::crossProduct(v1 - v0, v2 - v0).normalized();

            vertices[i0].normal += normal;
            vertices[i1].normal += normal;
            vertices[i2].normal += normal;
        }
    }

    for (auto &v : vertices)
        v.normal.normalize();
}


void Mesh::computeNormals() {
    for (auto &v : vertices) {
        v.normal = QVector3D(0,0,0);
    }

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i+1];
        unsigned int i2 = indices[i+2];

        QVector3D &v0 = vertices[i0].position;
        QVector3D &v1 = vertices[i1].position;
        QVector3D &v2 = vertices[i2].position;

        QVector3D normal = QVector3D::crossProduct(v1 - v0, v2 - v0).normalized();

        vertices[i0].normal += normal;
        vertices[i1].normal += normal;
        vertices[i2].normal += normal;
    }

    for (auto &v : vertices) {
        v.normal.normalize();
    }
}
