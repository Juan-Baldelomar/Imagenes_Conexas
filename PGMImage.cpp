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

int crossProduct(pair<int, int> a, pair<int, int> b, pair<int, int> c) {            //finds the place of c from ab vector
    int y1 = a.second - b.second;
    int y2 = a.second - c.second;
    int x1 = a.first - b.first;
    int x2 = a.first - c.first;
    return y2*x1 - y1*x2;          //if result < 0, c in the left, > 0, c in the right, = 0, a,b,c are collinear
}

int distance(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    int y1 = a.second - b.second;
    int y2 = a.second - c.second;
    int x1 = a.first - b.first;
    int x2 = a.first - c.first;

    int item1 = (y1*y1 + x1*x1);
    int item2 = (y2*y2 + x2*x2);

    if(item1 == item2)
        return 0;               // b and c are in same distance from a
    else if(item1 < item2)
        return -1;              // b is closer to a

    return 1;                   // c is closer to a
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
            transpose.assign(nCols, vector<unsigned char>(nRows, 0));
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

void PGMImage::findConnectedSubset() {
    if (!hasHeader){
        cout << "ERROR: Img is corrupted" << endl;
        return;
    }

    findSet.assign(nRows, vector<unsigned char>(nCols, 0));
    queue<pair<int, int>> pointsQueue;
    int x, y, last;

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            if (pixels[i][j] > 127 && findSet[i][j] == 0) {
                pointsQueue.push(pair<int, int>(i, j));         //new point for search
                pointHead.push_back(pair<int, int>(i, j));         //add head point
                findSet[i][j] = 255;                      //add point to findSet
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

void PGMImage::printHullPoints() {
    cout << "Number of Points: " << hullPoints.size() <<endl;
    for (unsigned long i = 0; i<hullPoints.size(); i++){
        cout << "Punto No. " << i << setw(5) << "(" << hullPoints[i].first << ", "  << hullPoints[i].second <<  ")" << endl;
    }
}

/*
 * verify which neighbors are not yet in the found set
 * if not in there, add them and push them to the queue
 * */
void PGMImage::pushNeighbors(int x, int y, queue<pair<int, int>> &q) {

    if (x - 1 >= 0 && y - 1 >= 0 && pixels[x - 1][y - 1] > 127 && findSet[x - 1][y - 1] == 0) {
        q.push(pair<int, int>(x - 1, y - 1));
        findSet[x - 1][y - 1] = 255;
    }
    if (x >= 0 && y - 1 >= 0 && pixels[x][y - 1] > 127 && findSet[x][y - 1] == 0) {
        q.push(pair<int, int>(x, y - 1));
        findSet[x][y - 1] = 255;
    }
    if (x + 1 < nRows && y - 1 >= 0 && pixels[x + 1][y - 1] > 127 && findSet[x + 1][y - 1] == 0) {
        q.push(pair<int, int>(x + 1, y - 1));
        findSet[x + 1][y - 1] = 255;
    }
    if (x - 1 >= 0 && y >= 0 && pixels[x - 1][y] > 127 && findSet[x - 1][y] == 0) {
        q.push(pair<int, int>(x - 1, y));
        findSet[x - 1][y] = 255;
    }
    if (x + 1 < nRows && y >= 0 && pixels[x + 1][y] > 127 && findSet[x + 1][y] == 0) {
        q.push(pair<int, int>(x + 1, y));
        findSet[x + 1][y] = 255;
    }
    if (x - 1 >= 0 && y + 1 < nCols && pixels[x - 1][y + 1] > 127 && findSet[x - 1][y + 1] == 0) {
        q.push(pair<int, int>(x - 1, y + 1));
        findSet[x - 1][y + 1] = 255;
    }
    if (x >= 0 && y + 1 < nCols && pixels[x][y + 1] > 127 && findSet[x][y + 1] == 0) {
        q.push(pair<int, int>(x, y + 1));
        findSet[x][y + 1] = 255;
    }
    if (x + 1 < nRows && y + 1 < nCols && pixels[x + 1][y + 1] > 127 && findSet[x + 1][y + 1] == 0) {
        q.push(pair<int, int>(x + 1, y + 1));
        findSet[x + 1][y + 1] = 255;
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


// Encontrar envolvente Convexa

void PGMImage::findConvexHull() {
    pair<int, int> current, headPoint, endPoint;
    vector<pair<int, int>>points, collinearPoints;

    findInitialHullPoint(current, points);
    headPoint = current;
    hullPoints.push_back(headPoint);
    while(true){
        endPoint = points[0];
        int s = points.size();
        for (int i = 1; i<s; i++){
            pair<int, int> point = points[i];
            if (point == current)
                continue;
            int val = onLeft(point, current, endPoint);
            if (val > 0){
                endPoint = point;
                collinearPoints.clear();
            }else if(val == 0){
                if (distance(current, endPoint, point) < 0){
                    collinearPoints.push_back(endPoint);
                    endPoint = point;
                }else{
                    collinearPoints.push_back(point);
                }
            }
        }
        for (auto p:collinearPoints){
            hullPoints.push_back(p);
        }
        collinearPoints.clear();
        cout << "point i: " << endPoint.first << " point j: " << endPoint.second << endl;
        if (endPoint == headPoint)
            break;

        hullPoints.push_back(endPoint);
        current = endPoint;
    }
}

void PGMImage::drawConvexHull() {
    int s = hullPoints.size();
    int i = 0;
    while(i%s != 0 || i<s){
        drawLine(hullPoints[i%s], hullPoints[(i+1)%s]);
        i++;
    }

    /*int n = pixels.size();
    int m = pixels[0].size();
    for (int i = 0; i<n; i++){
        for (int j = 0; j<m; j++){
            pixels[i][j] = transpose[j][i];
        }
    }*/
}

void PGMImage::drawLine(pair<int, int> &p1, pair<int, int> &p2) {
    int i_max = p1.first > p2.first? p1.first : p2.first;
    int j_max = p1.second > p2.second? p1.second : p2.second;
    int i_min = p1.first < p2.first? p1.first : p2.first;
    int j_min = p1.second < p2.second? p1.second : p2.second;

    int step_x = p1.first < p2.first? 1 : -1;
    int step_y = p1.second < p2.second? 1: -1;

    int delta_x = abs(p1.first - p2.first);
    int delta_y = abs(p1.second - p2.second);

    int sign = 0, rate = 0;

    if (delta_x > delta_y){
        sign = 1;
        rate = round((delta_x + 1) / (double)(delta_y + 1));
    }else if (delta_x < delta_y){
        sign = -1;
        rate = round((delta_y + 1)/ (double)(delta_x + 1));
    }
    int i_start = p1.first, j_start = p1.second;
    int rate_step = 0;

    if (sign == 1){
        while(i_start != p2.first){
            pixels[i_start][j_start] = 255;
            if (rate_step == rate && j_start>= j_min && j_start<= j_max){
                j_start += step_y;
                rate_step = 0;

                if (j_start<j_min || j_start> j_max)
                    j_start += -step_y;
                continue;
            }
            i_start += step_x;
            rate_step++;
        }
        pixels[i_start][j_start] = 255;
    }else if (sign == -1){
        while(j_start != p2.second){
            pixels[i_start][j_start] = 255;
            if (rate_step == rate && i_start>=i_min && i_start<= i_max){
                i_start += step_x;
                rate_step = 0;

                if (i_start<i_min || i_start> i_max)
                    i_start += -step_x;
                continue;
            }
            j_start += step_y;
            rate_step++;
        }
        pixels[i_start][j_start] = 255;
    }else{
        while (i_start != p2.first){
            pixels[i_start][j_start] = 255;
            i_start += step_x;
            j_start += step_y;
        }
    }
}

void PGMImage::findInitialHullPoint(pair<int, int> &hullPoint, vector<pair<int, int>>&points) {
    int n = pixels.size();
    int m = pixels[0].size();

   /* for (int i = 0; i<n; i++){
        for (int j = 0; j<m; j++){
            transpose[j][i] = pixels[i][j];
        }
    }*/

    for (int  i = 0; i<m; i++){
        for (int j = 0; j<n; j++){
            if (pixels[i][j] > 127)
                points.push_back(pair<int, int>(i, j));
        }
    }
    hullPoint = points[0];
    for (auto p:points){
        if (p.first < hullPoint.first)
            hullPoint = p;
    }
}

short PGMImage::onLeft(pair<int, int>&point, pair<int, int> &hullPoint, pair<int, int> &endPoint) {
    int x = point.first, y = point.second;
    double det = (endPoint.first - hullPoint.first)*(y - hullPoint.second)
               - (endPoint.second-hullPoint.second)*(x - hullPoint.first);
    short sign;
        if (det == 0)
            sign = 0;           // colinear
        else if (det < 0)
            sign = -1;          // on right
        else if (det > 0)
            sign = 1;           // on left
    return sign;
}

/*
bool operator==(const pair<int, int> &p1, const pair<int, int> &p2){
    return p1.first == p2.first && p1.second == p2.second;
}*/
