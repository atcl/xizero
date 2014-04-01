///<header>
// atCROSSLEVEL 2010-2014
// released under 2-clause BSD license
// XZtile.hh
// Tile Library
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
///</include>

///<define>
class tile
{
	public:
		xint* data;
		const yint width;
		const yint height;
		tile(yint w,yint h) : data(new xint[w*h]),width(w),height(h) { ; };
		~tile() { delete data; }
};
///</define>

