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
    map = std::vector<std::vector<std::vector<int> > >(MapSize, std::vector<std::vector<int> >(MapSize, std::vector<int>(MapSize, 0)));
}

void MapInfo::MapGenerator() {
    
    for(int k = 0 ; k < 5 ; k++) {
        for(int i = 0 ; i < 15 ; i++) {
            for(int j = 13 ; j < 16; j++) {
                map[i][j][k] = 1;
            }
        }
    }

    for(int k = 0 ; k < 4 ; k++) {
        for(int i = 15 ; i < 30 ; i++) {
            for(int j = 13 ; j < 16; j++) {
                map[i][j][k] = 1;
            }
            for(int j = 0 ; j < 3; j++) {
                map[i][j][k] = 1;
            }
        }
    }


    for(int k = 0 ; k < 5 ; k++) {
        for(int i = 58 ; i < 60 ; i++) {
            for(int j = 0 ; j < 20; j++) {
                map[i][j][k] = 1;
            }
        }
        for(int i = 60 ; i < 62 ; i++) {
            for(int j = 20 ; j < 22; j++) {
                map[i][j][k] = 1;
            }
        }
        for(int i = 62 ; i < 64 ; i++) {
            for(int j = 20 ; j < 22; j++) {
                map[i][j][k] = 1;
            }
        }

        for(int i = 62 ; i < 64 ; i++) {
            for(int j = 22 ; j < 24; j++) {
                map[i][j][k] = 1;
            }
        }

        for(int i = 64 ; i < 66 ; i++) {
            for(int j = 22 ; j < 24; j++) {
                map[i][j][k] = 1;
            }
        }
        for(int i = 64 ; i < 66 ; i++) {
            for(int j = 24 ; j < 27; j++) {
                map[i][j][k] = 1;
            }
        }

    }

    for(int j = 25 ; j < 81; j++) {
        for(int i = 66 ; i < 73 ; i++) {
            map[i][j][0] = 1;
        }
    }

    for(int i = 90 ; i < 105 ; i++) {
        for(int j = 0 ; j < 15; j++) {
            map[i][j][0] = 1;
            map[i][j][1] = 1;
            map[i][j][2] = 1;
        }
    }

    for(int i = 105 ; i < 120 ; i++) {
        for(int j = 15 ; j < 35; j++) {
            map[i][j][0] = 1;
        }
    }

    for(int k = 0; k < 5 ; k++) {
        for(int i = 78+5 ; i < 91+5 ; i++) {
            for(int j = 58-4 ; j < 71-4; j++) {
                map[i][j][k] = 1;
            }
        }
    }


    for(int k = 0; k < 10 ; k++) {
        for(int i = 58 ; i < 71 ; i++) {
            for(int j = 68 ; j < 81; j++) {
                map[i][j][k] = 1;
            }
        }
        for(int i = 63 ; i < 65 ; i++) {
            for(int j = 71 ; j < 77; j++) {
                map[i][j][k] = 0;
            }
        }
    }

    for(int k = 0; k < 8 ; k++) {
        for(int i = 48 ; i < 61 ; i++) {
            for(int j = 88 ; j < 101; j++) {
                map[i][j][k] = 1;
            }
        }
        for(int i = 53 ; i < 55 ; i++) {
            for(int j = 90 ; j < 98; j++) {
                map[i][j][k] = 0;
            }
        }
    }

    for(int k = 0; k < 11 ; k++) {
        for(int i = 25 ; i < 38 ; i++) {
            for(int j = 93 ; j < 106; j++) {
                map[i][j][k] = 1;
            }
        }
    }
    

    // wall

    for(int k = 0 ; k < 7 ; k++) {
        for(int i = 108 ; i < 110 ; i++) {
            for(int j = 90 ; j < 128; j++) {
                map[i][j][k] = 1;
            }
        }
    }

    //stair

    for(int i = 110 ; i < 128 ; i++) {
        for(int j = 106 ; j < 108; j++) {
            map[i][j][0] = 1;
            map[i][j][1] = 1;
        }
        for(int j = 104 ; j < 106; j++) {
            map[i][j][0] = 1;
            map[i][j][1] = 1;
            map[i][j][2] = 1;
        }
        for(int j = 101 ; j < 104; j++) {
            map[i][j][0] = 1;
            map[i][j][1] = 1;
        }
        for(int j = 98 ; j < 101; j++) {
            map[i][j][0] = 1;
        }
    }


    
    // for(int k = 0 ; k < 3; k++) {
    //     for(int i = MapSize/10 ; i < MapSize/8 ; i++) {
    //         for(int len = 0; len<MapSize/10 ; len++) {
    //             map[i][len+MapSize/10][k] = 1;//y축 라벨링 len이 더 김
    //             map[len+MapSize/10][i][k] = 1;//x축 길게 라벨링
    //         }
    //     }
    // }

    // for(int k = 0 ; k < 2 ; k++) {
    //     for(int i = 0; i < MapSize/5 ; i++) {
    //         for(int len = 0; len < 3 ; len++) {
    //             map[i][len+MapSize/8+MapSize/10+1][k] = 1;
    //         }
    //     }
    // }

    // for(int k = 0 ; k < 8 ; k++) {
    //     for(int i = 0; i < MapSize/4 ; i++) {
    //         for(int len = 0; len < 3 ; len++) {
    //             map[len+MapSize/4][i][k] = 1;
    //         }
    //     }
    // }

    // for(int k = 0 ; k < 2 ; k++) {
    //     for(int i = 0 ; i < MapSize/1.8 ; i++) {
    //         int bais = MapSize/3*2;
    //         map[bais-i][i][k] = 1;
    //         map[bais-i+1][i][k] = 1;
    //         map[bais-i+2][i][k] = 1;
    //         map[bais-i+3][i][k] = 1;
    //     }
    // }
    // /////////////
    // for(int k = 0 ; k < 2 ; k++) {
    //     for(int i = MapSize/7*6 ; i < MapSize ; i++) {
    //         for(int j = MapSize/5*3+5 ; j<MapSize/5*4+5 ; j++) {
    //             map[i][j][k] = 1;
    //         }
    //     }
    // }
    // int desklen = MapSize/6;
    // for(int k = 0 ; k < 6 ; k++) {
    //     for(int i = MapSize/6*5 ; i < MapSize ; i++) {
    //         for(int j = MapSize/5*3-30 ; j<MapSize/5*4-32 ; j++) {
    //             map[i][j][k] = 1;
    //         }
    //     }
    // }

    // for(int k = 0 ; k < 3 ; k++) {
    //     for(int i = MapSize/6*5+desklen*0.3 ; i < MapSize-desklen*0.3 ; i++) {
    //         for(int j = MapSize/5*3-30 ; j<MapSize/5*4-32 ; j++) {
    //             map[i][j][k] = 0;
    //         }
    //     }
    // }


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


