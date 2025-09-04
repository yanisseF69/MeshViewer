#include "vertex.h"

Vertex::Vertex(const Vertex &v) : position(v.position), normal(v.normal), texCoords(v.texCoords) {

}

Vertex::Vertex(const float &x, const float &y, const float &z) : position(QVector3D(x, y, z)) {
    normal = QVector3D(0.0f, 0.0f, 0.0f);
    texCoords = QVector2D(0.0f, 0.0f);
}

Vertex::Vertex(const QVector3D &pos) : position(pos) {
    normal = QVector3D(0.0f, 0.0f, 0.0f);
    texCoords = QVector2D(0.0f, 0.0f);
}
