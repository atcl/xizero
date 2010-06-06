///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLLIGHT
#define HH_CLLIGHT
///*

///includes
#include "CLbase.hh"
#include "CLgfx.hh"
///*

///header
/* class name:	CLlight
 * 
 * description:	A class managing point light sources
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
class CLlight : public CLbase<CLlight,0>
{
	private:
		CLgfx& clgfx;
	protected:
		xlong radius;
		uxlong color;
		sprite* mask;
		inline uxlong lambertslaw(xlong x,xlong y) const;
	public:
		CLlight(xlong r,uxlong c);
		~CLlight() { delete mask; };
		void draw(xlong x,xlong y) const; 
};
///*

///implementation
uxlong CLlight::lambertslaw(xlong x,xlong y) const //! noncritical
{
	if(x==0 && y==0) { return color; }
	
	//precalc intensity square
	float i = float(radius*radius);
	//*
	
	//calc dist
	float d = (x*x) + (y*y);
	//*
	
	//split color components
	doubleword tempcolor = { color };
	float r = float(tempcolor.db[1]);
	float g = float(tempcolor.db[2]);
	float b = float(tempcolor.db[3]);
	//*
	
	//apply lamberts law on color components
	r = (r/(1-i) ) * ( (-i/d) + 1.0 ); //it is not d*d because the squareroot at init of d is omitted;
	if(r<0.0) { r = 0.0; }             //replace through multiplication with heaviside?
	g = (g/(1-i) ) * ( (-i/d) + 1.0 ); //it is not d*d because the squareroot at init of d is omitted;
	if(g<0.0) { g = 0.0; }             //replace through multiplication with heaviside?
	b = (b/(1-i) ) * ( (-i/d) + 1.0 ); //it is not d*d because the squareroot at init of d is omitted;
	if(b<0.0) { b = 0.0; }             //replace through multiplication with heaviside?
	//*
	
	//recombine
	tempcolor.db[1] = uxchar(r);
	tempcolor.db[2] = uxchar(g);
	tempcolor.db[3] = uxchar(b);
	//*
	
	return tempcolor.dd;
}

CLlight::CLlight(xlong r,uxlong c) //! noncritical
: clgfx(CLgfx::instance())
{
	radius = r;
	color = c;
	xlong length = (r<<1)+1;
	mask = new sprite;
	mask->height = mask->width = length;
	mask->size = length*length;
	mask->data = new uxlong[mask->size];
	
	uxlong temp = 0;
	xlong diff = r;
	for(xlong i=0; i<length; i++)
	{
		for(xlong j=0; j<length; j++) { mask->data[(i*mask->width)+j] = lambertslaw(j-diff,i-diff); }
	}
}

void CLlight::draw(xlong x,xlong y) const { clgfx.putsprite(x-radius,y-radius,mask,0,0); } //! noncritical
///*

 #endif
