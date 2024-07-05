// octree.cpp
#include "octree.h"

// Constructor for OctreeNode
OctreeNode::OctreeNode() : isLeaf(true), value(0) {
    for (int i = 0; i < 8; ++i) {
        children[i] = nullptr;
    }
}

// Destructor for OctreeNode
OctreeNode::~OctreeNode() {
    for (int i = 0; i < 8; ++i) {
        delete children[i];
    }
}

// Constructor for Octree
Octree::Octree(const std::vector<std::vector<std::vector<int>>>& map) {
    root = new OctreeNode();
    buildTree(root, 0, 0, 0, MapSize, map);
}

// Destructor for Octree
Octree::~Octree() {
    delete root;
}

// Function to build the octree
// void Octree::buildTree(OctreeNode* node, int x, int y, int z, int size, const std::vector<std::vector<std::vector<int>>>& map) {
//     if (size == 1) {
//         int val = map[x][y][z];
//         node->value = (val == 1) ? 1 : 0; // Treat only 1 as occupied, others as empty
//         return;
//     }

//     int halfSize = size / 2;
//     bool sameValue = true;
//     int firstValue = map[x][y][z];
//     if (firstValue == 1) firstValue = 1; // Treat 1 as occupied
//     else firstValue = 0; // Treat 0, 3, 5 as empty for comparison

//     for (int i = 0; i < size && sameValue; ++i) {
//         for (int j = 0; j < size && sameValue; ++j) {
//             for (int k = 0; k < size && sameValue; ++k) {
//                 int currentValue = map[x + i][y + j][z + k];
//                 if (currentValue == 1) currentValue = 1; // Treat 1 as occupied
//                 else currentValue = 0; // Treat 0, 3, 5 as empty for comparison
//                 if (currentValue != firstValue) {
//                     sameValue = false;
//                 }
//             }
//         }
//     }

//     if (sameValue) {
//         node->value = (firstValue == 1) ? 1 : 0; // Only 1 remains as occupied
//         node->isLeaf = true;
//     } else {
//         node->isLeaf = false;
//         for (int i = 0; i < 8; ++i) {
//             node->children[i] = new OctreeNode();
//         }

//         buildTree(node->children[0], x, y, z, halfSize, map);
//         buildTree(node->children[1], x + halfSize, y, z, halfSize, map);
//         buildTree(node->children[2], x, y + halfSize, z, halfSize, map);
//         buildTree(node->children[3], x + halfSize, y + halfSize, z, halfSize, map);
//         buildTree(node->children[4], x, y, z + halfSize, halfSize, map);
//         buildTree(node->children[5], x + halfSize, y, z + halfSize, halfSize, map);
//         buildTree(node->children[6], x, y + halfSize, z + halfSize, halfSize, map);
//         buildTree(node->children[7], x + halfSize, y + halfSize, z + halfSize, halfSize, map);
//     }
// }
void Octree::buildTree(OctreeNode* node, int x, int y, int z, int size, const std::vector<std::vector<std::vector<int>>>& map) {
    // Check if node size is below threshold
    if (size <= thresholdSize) {
        // If size is less than or equal to threshold, determine if this node contains an obstacle
        for (int i = x; i < x + size; ++i) {
            for (int j = y; j < y + size; ++j) {
                for (int k = z; k < z + size; ++k) {
                    if (map[i][j][k] == 1) { // Check if there is an obstacle
                        node->isLeaf = true;
                        node->value = 1;
                        return;
                    }
                }
            }
        }
        // If no obstacle is found, mark as empty
        node->isLeaf = true;
        node->value = 0;
        return;
    }

    // Check if current cube contains any obstacles
    bool hasObstacle = false;
    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            for (int k = z; k < z + size; ++k) {
                if (map[i][j][k] == 1) { // Check if there is an obstacle
                    hasObstacle = true;
                    break;
                }
            }
            if (hasObstacle) break;
        }
        if (hasObstacle) break;
    }

    if (!hasObstacle) {
        // If no obstacle is found in the current cube, mark as empty and do not create children
        node->isLeaf = true;
        node->value = 0;
        return;
    }

    // Otherwise, split into children
    node->isLeaf = false;
    int halfSize = size / 2;
    for (int i = 0; i < 8; ++i) {
        node->children[i] = new OctreeNode();
        int newX = x + (i & 1) * halfSize;
        int newY = y + ((i >> 1) & 1) * halfSize;
        int newZ = z + ((i >> 2) & 1) * halfSize;
        buildTree(node->children[i], newX, newY, newZ, halfSize, map);
    }
}


// Function to save the octree to a file
void Octree::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    saveTree(root, file);
    file.close();
}

// Recursive function to save the octree nodes to a file
void Octree::saveTree(OctreeNode* node, std::ofstream& file) {
    if (node->isLeaf) {
        switch (node->value) {
            case 1:
                file << "1";
                break;
            case 0:
                file << "0";
                break;
            case 3:
                file << "3";
                break;
            case 5:
                file << "5";
                break;
            case 2:
                file << "2";
                break;
            default:
                file << "0";  // Default case, should not happen
                break;
        }
    } else {
        file << "X";
        for (int i = 0; i < 8; ++i) {
            saveTree(node->children[i], file);
        }
    }
}

OctreeNode* Octree::findNode(OctreeNode* node, int x, int y, int z, int size) {
    if (node->isLeaf) {
        return node;
    }

    int halfSize = size / 2;
    int index = 0;
    if (x >= halfSize) index |= 1;
    if (y >= halfSize) index |= 2;
    if (z >= halfSize) index |= 4;

    int newX = x % halfSize;
    int newY = y % halfSize;
    int newZ = z % halfSize;

    return findNode(node->children[index], newX, newY, newZ, halfSize);
}

OctreeNode* Octree::findNode(int x, int y, int z) {
    return findNode(root, x, y, z, MapSize);
}

void Octree::updateNodeValue(int x, int y, int z, int newValue) {
    OctreeNode* node = findNode(x, y, z);
    if (node != nullptr && node->isLeaf) {
        if (newValue == 3 || newValue == 5) {
            node->value = newValue;
        } else {
            std::cout << "Invalid value. Only 3 and 5 are allowed.\n";
        }
    } else {
        std::cout << "Node not found or the node is not a leaf node.\n";
    }
}

