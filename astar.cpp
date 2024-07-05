#include "astar.h"

bool areVectorsEqual(const std::vector<std::tuple<int, int, int>>& v1, const std::vector<std::tuple<int, int, int>>& v2) {
    if (v1.size() != v2.size()) {
        std::cout<<"Warning Different Size\n";
        std::cout<< v1.size() << " " << v2.size() <<"\n";
        return false; // 크기가 다르면, 벡터는 동일할 수 없습니다.
    }

    return std::equal(v1.begin(), v1.end(), v2.begin(), [](const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b) {
        return std::get<0>(a) == std::get<0>(b) &&
               std::get<1>(a) == std::get<1>(b) &&
               std::get<2>(a) == std::get<2>(b);
    });
}

float heuristicDistance(const std::tuple<int, int, int>& a, const std::tuple<int, int, int>& b) {
    return std::abs(std::get<0>(a) - std::get<0>(b)) + 
           std::abs(std::get<1>(a) - std::get<1>(b)) +
           std::abs(std::get<2>(a) - std::get<2>(b));
}

float node2Ground(const std::tuple<int, int, int>& node, const std::vector<std::vector<std::vector<int>>>& mapData) {
    int x,y,z;
    float distance = 0;
    std::tie(x,y,z) = node;
    // std::cout<<"Cur Node "<<x<<" "<<y<<" "<<z<<"\n";
    for(int i = 0 ; i < MapSize-3 ; i++) {
        if((mapData[x][y][z-i]!=0)||(z-i<=0))   break;
        distance += 1;
    }
    // std::cout<<"Distance "<<distance<<"\n";

    return distance;
}

int isTupleInVector(const std::vector<std::tuple<int, int, int>>& vec, const std::tuple<int, int, int>& tup) {
    auto it = std::find(vec.begin(), vec.end(), tup);
    if (it != vec.end()) {
        // 튜플이 벡터에 존재함
        return 1;
    } else {
        // 튜플이 벡터에 존재하지 않음
        return 0;
    }
}

int findMinIndex(float* candidateCost) {
    float minCost = std::numeric_limits<float>::max(); // 가능한 가장 큰 수로 초기화
    int min_index = 0; // 인덱스 초기화
    for (int i = 0; i < 6 ; i++) {
        // printf("%d cost : %f\n",i,candidateCost[i]);
        if (candidateCost[i] > 0.5 && candidateCost[i] < minCost) {
            minCost = candidateCost[i];
            min_index = i;
        }
    }
    return min_index;
}

// int heuristicDistance(int *dst, int *str) {
//     return abs(dst[0]-str[0])+abs(dst[1]-str[1])+abs(dst[2]-str[2]);
// }

Astar::Astar() {
    iter = 0;
    currentCost = 0;
    //detectmap = std::vector<std::vector<std::vector<int> > >(MapSize, std::vector<std::vector<int> >(MapSize, std::vector<int>(MapSize, 0)));
    while(!candidateNode.empty()) {
        printf("empty candidate\n");
        candidateNode.pop_back();
    }
    while(!current.empty()) {
        printf("empty current\n");
        current.pop_back();
    }
    cost_so_far = std::vector<std::vector<std::vector<float> > >(MapSize, std::vector<std::vector<float> >(MapSize, std::vector<float>(MapSize, std::numeric_limits<float>::max())));
    came_from = std::vector<std::vector<std::vector<std::tuple<int, int, int>>>>(
            MapSize, std::vector<std::vector<std::tuple<int, int, int>>>(
                MapSize, std::vector<std::tuple<int, int, int>>(
                    MapSize, std::make_tuple(0, 0, 0))));

}

void Astar::getStr() {
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            for (size_t k = 0; k < map[i][j].size(); ++k) {
                if (map[i][j][k] == 3) {
                    // 조건을 만족하는 인덱스 저장
                    str.push_back(std::make_tuple(i, j, k));
                    // candidateNode.push_back(str[0]);
                    // printf("Str: %d %d %d\n",i,j,k);
                }
            }
        }
    }
}

