#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include "map.h"
#include "astar.h"
#include "octree.h"
#include "ocastar.h"

int main(int argc, char *argv[]) {
    MapInfo mapData;
    Astar astar;

    std::cout << "Hello Octree!\n";
    mapData.MapGenerator();

    std::vector<std::vector<std::vector<int>>> map = mapData.map;

    
    mapData.setStr(0, 1, 0);
    mapData.setDst(MapSize - 5, MapSize - 5, 0);

    astar.map = mapData.map;
    astar.getStr();
    astar.getDst();

    int x, y, z;
    std::tie(x, y, z) = astar.str[0];
    std::cout << "Start: " << x << " " << y << " " << z << "\n";
    std::tie(x, y, z) = astar.dst[0];
    std::cout << "Goal: " << x << " " << y << " " << z << "\n\n";
    auto startA = std::chrono::high_resolution_clock::now();
    astar.search();
    astar.constructPath();
    auto endA = std::chrono::high_resolution_clock::now();
    auto durationA = std::chrono::duration_cast<std::chrono::microseconds>(endA - startA);

    mapData.map = astar.map;
    mapData.SaveMapToFile("mapData.txt");
    auto startO = std::chrono::high_resolution_clock::now();
    Octree octree(map);
    auto endO = std::chrono::high_resolution_clock::now();
    auto durationO = std::chrono::duration_cast<std::chrono::microseconds>(endO - startO);
    x = MapSize - 5;
    y = MapSize - 5;
    z = 0;

    octree.updateNodeValue(x, y, z, 5);
    octree.updateNodeValue(0, 0, 0, 3);

    OctreeNode* node = octree.findNode(x, y, z);
    // if (node != nullptr) {
    //     std::cout << "Node found at (" << x << ", " << y << ", " << z << ") with value: " << node->value << std::endl;
    // } else {
    //     std::cout << "Node not found at (" << x << ", " << y << ", " << z << ")" << std::endl;
    // }
    octree.saveToFile("octree.txt");

    // Octree A* Search
    OcAstar ocAstar(&octree);
	ocAstar.setStr(0, 1, 0); // Set start node
    ocAstar.setDst(MapSize - 5, MapSize - 5, 0); // Set goal node
    auto startOA = std::chrono::high_resolution_clock::now();
    ocAstar.search();
    ocAstar.constructPath();
    auto endOA = std::chrono::high_resolution_clock::now();
    auto durationOA = std::chrono::duration_cast<std::chrono::microseconds>(endOA - startOA);
    // ocAstar.setStr(0, 0, 0); // Set start node
    ocAstar.savePathToFile("ocastar.txt");

    std::cout<<"\nAstar Path Length: "<<astar.calculatePathLength()<<"m\n";
    std::cout<<"OcAstar Path Length: "<<ocAstar.calculatePathLength()<<"m\n\n";
    
    // std::cout<<"Astar "<< durationA.count()<<"us\n";
    // std::cout<<"OcAstar with Octree "<< durationO.count()+durationOA.count()<<"us\n";
    // std::cout<<"Octree "<< durationO.count()<<"us\n";
    // std::cout<<"OcAstar "<< durationOA.count()<<"us\n\n";

    std::cout << "Astar: " << durationA.count() / 1000.0 << " ms\n";
    std::cout << "OcAstar with Octree: " << (durationO.count() + durationOA.count()) / 1000.0 << " ms\n";
    std::cout << "Octree: " << durationO.count() / 1000.0 << " ms\n";
    std::cout << "OcAstar: " << durationOA.count() / 1000.0 << " ms\n";


    return 0;
}
