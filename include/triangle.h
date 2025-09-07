#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

struct Triangle
{

    Triangle();
    Triangle(const unsigned int &, const unsigned int &, const unsigned int &);

    int localIndex(unsigned int indice) const;
    std::pair<int, int> findCommonEdge(const Triangle &t) const;

    unsigned int idVertices[3];
    std::vector<unsigned int> idFaces;
};

#endif // TRIANGLE_H
