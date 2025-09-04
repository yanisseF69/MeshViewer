#include "mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

Mesh::Mesh() {}

const std::vector<Vertex> &Mesh::getVertices() const {
    return vertices;
}

const std::vector<unsigned int> Mesh::getIndices() const {
    std::vector<unsigned int> indices;
    for (auto &f: faces) {
        indices.push_back(f.idVertices[0]);
        indices.push_back(f.idVertices[1]);
        indices.push_back(f.idVertices[2]);
    }

    return indices;
}

void Mesh::clear() {
    vertices.clear();
    faces.clear();
}

void Mesh::sew() {
    std::map<std::pair<int, int>, std::pair<int, int>> map; // idVertice, idVertice, idFace, idOppositeVertice
    for(size_t i = 0; i < faces.size(); i++) {
        for(int j = 0; j < 3; j++) {
            std::pair<int, int> tmp(faces[i].idVertices[j], faces[i].idVertices[(j+1)%3]);
            std::pair<int, int> tmp2(faces[i].idVertices[(j+1)%3], faces[i].idVertices[j]);
            if(map.count(tmp2) == 0) {
                std::pair<int, int> face(i, (j+2)%3);
                map.insert(std::make_pair(tmp, face));
            } else {
                std::pair<int,int> incidentFace = map.at(tmp2);
                faces[i].idFaces.push_back(incidentFace.first);
                faces[incidentFace.first].idFaces.push_back(i);

            }

        }
    }
}

int Mesh::loadFile(const char* link) {
    std::string filename(link);
    std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    int ok;

    if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".off") {
        ok = loadOFF(link);
        return ok;
    } else if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".obj") {
        ok = loadOBJ(link);
        return ok;
    } else if (filename.size() >= 4 && filename.substr(filename.size() - 4) == ".txt") {
        ok = loadTXT(link);
        return ok;
    } else {
        return MeshError::FORMAT;
    }
}

int Mesh::loadOFF(const char* link) {
    std::ifstream meshFile(link);
    if (!meshFile.is_open()) {
        return MeshError::READ;
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
        return MeshError::FORMAT;
    }

    size_t numVertices = 0, numFaces = 0, numEdges = 0;
    std::string token;

    if (!nextToken(token)) return MeshError::READ;
    numVertices = std::stoul(token);
    if (!nextToken(token)) return MeshError::READ;
    numFaces = std::stoul(token);
    if (!nextToken(token)) return MeshError::READ;
    numEdges = std::stoul(token);

    clear();

    for (size_t i = 0; i < numVertices; ++i) {
        float x, y, z;
        if (!(meshFile >> x >> y >> z)) {
            return MeshError::READ;
        }
        vertices.push_back(Vertex(x, y, z));
    }

    for (size_t i = 0; i < numFaces; ++i) {
        int nVerts;
        if (!(meshFile >> nVerts)) {
            return MeshError::READ;
        }

        std::vector<unsigned int> faceIndices(nVerts);
        for (int j = 0; j < nVerts; ++j) {
            meshFile >> faceIndices[j];
        }

        for (int j = 1; j < nVerts - 1; ++j) {
            faces.push_back(Triangle(faceIndices[0], faceIndices[j], faceIndices[j + 1]));
        }
    }

    sew();
    computeNormals();

    return MeshError::OK;
}

int Mesh::loadOBJ(const char* link) {
    std::ifstream meshFile(link);
    if (!meshFile.is_open()) {
        return MeshError::READ;
    }

    clear();

    std::vector<QVector3D> tempPositions;
    std::vector<QVector3D> tempNormals;
    std::vector<QVector2D> tempTexCoords;

    std::string line;
    while (std::getline(meshFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            tempPositions.emplace_back(x, y, z);
        } else if (prefix == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            tempNormals.emplace_back(x, y, z);
        } else if (prefix == "vt") {
            float x, y;
            iss >> x >> y;
            tempTexCoords.emplace_back(x, y);
        } else if (prefix == "f") {
            std::string token;
            std::vector<unsigned int> faceIndices;

            while (iss >> token) {
                unsigned int vi = 0, ni = 0, ti;
                // format v//n or v/t/n
                if (sscanf(token.c_str(), "%u//%u", &vi, &ni) == 2) {
                    // v//n
                }
                else if (sscanf(token.c_str(), "%u/%*u/%u", &vi, &ni) == 2) {
                    // v/t/n
                }
                else if (sscanf(token.c_str(), "%u", &vi) == 1) {
                    // only v
                }

                vi--; ni--; ti--;

                Vertex v(tempPositions[vi]);
                if (ni < tempNormals.size() && ni >= 0) {
                    v.normal = tempNormals[ni];
                }
                if (ti < tempTexCoords.size() && ti >= 0) {
                    v.texCoords = tempTexCoords[ni];
                }

                vertices.push_back(v);
                faceIndices.push_back(vertices.size() - 1);
            }

            int nVerts = (int)faceIndices.size();
            for (int j = 1; j < nVerts - 1; ++j) {
                faces.push_back(Triangle(faceIndices[0], faceIndices[j], faceIndices[j + 1]));
            }
        }
    }

    sew();

    return MeshError::OK;
}

int Mesh::loadTXT(const char* link) {
    std::ifstream meshFile(link);
    if (!meshFile.is_open()) {
        return READ;
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

    size_t numVertices = 0;
    std::string token;

    if (!nextToken(token)) return MeshError::READ;
    try {
        numVertices = std::stoul(token);
    } catch (const std::invalid_argument&) {
        return MeshError::FORMAT;
    } catch (const std::out_of_range&) {
        return MeshError::FORMAT;
    }

    clear();

    for (size_t i = 0; i < numVertices; ++i) {
        float x, y, z;
        if (!(meshFile >> x >> y >> z)) {
            return MeshError::READ;
        }
        vertices.push_back(Vertex(x, y, z));
    }

    computeNormals();

    return MeshError::OK;
}

float Mesh::faceArea(int faceIndex) const {
    QVector3D a = vertices[faces[faceIndex].idVertices[0]].position;
    QVector3D b = vertices[faces[faceIndex].idVertices[1]].position;
    QVector3D c = vertices[faces[faceIndex].idVertices[2]].position;
    QVector3D ba = QVector3D(a - b);
    QVector3D ca = QVector3D(a - c);
    return QVector3D::dotProduct(ba, ca) / 2;
}

void Mesh::computeNormals() {
    for (auto &v : vertices) {
        v.normal = QVector3D(0,0,0);
    }

    for (size_t i = 0; i < faces.size(); i ++) {
        unsigned int i0 = faces[i].idVertices[0];
        unsigned int i1 = faces[i].idVertices[1];
        unsigned int i2 = faces[i].idVertices[2];

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
