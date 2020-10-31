#include <iostream>
#include "PGMImage.h"

int main() {
    PGMImage pgmImage("Figures/fig5.pgm");
    pgmImage.findConvexSubset();
    pgmImage.paintLSubset("Figures/fig5_ls.pgm");
    return 0;
}
