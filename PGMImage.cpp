//
// Created by juan on 30/10/20.
//
#include <bits/stdc++.h>

#include "PGMImage.h"
#include <algorithm>

// tools to print data
ostream &operator<<(ostream &os, const vector<vector<unsigned char> > &M) {
    for (unsigned long i = 0; i < M.size(); i++) {
        for (auto x:M[i]) {
            os << setw(5) << (int)x << setw(5);
        }
        os << endl;
    }
    return os;
}

ostream &operator<<(ostream &os, const vector<unsigned char> &V) {
    for (auto x:V)
        os << x << " ";
    os << endl;
    return os;
}


PGMImage::PGMImage(string filename) {
    ifstream file(filename);
    stringstream caster;
    string line;

    // verificar que archivo se pueda abrir
    if (!file.good()) {
        cout << "ERROR OPENING FILE IMG: " << filename << endl;
        return;
    }

    //read header
    while (getline(file, line)) {
        if (line.at(0) == '#')                                  //ignore comment
            continue;

        if (!hasHeader) {
            if (line.at(0) != 'P' && line.at(1) != '2') {
                cout << "ERROR: NO HEADER " << endl;
                return;
            }
            hasHeader = true;
            continue;
        }
        if (nRows == -1 && nCols == -1) {
            stringstream ss(line);
            string splitLine;
            getline(ss, splitLine, ' ');
            caster << splitLine;
            caster >> nRows;

            caster.clear();
            getline(ss, splitLine, ' ');
            caster << splitLine;
            caster >> nCols;
            pixels.assign(nRows, vector<unsigned char>(nCols, 0));
            continue;
        } else if (max == -1) {
            caster.clear();
            caster << line;
            caster >> max;
        }
        break;
    }

    //read data
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            unsigned int number;
            getline(file, line);
            caster.clear();
            caster << line;
            caster >> number;
            pixels[i][j] = number;
        }
    }
    file.close();
}

void PGMImage::writeFile(string filename) {
    ofstream file(filename);
    file << "P2\n";
    file << "# IMG by Juan Luis" << endl;
    file << nRows << " " << nCols << endl;
    file << max << endl;

    //write data
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            file << (unsigned int) pixels[i][j] << endl;
        }
    }
    cout << "IMG writen successful" << endl;
    file.close();
}

void PGMImage::showData() {
    cout << "rows: " << nRows << endl;
    cout << "cols: " << nCols << endl;
    cout << "max: " << max << endl;
    cout << pixels << endl;
}

void PGMImage::findConvexSubset() {
    if (!hasHeader){
        cout << "ERROR: Img is corrupted" << endl;
        return;
    }

    findSet.assign(nRows, vector<unsigned char>(nCols, 0));
    queue<pair<int, int>> pointsQueue;
    int x, y, last;

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            if (pixels[i][j] > 0 && findSet[i][j] == 0) {
                pointsQueue.push(pair<int, int>(i, j));         //new point for search
                pointHead.push_back(pair<int, int>(i, j));         //add head point
                findSet[i][j] = pixels[i][j];                      //add point to findSet
                setLength.push_back(0);
            }
            last = setLength.size() - 1;
            while (!pointsQueue.empty()) {
                x = pointsQueue.front().first;              // (x,y) coord
                y = pointsQueue.front().second;
                setLength[last]++;                          //increment size of current set
                pointsQueue.pop();                          //remove front
                pushNeighbors(x, y, pointsQueue);       //push neighbors
            }
        }// for(j)
    }//for(i)
}

void PGMImage::printSubsets() {
    cout << "Number of Subsets: " << pointHead.size() <<endl;
    for (unsigned long i = 0; i<pointHead.size(); i++){
        cout << "Conjunto No. " << i << " size: " << setLength[i] << endl;
    }
}

/*
 * verify which neighbors are not yet in the found set
 * if not in there, add them and push them to the queue
 * */
void PGMImage::pushNeighbors(int x, int y, queue<pair<int, int>> &q) {

    if (x - 1 >= 0 && y - 1 >= 0 && pixels[x - 1][y - 1] > 0 && findSet[x - 1][y - 1] == 0) {
        q.push(pair<int, int>(x - 1, y - 1));
        findSet[x - 1][y - 1] = pixels[x - 1][y - 1];
    }
    if (x >= 0 && y - 1 >= 0 && pixels[x][y - 1] > 0 && findSet[x][y - 1] == 0) {
        q.push(pair<int, int>(x, y - 1));
        findSet[x][y - 1] = pixels[x][y - 1];
    }
    if (x + 1 < nRows && y - 1 >= 0 && pixels[x + 1][y - 1] > 0 && findSet[x + 1][y - 1] == 0) {
        q.push(pair<int, int>(x + 1, y - 1));
        findSet[x + 1][y - 1] = pixels[x + 1][y - 1];
    }
    if (x - 1 >= 0 && y >= 0 && pixels[x - 1][y] > 0 && findSet[x - 1][y] == 0) {
        q.push(pair<int, int>(x - 1, y));
        findSet[x - 1][y] = pixels[x - 1][y];
    }
    if (x + 1 < nRows && y >= 0 && pixels[x + 1][y] > 0 && findSet[x + 1][y] == 0) {
        q.push(pair<int, int>(x + 1, y));
        findSet[x + 1][y] = pixels[x + 1][y];
    }
    if (x - 1 >= 0 && y + 1 < nCols && pixels[x - 1][y + 1] > 0 && findSet[x - 1][y + 1] == 0) {
        q.push(pair<int, int>(x - 1, y + 1));
        findSet[x - 1][y + 1] = pixels[x - 1][y + 1];
    }
    if (x >= 0 && y + 1 < nCols && pixels[x][y + 1] > 0 && findSet[x][y + 1] == 0) {
        q.push(pair<int, int>(x, y + 1));
        findSet[x][y + 1] = pixels[x][y + 1];
    }
    if (x + 1 < nRows && y + 1 < nCols && pixels[x + 1][y + 1] > 0 && findSet[x + 1][y + 1] == 0) {
        q.push(pair<int, int>(x + 1, y + 1));
        findSet[x + 1][y + 1] = pixels[x + 1][y + 1];
    }
}

