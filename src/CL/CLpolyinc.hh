//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLPOLYINC
#define HH_CLPOLYINC
#pragma message "Compiling " __FILE__ " ! TODO: all"

xlong xmin = 1;
xlong xmax = xres - 1;
xlong ymin = 1;
xlong ymax = yres - 1;
xlong zmin = 1;
xlong zmax = zres - 1;

CLfvector ppoint[20];
CLfvector cpoint[20];
CLfvector dpoint[20];
CLfvector spoint[20];

xlong cleartrans = 0;
xlong maxcleartrans = 0;
xlong zdistance = 1000; //=zmax?
uxlong nolight = 0x00303030;
uxlong shadowcolor = 0x00C0C0C0;

screenside *leftside;
screenside *rightside;

#define prjx 80
#define prjy 95

#endif
