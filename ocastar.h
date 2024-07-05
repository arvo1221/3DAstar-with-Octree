#ifndef OCASTAR_H
#define OCASTAR_H

#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>
#include "octree.h"

class OcAstar {
public:
    OcAstar(Octree* octree);

    void search();
    void constructPath();
    void savePathToFile(const std::string& filename);
    void setStr(int x, int y, int z);
    void setDst(int x, int y, int z);
    float calculatePathLength();

private:
    Octree* octree;
    std::vector<std::tuple<int, int, int>> path;
    std::tuple<int, int, int> start;
    std::tuple<int, int, int> goal;

    int heuristic(const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b);
    std::vector<std::tuple<int, int, int>> getNeighbors(const std::tuple<int, int, int>& node);
    bool isValidNode(const std::tuple<int, int, int>& node);
    float node2Ground(const std::tuple<int, int, int>& node);

    struct TupleHash {
        std::size_t operator()(const std::tuple<int, int, int>& t) const {
            std::size_t h1 = std::hash<int>{}(std::get<0>(t));
            std::size_t h2 = std::hash<int>{}(std::get<1>(t));
            std::size_t h3 = std::hash<int>{}(std::get<2>(t));
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
};

#endif // OCASTAR_H