void PGMImage::paintLSubset(string filename) {
    if (!hasHeader){
        cout << "ERROR: Img corrupted " << endl;
        return;
    }

    int max = -1;
    int min = nRows * nCols;
    int i_max = 0, i_min = 0;

    for (unsigned long i = 0; i < setLength.size(); i++) {
        if (max < setLength[i]) {
            max = setLength[i];
            i_max = i;
        }
        if (min > setLength[i]) {
            min = setLength[i];
            i_min = i;
        }
    }
    // clean pixels to repaint
    pixels.assign(nRows, vector<unsigned char>(nCols, 0));

    queue<pair<int, int>> pointsQueue;
    int x, y;

    //paint largest
    x = pointHead[i_max].first;
    y = pointHead[i_max].second;
    pixels[x][y] = findSet[x][y];               //paint initial point from largest set
    pointsQueue.push(pair<int, int>(x, y));
    while (!pointsQueue.empty()) {
        x = pointsQueue.front().first;
        y = pointsQueue.front().second;
        pointsQueue.pop();                          //remove painted point
        paintNeighbors(x, y, pointsQueue);       //check his neighbors
    }

    //paint smallest
    x = pointHead[i_min].first;
    y = pointHead[i_min].second;
    pixels[x][y] = findSet[x][y];               //paint initial point from smalles set
    pointsQueue.push(pair<int, int>(x, y));
    while (!pointsQueue.empty()) {
        x = pointsQueue.front().first;
        y = pointsQueue.front().second;
        pointsQueue.pop();                          //remove painted point
        paintNeighbors(x, y, pointsQueue);       //check his neighbors
    }
    writeFile(filename);                            //write file
}

/*
 * verify which neighbors are not yet in the paint set
 * if not in there, add them and push them to the queue
 * */
void PGMImage::paintNeighbors(int x, int y, queue<pair<int, int>> &q) {
    if (x - 1 >= 0 && y - 1 >= 0 && findSet[x - 1][y - 1] > 0 && pixels[x - 1][y - 1] == 0) {
        q.push(pair<int, int>(x - 1, y - 1));
        pixels[x - 1][y - 1] = findSet[x - 1][y - 1];
    }
    if (x >= 0 && y - 1 >= 0 && findSet[x][y - 1] > 0 && pixels[x][y - 1] == 0) {
        q.push(pair<int, int>(x, y - 1));
        pixels[x][y - 1] = findSet[x][y - 1];
    }
    if (x + 1 < nRows && y - 1 >= 0 && findSet[x + 1][y - 1] > 0 && pixels[x + 1][y - 1] == 0) {
        q.push(pair<int, int>(x + 1, y - 1));
        pixels[x + 1][y - 1] = findSet[x + 1][y - 1];
    }
    if (x - 1 >= 0 && y >= 0 && findSet[x - 1][y] > 0 && pixels[x - 1][y] == 0) {
        q.push(pair<int, int>(x - 1, y));
        pixels[x - 1][y] = findSet[x - 1][y];
    }
    if (x + 1 < nRows && y >= 0 && findSet[x + 1][y] > 0 && pixels[x + 1][y] == 0) {
        q.push(pair<int, int>(x + 1, y));
        pixels[x + 1][y] = findSet[x + 1][y];
    }
    if (x - 1 >= 0 && y + 1 < nCols && findSet[x - 1][y + 1] > 0 && pixels[x - 1][y + 1] == 0) {
        q.push(pair<int, int>(x - 1, y + 1));
        pixels[x - 1][y + 1] = findSet[x - 1][y + 1];
    }
    if (x >= 0 && y + 1 < nCols && findSet[x][y + 1] > 0 && pixels[x][y + 1] == 0) {
        q.push(pair<int, int>(x, y + 1));
        pixels[x][y + 1] = findSet[x][y + 1];
    }
    if (x + 1 < nRows && y + 1 < nCols && findSet[x + 1][y + 1] > 0 && pixels[x + 1][y + 1] == 0) {
        q.push(pair<int, int>(x + 1, y + 1));
        pixels[x + 1][y + 1] = findSet[x + 1][y + 1];
    }
}