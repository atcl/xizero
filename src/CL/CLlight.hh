//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLLIGHT
#define HH_CLLIGHT
#pragma message "Compiling " __FILE__ " ! TODO: "

/* class name:	CLlight
 * 
 * description:	A class managing point light sources
 * 
 * author:	atcl
 * 
 * notes:	implement
 * 
 * version: 0.1
 */

class CLlight : public virtual CLcl
{
	private:
		xlong radius;
		xlong intensity;
		uxlong color;
		sprite* mask;
		inline uxlong computepixel(xlong x,xlong y);
	public:
		CLlight(xlong r,xlong i,uxlong c);
		~CLlight();
		void draw(xlong x,xlong y); 
};

uxlong CLlight::computepixel(xlong x,xlong y)
{
	//precalc intensity square
	float i = float(intensity*intensity);
	//*
	
	//calc dist
	float d = (x*x) + (y*y);
	//*
	
	//split color
	doubleword tempcolor = { color };
	float r = float(tempcolor.db[1]);
	float g = float(tempcolor.db[2]);
	float b = float(tempcolor.db[3]);
	//*
	
	//recombine
	tempcolor.db[1] = uxchar( (r/(1-i) ) * ( (-i/d) + 1.0 ) ); //it is not d*d because the squareroot at init of d is omitted;
	tempcolor.db[2] = uxchar( (g/(1-i) ) * ( (-i/d) + 1.0 ) ); //it is not d*d because the squareroot at init of d is omitted;
	tempcolor.db[3] = uxchar( (b/(1-i) ) * ( (-i/d) + 1.0 ) ); //it is not d*d because the squareroot at init of d is omitted;
	//*
	
	return tempcolor.dd;
}

CLlight::CLlight(xlong r,xlong i,uxlong c)
{
	radius = r;
	intensity = i;
	color = c;
	mask = new sprite;
	mask->size = ((r<<1)+1)*((r<<1)+1);
	mask->width = (r<<1)+1;
	mask->height = (r<<1)+1;
	mask->data = new uxlong[mask->size];
	
	uxlong ver = mask->height;
	uxlong hor = mask->width;
	for(uxlong i=0; i<hor; i++) { for(uxlong j=0; j<ver; j++) { mask->data[i*hor+j] = computepixel(j,i); } }
}

CLlight::~CLlight() { delete mask; }

void CLlight::draw(xlong x,xlong y) { clgfx1->putsprite(x,y,mask,0); }

 #endif
