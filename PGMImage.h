//
// Created by juan on 30/10/20.
//

#ifndef PA1_T11_PGMIMAGE_H
#define PA1_T11_PGMIMAGE_H
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>

using namespace std;
class PGMImage {
private:
    bool hasHeader = false;
    int nRows = -1;
    int nCols = -1;
    int max = -1;
    vector<vector<unsigned char>> pixels;
    vector<vector<unsigned char>> findSet;
    vector<pair<int, int>> pointHead;
    vector<int> setLength;
    void pushNeighbors(int x, int y, queue<pair<int, int>>&q);
public:
    PGMImage(string filename);
    void writeFile(string filename);
    void showData();
    void findConvexSubset();
    void paintLSubset(string filename);
    void paintNeighbors(int x, int y, queue<pair<int, int>> &q);
    void printSubsets();

};

ostream &operator<<(ostream &os, const vector< vector<unsigned char> > &M);
ostream &operator<<(ostream &os, const vector<unsigned char> &V);

#endif //PA1_T11_PGMIMAGE_H
