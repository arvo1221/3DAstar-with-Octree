// octree.h
#ifndef OCTREE_H
#define OCTREE_H

#include <vector>
#include <fstream>
#include <algorithm>
#include <stack>
#include <sstream>
#include "map.h"

class OctreeNode {
public:
    bool isLeaf;
    int value; // 1 for obstacle, 0 for empty
    OctreeNode* children[8];

    OctreeNode();
    ~OctreeNode();
};

class Octree {
private:
    OctreeNode* root;
    int thresholdSize = 2;

    void buildTree(OctreeNode* node, int x, int y, int z, int size, const std::vector<std::vector<std::vector<int>>>& map);
    void saveTree(OctreeNode* node, std::ofstream& file);
public:
    Octree(const std::vector<std::vector<std::vector<int>>>& map);
    ~Octree();
    void saveToFile(const std::string& filename);

    OctreeNode* findNode(OctreeNode* node, int x, int y, int z, int size);
    OctreeNode* findNode(int x, int y, int z);
    void updateNodeValue(int x, int y, int z, int newValue);
};

#endif // OCTREE_H