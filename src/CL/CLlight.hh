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
		inline uxlong lambertslaw(xlong x,xlong y);
	public:
		CLlight(xlong r,xlong i,uxlong c);
		~CLlight();
		void draw(xlong x,xlong y); 
};

uxlong CLlight::lambertslaw(xlong x,xlong y)
{
	//precalc intensity square
	float i = float(intensity*intensity);
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
	if(r<0.0) r = 0.0;                 //replace through multiplication with heaviside?
	g = (g/(1-i) ) * ( (-i/d) + 1.0 ); //it is not d*d because the squareroot at init of d is omitted;
	if(g<0.0) g = 0.0;                 //replace through multiplication with heaviside?
	b = (b/(1-i) ) * ( (-i/d) + 1.0 ); //it is not d*d because the squareroot at init of d is omitted;
	if(b<0.0) b = 0.0;                 //replace through multiplication with heaviside?
	//*
	
	//recombine
	tempcolor.db[1] = uxchar(r);
	tempcolor.db[2] = uxchar(g);
	tempcolor.db[3] = uxchar(b);
	//*
	
	return tempcolor.dd;
}

CLlight::CLlight(xlong r,xlong i,uxlong c)
{
	radius = r;
	intensity = i;
	color = c;
	xlong length = (r<<1)+1;
	mask = new sprite;
	mask->height = mask->width = length;
	mask->size = length*length;
	mask->data = new uxlong[mask->size];
	
	uxlong temp = 0;
	xlong diff = r;
	for(uxlong i=0; i<length; i++)
	{
		for(uxlong j=0; j<length; j++)
		{
			mask->data[(i*mask->width)+j] = lambertslaw(j-diff,i-diff);	
		}
	}
}

CLlight::~CLlight() { delete mask; }

void CLlight::draw(xlong x,xlong y) { clgfx1->putsprite(x,y,mask,0,0); }

 #endif