void Astar::getDst() {
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            for (size_t k = 0; k < map[i][j].size(); ++k) {
                if (map[i][j][k] == 5) {
                    // 조건을 만족하는 인덱스 저장
                    dst.push_back(std::make_tuple(i, j, k));
                    // printf("Dst: %d %d %d\n",i,j,k);
                }
            }
        }
    }
}
//heuristicDistance(std::make_tuple(px+1, py, pz),dst[0]);
void Astar::search() {

    frontier.push_back({0,str[0]});

    std::tie(px,py,pz) = frontier[0].second;
    cost_so_far[px][py][pz] = 0;
    

    for(iter ; iter < MapSize*MapSize*MapSize ; iter++) {
        while(!current.empty()) {
            current.pop_back();
        }

        while(!candidateNode.empty()) {
            candidateNode.pop_back();
        }

        current.push_back(frontier[iter].second);

        // printf("%d\n",std::get<2>(current.back()));

        if(areVectorsEqual(current,dst)) {
            std::cout << "Goal node reached! Total cost: " << cost_so_far[px][py][pz] << std::endl; // goal node 도달 시 cost 출력
            break;
        }
        //현재
        std::tie(px,py,pz) = current[0];

        // std::cout<<"current node:"<<px<<" "<<py<<" "<<pz<<" iter:"<<iter<<"\n\n";

        //candidate node 탐색 시작

        for(int i = 0; i < 6 ; i++) {
            switch(i) {
                case 2:
                    if(py>=MapSize-1) continue;
                    candidateNode.push_back(std::make_tuple(px, py+1, pz));
                    break;
                case 3:
                    if(py<1) continue;
                    candidateNode.push_back(std::make_tuple(px, py-1, pz));
                    break;
                case 0:
                    if((px>=MapSize-1)) continue;
                    candidateNode.push_back(std::make_tuple(px+1, py, pz));
                    break;
                case 1:
                    if(px<1) continue;
                    candidateNode.push_back(std::make_tuple(px-1, py, pz));
                    break;
                case 4:
                    if(pz>=MapSize-1) continue;
                    candidateNode.push_back(std::make_tuple(px, py, pz+1));
                    break;
                case 5:
                    if(pz<1) continue;
                    candidateNode.push_back(std::make_tuple(px, py, pz-1));
                    break;
            }
        }
        // std::cout << "Number of tuples in candidateNode: " << candidateNode.size() << "\n";

        priority = std::numeric_limits<float>::max();
        minIndex = 0;
        for(int i = 0; i < candidateNode.size() ; i++) {
            std::tie(cx,cy,cz) = candidateNode[i];
            while(map[cx][cy][cz]==1) {
                // printf("Warning!!%d %d %d %d\n",cx,cy,cz,map[cx][cy][cz]);
                cz++;
                if(cz>MapSize-3)    break;
            }
            // printf("%d %d %d %d\n",cx,cy,cz,map[cx][cy][cz]);
            candidateNode[i] = std::make_tuple(cx, cy, cz);
            // std::cout<<"prev cost: "<<cost_so_far[px][py][pz]<<"\n";
            newCost = cost_so_far[px][py][pz] + heuristicDistance(candidateNode[i],current[0]) + 1.5*node2Ground(candidateNode[i],map);
            if(newCost < cost_so_far[cx][cy][cz]) {
                cost_so_far[cx][cy][cz] = newCost;
                currentCost = newCost + heuristicDistance(candidateNode[i],dst[0]);// + 1.5*node2Ground(candidateNode[i],map);
            
                if(currentCost < priority) {
                    minIndex = i;
                    priority = currentCost;
                    // std::tie(cx,cy,cz) = candidateNode[i];
                    frontier.push_back({priority,candidateNode[i]});
                    came_from[cx][cy][cz] = current[0];
                }
            }
        }
    }
}

void Astar::constructPath() {
    int x,y,z,i;

    int trackingLen = 0;
    while(!current.empty()) {
        current.pop_back();
    }
    current.push_back(dst[0]);
    
    for(int iter = 0 ; iter < MapSize*MapSize*MapSize ; iter++) {
        // std::cout << "iter: " << iter << "\n";
        if (areVectorsEqual(current, str)) {//dst 도달 유무 확인
            std::reverse(pathNode.begin(), pathNode.end());
            // std::cout << "Current and STR are equal.\n";
            // std::cout << "iter: " << iter << "\n";
            break;
        }

        pathNode.push_back(current[0]);

        std::tie(px,py,pz) = current[0];
        // printf("%d %d %d\n",px,py,pz);
        
        while(!current.empty()) {
            current.pop_back();
        }
        current.push_back(came_from[px][py][pz]);

        trackingLen = iter;
    }
    
    for (int iter = 0 ; iter < trackingLen ; iter++) {
        std::tie(x,y,z) = pathNode[iter];
        map[x][y][z] = 2;
        // std::cout<<"Final Path : "<<x<<" "<<y<<" "<<z<<"\n";
        // std::cout << "Path node: (" << x << ", " << y << ", " << z << ") with cost: " << cost_so_far[x][y][z] <<"\n";
    }
}

float Astar::calculatePathLength() {
    float totalLength = 0.0;
    for (size_t i = 1; i < pathNode.size(); ++i) {
        totalLength += heuristicDistance(pathNode[i-1], pathNode[i]);
    }
    return totalLength;
}