///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZtile.hh
// Tile Library 
///</header>

///<include>
#pragma once
#include "XZbasic.hh"
///</include>

///<define>
class tile
{
	public:
		const yint width;
		const yint height;
		xint* data;
		tile(yint w,yint h) : width(w),height(h),data(new xint[w*h]) { };
		~tile() { delete data; }
};
///</define>

