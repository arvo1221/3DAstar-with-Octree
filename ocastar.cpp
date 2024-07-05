#include "ocastar.h"
#include <queue>
#include <unordered_map>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>

// Node 구조체는 A* 알고리즘의 각 노드를 나타냅니다.
struct Node {
    std::tuple<int, int, int> position; // 노드의 위치 (x, y, z)
    float g; // 시작 노드에서 현재 노드까지의 비용
    int h; // 현재 노드에서 목표 노드까지의 추정 비용 (휴리스틱)
    float f; // 총 비용 (g + h)
    std::tuple<int, int, int> parent; // 부모 노드의 위치

    // 기본 생성자
    Node() : position(std::make_tuple(0, 0, 0)), g(0), h(0), f(0), parent(std::make_tuple(0, 0, 0)) {}

    // 매개변수가 있는 생성자
    Node(std::tuple<int, int, int> pos, float g_val, int h_val, std::tuple<int, int, int> parent_node) 
        : position(pos), g(g_val), h(h_val), f(g_val + h_val), parent(parent_node) {}

    // f 값을 기준으로 노드 비교 (작을수록 우선순위가 높음)
    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

// OcAstar 클래스의 생성자
OcAstar::OcAstar(Octree* octree) : octree(octree) {
    // 시작 노드와 목표 노드를 기본 값으로 초기화
    // start = std::make_tuple(0, 0, 0);
    // goal = std::make_tuple(MapSize - 5, MapSize - 5, 0);
}

// 시작 노드 설정 함수
void OcAstar::setStr(int x, int y, int z) {
    start = std::make_tuple(x, y, z);
    OctreeNode* startNode = octree->findNode(x, y, z);
    if (startNode) {
        startNode->value = 3; // 시작 노드 값 설정
    } else {
        std::cerr << "Error: Start node not found in the octree!" << std::endl;
    }
}
// void OcAstar::setStr(int x, int y, int z) {
//     start = std::make_tuple(x, y, z);
//     OctreeNode* startNode = octree->findNode(x, y, z);
//     if (startNode) {
//         startNode->value = 3; // 시작 노드 값 설정
//         std::cout << "Start node set at: " << x << ", " << y << ", " << z << std::endl;
//     } else {
//         std::cerr << "Error: Start node not found in the octree!" << std::endl;
//     }
// }

// 목표 노드 설정 함수
void OcAstar::setDst(int x, int y, int z) {
    goal = std::make_tuple(x, y, z);
    OctreeNode* goalNode = octree->findNode(x, y, z);
    if (goalNode) {
        goalNode->value = 5; // 목표 노드 값 설정
            } else {
        std::cerr << "Error: Goal node not found in the octree!" << std::endl;
    }
}

float OcAstar::calculatePathLength() {
    float totalLength = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        totalLength += heuristic(path[i-1], path[i]);
    }
    return totalLength;
}

// 휴리스틱 함수: 두 노드 간의 맨해튼 거리 계산
int OcAstar::heuristic(const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b) {
    return std::abs(std::get<0>(a) - std::get<0>(b)) + std::abs(std::get<1>(a) - std::get<1>(b)) + std::abs(std::get<2>(a) - std::get<2>(b));
}

// 이웃 노드를 반환하는 함수
std::vector<std::tuple<int, int, int>> OcAstar::getNeighbors(const std::tuple<int, int, int>& node) {
    std::vector<std::tuple<int, int, int>> neighbors;
    int x, y, z;
    std::tie(x, y, z) = node;

    std::vector<std::tuple<int, int, int>> possible_moves = {
        {x, y+1, z}, {x, y-1, z}, {x+1, y, z}, {x-1, y, z},
        {x, y, z+1}, {x, y, z-1}
    };

    for (const auto& move : possible_moves) {
        if (isValidNode(move)) {
            neighbors.push_back(move);
        }
    }

    return neighbors;
}

// 노드가 유효한지 확인하는 함수 (장애물이 아닌지 확인)
bool OcAstar::isValidNode(const std::tuple<int, int, int>& node) {
    OctreeNode* octreeNode = octree->findNode(std::get<0>(node), std::get<1>(node), std::get<2>(node));
    return octreeNode && octreeNode->value != 1;
}

// 노드에서 지면까지의 거리 계산 함수
float OcAstar::node2Ground(const std::tuple<int, int, int>& node) {
    int x, y, z;
    std::tie(x, y, z) = node;

    int orinZ = z;

    while (z > 0 && octree->findNode(x, y, z)->value != 0) {
        z--;
    }

    return orinZ-z;
}

