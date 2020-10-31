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
            os << setw(12) << x << setw(12);
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
        if (line.at(0) == '#')
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
    cout << "IMG writen succesful" << endl;
    file.close();
}

void PGMImage::showData() {
    cout << "rows: " << nRows << endl;
    cout << "cols: " << nCols << endl;
    cout << "max: " << max << endl;
    cout << pixels << endl;
}

void PGMImage::findConvexSubset() {
    findSet.assign(nRows, vector<unsigned char>(nCols, 0));
    queue<pair<int, int>> pointsQueue;
    int x, y, last;

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            if (pixels[i][j] > 0 && findSet[i][j] == 0) {
                pointsQueue.push(pair<int, int>(i, j));         //new point for search
                pointHead.push_back(pair<int, int>(i, j));          //point head found
                setLength.push_back(0);
            }
            last = setLength.size() - 1;
            while (!pointsQueue.empty()) {
                x = pointsQueue.front().first;
                y = pointsQueue.front().second;
                setLength[last]++;
                pointsQueue.pop();
                pushNeighbors(x, y, pointsQueue);
                //cout << x << " " << y << endl;
            }
        }// for(j)
    }//for(i)
    cout << "Number: " << pointHead.size() <<endl;
}

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
    int max = -1;
    int min = nRows * nCols;
    int i_max = 0, i_min = 0;

    for (int i = 0; i < setLength.size(); i++) {
        if (max < setLength[i]) {
            max = setLength[i];
            i_max = i;
        }
        if (min > setLength[i]) {
            min = setLength[i];
            i_min = i;
        }
    }

    pixels.assign(nRows, vector<unsigned char>(nCols, 0));

    queue<pair<int, int>> pointsQueue;
    int x, y, last;

    //paint largest
    pointsQueue.push(pair<int, int>(pointHead[i_max].first, pointHead[i_max].second));
    while (!pointsQueue.empty()) {
        x = pointsQueue.front().first;
        y = pointsQueue.front().second;
        pointsQueue.pop();
        paintNeighbors(x, y, pointsQueue);
    }

    //paint smallest
    pointsQueue.push(pair<int, int>(pointHead[i_min].first, pointHead[i_min].second));
    while (!pointsQueue.empty()) {
        x = pointsQueue.front().first;
        y = pointsQueue.front().second;
        pointsQueue.pop();
        paintNeighbors(x, y, pointsQueue);
    }

    writeFile(filename);
}

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