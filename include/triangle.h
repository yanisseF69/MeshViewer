#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include <array>

struct Triangle
{

    Triangle();
    Triangle(const unsigned int &, const unsigned int &, const unsigned int &);

    int localIndex(unsigned int indice) const;
    std::pair<int, int> findCommonEdge(const Triangle &t) const;

    std::array<unsigned int, 3> idVertices;
    std::vector<unsigned int> idFaces;
};

#endif // TRIANGLE_H