// A* 탐색 알고리즘 함수
void OcAstar::search() {
    // 우선순위 큐와 모든 노드를 저장할 해시맵 초기화
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_list;
    std::unordered_map<std::tuple<int, int, int>, Node, TupleHash> all_nodes;

    // 시작 노드를 초기화하고 큐에 추가
    Node start_node(start, 0, heuristic(start, goal), start);
    open_list.push(start_node);
    all_nodes[start] = start_node;

    // 큐가 비어있지 않은 동안 반복
    while (!open_list.empty()) {
        // 큐에서 가장 높은 우선순위 노드(비용이 가장 적은 노드)를 추출
        Node current = open_list.top();
        open_list.pop();

        // 목표 노드에 도달한 경우
        if (current.position == goal) {
            std::tuple<int, int, int> pos = goal;
            std::cout << "Goal node reached! Total cost: " << current.g << "\n"; // 경로 길이 출력
            while (pos != start) {
                path.push_back(pos);
                pos = all_nodes[pos].parent;
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            // std::cout << "Path found!\n";
            for (const auto& node : path) {
                int x, y, z;
                std::tie(x, y, z) = node;
                // std::cout << "Path node: " << x << ", " << y << ", " << z << "\n";
            }
            return;
        }

        // 현재 노드의 이웃 노드를 처리
        for (const auto& neighbor : getNeighbors(current.position)) {
            int cx, cy, cz;
            std::tie(cx, cy, cz) = neighbor;
            int px, py, pz;
            std::tie(px, py, pz) = current.position;

            // 필요한 경우 cz 값을 지면 위로 업데이트
            while (octree->findNode(cx, cy, cz)->value == 1) {
                cz++;
                if (cz > MapSize - 3) break;
            }
            std::tuple<int, int, int> adjusted_neighbor = std::make_tuple(cx, cy, cz);

            // 새로운 비용 계산: g값 + 휴리스틱 거리 + 지면까지의 거리
            float newCost = all_nodes[current.position].g + heuristic(adjusted_neighbor, current.position) + 1.0 * node2Ground(adjusted_neighbor);
            if (all_nodes.find(adjusted_neighbor) == all_nodes.end() || newCost < all_nodes[adjusted_neighbor].g) {
                float priority = newCost + heuristic(adjusted_neighbor, goal);
                Node neighbor_node(adjusted_neighbor, newCost, heuristic(adjusted_neighbor, goal), current.position);
                open_list.push(neighbor_node);
                all_nodes[adjusted_neighbor] = neighbor_node;
            }
        }
    }
}

// 경로를 구성하는 함수
void OcAstar::constructPath() {
    for (const auto& node : path) {
        int x, y, z;
        std::tie(x, y, z) = node;
        // std::cout << "Marking path at: " << x << ", " << y << ", " << z << "\n";
        OctreeNode* octreeNode = octree->findNode(x, y, z);
        octreeNode->value = 2;

        // std::cout << "Marking path at: " << x << ", " << y << ", " << z <<", "<< octreeNode->value << "\n";
    }

    for (const auto& node : path) {
        int x, y, z;
        std::tie(x, y, z) = node;
        // std::cout << "Marking path at: " << x << ", " << y << ", " << z << "\n";
        OctreeNode* octreeNode = octree->findNode(x, y, z);
        
        if (std::make_tuple(x, y, z) == start) {
            octreeNode->value = 3;  // 시작 노드
            // std::cout << "Node at (" << x << ", " << y << ", " << z << ") is start, marked as 3\n";
        } else if (std::make_tuple(x, y, z) == goal) {
            octreeNode->value = 5;  // 목표 노드
            // std::cout << "Node at (" << x << ", " << y << ", " << z << ") is goal, marked as 5\n";
        }
        

        // std::cout << "Marking path at: " << x << ", " << y << ", " << z <<", "<< octreeNode->value << "\n";
    }

    // for (const auto& node : path) {
    //     int x, y, z;
    //     std::tie(x, y, z) = node;
    //     // std::cout << "Marking path at: " << x << ", " << y << ", " << z << "\n";
    //     OctreeNode* octreeNode = octree->findNode(x, y, z);
        
    //     std::cout << "Marking path at: " << x << ", " << y << ", " << z <<", "<< octreeNode->value << "\n";
    // }
    
}

void OcAstar::savePathToFile(const std::string& filename) {
    octree->saveToFile(filename);
    
}
