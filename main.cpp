#include <iostream>
#include <cmath>
#include <cstdlib>
#include "map.h"
#include "astar.h"
#include "octree.h"

int main (int argc, char *argv[]) {
	
	MapInfo mapData;
	Astar astar;

	std::cout<<"Hello Octree!\n";
	mapData.MapGenerator();

	std::vector<std::vector<std::vector<int>>> map = mapData.map;
	
	Octree octree(map);
	// octree.setStr(0,0,0);
	// octree.setDst(MapSize-5,MapSize-5,0);
	mapData.setStr(0,0,0);
	mapData.setDst(MapSize-5,MapSize-5,0);

	
    

	astar.map = mapData.map;
	astar.getStr();
	astar.getDst();

	// std::cout<<std::get<0>(astar.dst[0])<<"\n";
	int x,y,z;
	std::tie(x,y,z) = astar.str[0];
	std::cout<<"Start : "<<x<<" "<<y<<" "<<z<<"\n";
	std::tie(x,y,z) = astar.dst[0];
	std::cout<<"Goal : "<<x<<" "<<y<<" "<<z<<"\n\n";
	
	astar.search();
	astar.constructPath();

	mapData.map = astar.map;
	mapData.SaveMapToFile("mapData.txt");

	x = MapSize - 5;
	y = MapSize - 5;
	z = 0;

	octree.updateNodeValue(x,y,z,5);
	octree.updateNodeValue(0,0,0,3);

	OctreeNode* node = octree.findNode(x, y, z);
	if (node != nullptr) {
        std::cout << "Node found at (" << x << ", " << y << ", " << z << ") with value: " << node->value << std::endl;
    } else {
        std::cout << "Node not found at (" << x << ", " << y << ", " << z << ")" << std::endl;
    }
	octree.saveToFile("octree.txt");
	// octree.printTree();
	
	return 0;
}
