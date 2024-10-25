#include "mapping.h"

//Mapping::Mapping() {}

void Mapping::window(float wx1, float wy1, float wx2, float wy2)
{
    xWmin = wx1;
    yWmin = wy1;
    xWmax = wx2;
    yWmax = wy2;
}

void Mapping::port(int pvx1, int pvy1, int pvx2, int pvy2)
{
    xPVmin = pvx1;
    yPVmin = pvy1;
    xPVmax = pvx2;
    yPVmax = pvy2;
}

void Mapping::map(int xW, int yW, int &xP, int &yP, int L, int M)
{
    float sX, sY;

    sX = ( xPVmax - xPVmin ) / ( xWmax - xWmin );
    sY = ( yPVmax - yPVmin ) / ( yWmax - yWmin );

    xP = int(round ( sX * ( xW - xWmin ) + xPVmin + L ));
    yP = (int) round ( sY * ( yWmin - yW ) - yPVmin + M );
}
