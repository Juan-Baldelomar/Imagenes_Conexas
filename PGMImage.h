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
    vector<vector<unsigned char>> transpose;

    // encontrar conjuntos conexos
    vector<vector<unsigned char>> findSet;
    vector<pair<int, int>> pointHead;
    vector<int> setLength;
    void pushNeighbors(int x, int y, queue<pair<int, int>>&q);
    void paintNeighbors(int x, int y, queue<pair<int, int>> &q);

    // encontrar envolvente convexa
    vector<pair<int, int>> hullPoints;
    void findInitialHullPoint(pair<int, int>&hullPoint, vector<pair<int, int>>&points);
    void drawLine(pair<int, int> &p1, pair<int, int> &p2);
    short onLeft(pair<int, int>&point, pair<int, int> &hullPoint, pair<int, int> &endPoint);


public:
    PGMImage(string filename);
    void writeFile(string filename);
    void showData();

    // encontrar conjuntos conexos
    void findConnectedSubset();
    void paintLSubset(string filename);

    void printSubsets();

    // encontrar envolvente convexa
    void findConvexHull();
    void printConvexPoints();
    void drawConvexHull();
    void printHullPoints();


};

ostream &operator<<(ostream &os, const vector< vector<unsigned char> > &M);
ostream &operator<<(ostream &os, const vector<unsigned char> &V);

#endif //PA1_T11_PGMIMAGE_H
