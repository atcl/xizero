///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Tile Library ( XZtile.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
///</include>

///<define>
struct tile
{
	      xint* data;
	const yint  width;
	const yint  height;
	tile(yint w,yint h) : data(new xint[w*h]),width(w),height(h) { };
	~tile() { delete data; }

	tile(const tile& t);
	tile& operator=(const tile& t);
};
///</define>

