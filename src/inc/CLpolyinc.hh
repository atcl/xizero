//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPOLYINC
#define HH_CLPOLYINC
#pragma message "Compiling " __FILE__ " ! TODO: all"


fvertex ppoint[20];
fvertex cpoint[20];
fvertex dpoint[20];
fvertex spoint[20];

xlong cleartrans = 0;
xlong maxcleartrans = 0;
xlong zdistance = 1000; //=zmax?
uxlong nolight = 0x00303030;
uxlong shadowcolor = 0x00C0C0C0;

fvector plane = {0,0,1,1};

screenside *leftside;
screenside *rightside;

#endif
