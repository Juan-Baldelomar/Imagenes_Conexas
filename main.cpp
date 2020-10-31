#include <iostream>
#include "PGMImage.h"
#include <chrono>

using namespace std::chrono;

int main(int numArgs, char** args) {
    high_resolution_clock::time_point start = high_resolution_clock::now();

    //find convex subsets
    PGMImage pgmImage(args[1]);
    pgmImage.findConvexSubset();

    //generate output img
    if (numArgs==3)
        pgmImage.paintLSubset(args[2]);
    else
        pgmImage.paintLSubset("Figures/output.pgm");

    //calculo de tiempo
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> timeElapsed = duration_cast<duration<double>>(end-start);
    cout << "TIEMPO EJECUCION :" << timeElapsed.count() << endl;
    return 0;
}
