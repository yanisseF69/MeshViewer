#include "triangle.h"

Triangle::Triangle() {}

Triangle::Triangle(const unsigned int &v0, const unsigned int &v1, const unsigned int &v2) {
    idVertices[0] = v0;
    idVertices[1] = v1;
    idVertices[2] = v2;
}

int Triangle::localIndex(unsigned int indice) const {
    int res = -1;

    for (int i = 0; i < 3; i++) {
        if (idVertices[i] == indice) res = indice;
    }

    return res;
}
