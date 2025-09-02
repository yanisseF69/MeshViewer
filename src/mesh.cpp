#include "mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>

Mesh::Mesh() {}

const std::vector<Vertex> &Mesh::getVertices() const {
    return vertices;
}

const std::vector<unsigned int> &Mesh::getIndices() const {
    return indices;
}

void Mesh::clear() {
    vertices.clear();
    indices.clear();
}

int Mesh::loadFile(const char* link)
{
    std::string filename(link);
    std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

    if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".off") {
        loadOFF(link);
        return 0;
    }
    else if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".obj") {
        loadOBJ(link);
        return 0;
    }
    else {
        return -1;
    }
}

void Mesh::loadOFF(const char* link) {
    std::ifstream meshFile(link);
    if (!meshFile.is_open()) {
        std::cerr << "Can't open file \"" << link << "\"\n";
        exit(EXIT_FAILURE);
    }

    auto nextToken = [&](std::string &token) {
        while (meshFile >> token) {
            if (token[0] == '#') {
                std::string line;
                std::getline(meshFile, line);
                continue;
            }
            return true;
        }
        return false;
    };

    std::string header;
    if (!nextToken(header) || header != "OFF") {
        std::cerr << "Not a valid OFF file\n";
        exit(EXIT_FAILURE);
    }

    size_t numVertices = 0, numFaces = 0, numEdges = 0;
    std::string token;

    if (!nextToken(token)) exit(EXIT_FAILURE);
    numVertices = std::stoul(token);
    if (!nextToken(token)) exit(EXIT_FAILURE);
    numFaces = std::stoul(token);
    if (!nextToken(token)) exit(EXIT_FAILURE);
    numEdges = std::stoul(token);

    clear();

    for (size_t i = 0; i < numVertices; ++i) {
        float x, y, z;
        if (!(meshFile >> x >> y >> z)) {
            std::cerr << "Error reading vertex " << i << "\n";
            exit(EXIT_FAILURE);
        }
        vertices.push_back(Vertex(x, y, z));
        vertices.back().normal = QVector3D(0, 0, 0);
    }

    for (size_t i = 0; i < numFaces; ++i) {
        int nVerts;
        if (!(meshFile >> nVerts)) {
            std::cerr << "Error reading face size " << i << "\n";
            exit(EXIT_FAILURE);
        }

        std::vector<unsigned int> faceIndices(nVerts);
        for (int j = 0; j < nVerts; ++j) {
            meshFile >> faceIndices[j];
        }

        for (int j = 1; j < nVerts - 1; ++j) {
            unsigned int i0 = faceIndices[0];
            unsigned int i1 = faceIndices[j];
            unsigned int i2 = faceIndices[j + 1];

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

void Mesh::loadOBJ(const char* link) {
    std::ifstream meshFile(link);
    if (!meshFile.is_open()) {
        std::cerr << "Can't open file \"" << link << "\"\n";
        exit(EXIT_FAILURE);
    }

    clear();

    std::string line;
    while (std::getline(meshFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(Vertex(x, y, z));
            vertices.back().normal = QVector3D(0, 0, 0);
        }
        else if (prefix == "f") {
            std::vector<unsigned int> faceIndices;
            std::string token;
            while (iss >> token) {
                size_t pos = token.find('/');
                unsigned int idx = std::stoul(token.substr(0, pos));
                faceIndices.push_back(idx - 1);
            }

            int nVerts = (int)faceIndices.size();
            for (int j = 1; j < nVerts - 1; ++j) {
                unsigned int i0 = faceIndices[0];
                unsigned int i1 = faceIndices[j];
                unsigned int i2 = faceIndices[j + 1];

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
    }
}

QVector3D Mesh::getCenter() const {
    if (vertices.empty()) return QVector3D(0,0,0);
    QVector3D min = vertices[0].position;
    QVector3D max = vertices[0].position;
    for (const auto &v : vertices) {
        min.setX(std::min(min.x(), v.position.x()));
        min.setY(std::min(min.y(), v.position.y()));
        min.setZ(std::min(min.z(), v.position.z()));
        max.setX(std::max(max.x(), v.position.x()));
        max.setY(std::max(max.y(), v.position.y()));
        max.setZ(std::max(max.z(), v.position.z()));
    }
    return (min + max) * 0.5f;
}

float Mesh::getBoundingRadius() const {
    QVector3D center = getCenter();
    float maxDist = 0.0f;
    for (const auto &v : vertices) {
        maxDist = std::max(maxDist, (v.position - center).length());
    }
    return maxDist;
}
