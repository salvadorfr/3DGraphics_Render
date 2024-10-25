#ifndef MAPPING_H
#define MAPPING_H

#include <math.h>

class Mapping
{
    float xWmin, yWmin, xWmax, yWmax;
    int   xPVmin, yPVmin, xPVmax, yPVmax;

public:
    // Mapping();

    void window ( float wx1, float wy1, float wx2, float wy2 );

    void port  ( int pvx1, int pvy1, int pvx2, int pvy2 );

    void map  ( int, int, int &, int &, int L, int M);
};

#endif // MAPPING_H
