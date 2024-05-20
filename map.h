// #pragma once

#ifndef MAP_H
#define MAP_H

#include <cmath>

#include <vector>
#include <iostream>
#include <fstream>
#include <stdint.h>


#define MapSize 128

void copySlice(std::vector<std::vector<std::vector<int>>>& map, int from, int to);


class MapInfo {
    public:
    MapInfo();
    ~MapInfo(){};

    void MapGenerator();

    void setStr(int x, int y, int z);

    void setDst(int x, int y, int z);

    void SaveMapToFile(const std::string& filename);
 
    std::vector<std::vector<std::vector<int>>> map;
};

#endif