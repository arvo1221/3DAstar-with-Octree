#include "map.h"


void copySlice(std::vector<std::vector<std::vector<int>>>& map, int from, int to) {
    if (from < 0 || to < 0 || from >= MapSize || to >= MapSize) return; // 유효성 검사
    
    for (int i = 0; i < MapSize; ++i) {
        for (int j = 0; j < MapSize; ++j) {
            map[i][j][to] = map[i][j][from]; // 2D 벡터의 각 원소를 복사
        }
    }
}


MapInfo::MapInfo() {
    // memset(map,0,sizeof(map));
    map = std::vector<std::vector<std::vector<int> > >(MapSize, std::vector<std::vector<int> >(MapSize, std::vector<int>(MapSize, 0)));
}

void MapInfo::MapGenerator() {
    // map[MapSize/10][MapSize/10][0] = 1;
    for(int k = 0 ; k < 3; k++) {
            for(int i = MapSize/10 ; i < MapSize/8 ; i++) {
                for(int len = 0; len<MapSize/10 ; len++) {
                    map[i][len+MapSize/10][k] = 1;//y축 라벨링 len이 더 김
                    map[len+MapSize/10][i][k] = 1;//x축 길게 라벨링
                }
            }
    }

    for(int k = 0 ; k < 2 ; k++) {
        for(int i = 0; i < MapSize/5 ; i++) {
            for(int len = 0; len < 3 ; len++) {
                map[i][len+MapSize/8+MapSize/10+1][k] = 1;
            }
        }
    }

    for(int k = 0 ; k < 8 ; k++) {
        for(int i = 0; i < MapSize/4 ; i++) {
            for(int len = 0; len < 3 ; len++) {
                map[len+MapSize/4][i][k] = 1;
            }
        }
    }

    for(int k = 0 ; k < 2 ; k++) {
        for(int i = 0 ; i < MapSize/1.8 ; i++) {
            int bais = MapSize/3*2;
            map[bais-i][i][k] = 1;
            map[bais-i+1][i][k] = 1;
            map[bais-i+2][i][k] = 1;
            map[bais-i+3][i][k] = 1;
        }
    }

    for(int k = 0 ; k < 3 ; k++) {
        for(int i = MapSize/7*6 ; i < MapSize ; i++) {
            for(int j = MapSize/5*3 ; j<MapSize/5*4 ; j++) {
                map[i][j][k] = 1;
            }
        }
    }


}

void MapInfo::setStr(int x, int y, int z) {
    map[x][y][z] = 3;
}

void MapInfo::setDst(int x, int y, int z) {
    map[x][y][z] = 5;
}

void MapInfo::SaveMapToFile(const std::string& filename) {
    std::ofstream file(filename); // 파일 열기
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    for (int i = 0; i < MapSize; ++i) {
        for (int j = 0; j < MapSize; ++j) {
            for (int k = 0; k < MapSize; ++k) {
                file << map[i][j][k] << " ";
            }
            file << "\n";
        }
        file << "\n\n";
    }

    file.close(); // 파일 닫기
}


