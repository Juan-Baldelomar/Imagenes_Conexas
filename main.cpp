#include <iostream>
#include "PGMImage.h"
#include <chrono>

using namespace std::chrono;

int main(int numArgs, char** args) {
    //leer imagen
    PGMImage pgmImage(args[1]);

    //clock start
    high_resolution_clock::time_point start = high_resolution_clock::now();

    //find convex subsets
    pgmImage.findConvexSubset();

    //calculo de tiempo
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> timeElapsed = duration_cast<duration<double>>(end-start);
    cout << "************************************************** TIEMPO DE BUSQUEDA : **************************************** \n" ;
    cout << timeElapsed.count() << " seconds " << endl;

    // imprimir conjuntos
    pgmImage.printSubsets();

    //generate output img
    if (numArgs==3)
        pgmImage.paintLSubset(args[2]);
    else
        pgmImage.paintLSubset("Figures/output.pgm");
    return 0;
}
