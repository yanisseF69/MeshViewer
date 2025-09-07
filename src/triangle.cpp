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
        if (idVertices[i] == indice) return i;
    }

    return res;
}

std::pair<int, int> Triangle::findCommonEdge(const Triangle &t) const {
    for (size_t i = 0; i < 3; i++) {
        int v1 = idVertices[i];
        int v2 = idVertices[(i + 1) % 3];

        for (size_t j = 0; j < 3; j++) {
            int u1 = t.idVertices[j];
            int u2 = t.idVertices[(j + 1) % 3];

            if (v1 == u2 && v2 == u1) {
                return std::make_pair(v1, v2);
            }
        }
    }

    return std::make_pair(-1, -1);

}
