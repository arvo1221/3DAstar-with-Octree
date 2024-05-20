// #pragma once
#ifndef ASTAR_H
#define ASTAR_H

#include <cmath>

#include <vector>
#include <iostream>
#include <queue>
#include <tuple>
#include <algorithm>
#include <limits>
#include <stdint.h>
#include <string>
#include <string.h>
#include "map.h"

bool areVectorsEqual(const std::vector<std::tuple<int, int, int>>& v1, const std::vector<std::tuple<int, int, int>>& v2);
float heuristicDistance(const std::vector<std::tuple<int, int, int>>& v1, const std::vector<std::tuple<int, int, int>>& v2);
float node2Ground(const std::tuple<int, int, int>& node, const std::vector<std::vector<std::vector<int>>>& mapData);
int isTupleInVector(const std::vector<std::tuple<int, int, int>>& vec, const std::tuple<int, int, int>& tup);


using DataElement = std::pair<float, std::tuple<int, int, int>>;

class Astar {
    public:
    Astar();
    ~Astar(){};

    void getStr();

    void getDst();

    void search();

    void constructPath();

    int iter = 0;
    int minIndex = 0;
    int px,py,pz;
    int cx,cy,cz;
    float currentCost = 0;
    float prevCost = 0;
    float priority = 0;
    float candidateCost[6] = {0, 0, 0, 0, 0, 0};
    int depth = 64;

    std::vector<std::vector<std::vector<int>>> map;

    std::vector<std::tuple<int, int, int>> dst;
    std::vector<std::tuple<int, int, int>> str;
    std::vector<std::tuple<int, int, int>> current;

    std::vector<std::pair<float, std::tuple<int, int, int>>> frontier;
    std::vector<std::vector<std::vector<float>>> cost_so_far;
    std::vector<std::vector<std::vector<std::tuple<int, int, int>>>> came_from;

    float newCost = 0;

    std::vector<std::tuple<int, int, int>> candidateNode;
    std::vector<std::tuple<int, int, int>> pathNode;
    
    std::vector<std::vector<std::vector<DataElement>>> octreeNode;

};

#endif