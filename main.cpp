#include <iostream>
#include "PGMImage.h"
#include <queue>
#include <map>

void mapTry(){
    queue<pair<int,int>> cola;

    cola.push(pair<int, int>(1,2));
    cola.push(pair<int, int>(1,3));
    cola.push(pair<int, int>(1,4));
    cola.pop();

    map<int, map<int, int>> m;


    cout << "h" ;
}

int main() {
    PGMImage pgmImage("Figures/fig5.pgm");
    pgmImage.findConvexSubset();
    pgmImage.paintLSubset("Figures/fig5_ls.pgm");
    return 0;
}
