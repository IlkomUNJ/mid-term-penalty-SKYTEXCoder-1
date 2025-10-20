#ifndef CUSTOMMATRIX_H
#define CUSTOMMATRIX_H

#include <iostream>
#include <vector>

using namespace std;

class CustomMatrix{
public:
    bool mat[3][3]={0};
    CustomMatrix(){

    }

    //alternate way to instantiate the class
    CustomMatrix(bool m[3][3]){
        const int m_size = sizeof(m) / sizeof(m[0]);
        for (int i=0;i< m_size;i++) {
            for(int j=0; j < sizeof(m[0]);j++){
                //cout << i << " " << j << endl;
                mat[i][j] = m[i][j];
            }
        }
    }

    void fillMatrix(bool m[3][3]){
        const int m_size = sizeof(m) / sizeof(m[0]);
        for (int i=0;i< m_size;i++) {
            for(int j=0; j < sizeof(m[0]);j++){
                //cout << i << " " << j << endl;
                mat[i][j] = m[i][j];
            }
        }
    }

    bool isNotEmpty() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (mat[i][j]) return true;
            }
        }
        return false;
    }

    int countTheAmountOfTruePixels() const {
        int count = 0;
        for (int i = 0; i< 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (mat[i][j] == true) count++;
            }
        }
        return count;
    }

    bool isTheCenterPixelTrue() const {
        if (mat[1][1] == true) {
            return true;
        } else {
            return false;
        }
    }

    int countTrueNeighbors() const {
        int count = 0;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (((i == 1 && j == 1) == false ) && mat[i][j] == true) {
                    count++;
                }
            }
        }
        return count;
    }

    bool areNeighboringPixelsAdjacent() const {
        vector<pair<int, int>> neighbors;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (!(i == 1 && j == 1) && mat[i][j]) {
                    neighbors.push_back({i, j});
                }
            }
        }
        if (neighbors.size() != 2) return false;
        int dx = abs(neighbors[0].first  - neighbors[1].first);
        int dy = abs(neighbors[0].second - neighbors[1].second);
        return (dx <= 1 && dy <= 1 && (dx + dy) == 1);
    }

    bool isWindowAnLShape() const {
        if (mat[0][1] && mat[1][0] && mat[1][1]) return true;
        if (mat[0][1] && mat[1][2] && mat[1][1]) return true;
        if (mat[2][1] && mat[1][0] && mat[1][1]) return true;
        if (mat[2][1] && mat[1][2] && mat[1][1]) return true;
        return false;
    }
};

#endif // CUSTOMMATRIX_H
