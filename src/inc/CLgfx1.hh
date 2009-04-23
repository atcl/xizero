//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLGFX1
#define HH_CLGFX1
#pragma message "Compiling " __FILE__ " ! TODO: all"


class CLgfx1
{
	private:
		static xlong version;

		static xlong last_p;
		static xlong interval_p;
		static xlong last_s;
		static xlong interval_s;

		xlong* doublebuffer;
		xlong locmaxx;
		xlong locmaxy;
		
	public:
		CLgfx1(xlong* db);
		~CLgfx1();
		uxlong readpixel(xlong x,xlong y);
		void drawpixel(xlong x,xlong y,uxlong c);
		void drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c);
		void copypixel(xlong x1,xlong y1,xlong x2,xlong y2);
		void drawbigpixel(xlong x,xlong y,uxlong c);
		void drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i);
		void drawhorline(xlong x1,xlong y1,xlong x2,uxlong c);
		void drawverline(xlong x1,xlong y1,xlong y2,uxlong c);
		void drawdialine(xlong x1,xlong y1,xlong xy,uxlong c);
		void drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		void drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		void drawarc(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		void drawarc(xlong xc,xlong yc,xlong r,uxlong l,uxlong c);
		void drawrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		void drawfilledrectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c);
		void drawpolygon(xlong x1,xlong y1,xlong x2,xlong y2,xlong x3,xlong y3,xlong x4,xlong y4,uxlong c);
		void drawcircle(xlong xc,xlong yc,xlong r,uxlong c);
		void drawanticircle(xlong xc,xlong yc,xlong r,uxlong c);
		void drawellipse(xlong xc,xlong yc,xlong r1,xlong r2,uxlong c);
		void fill(xlong x,xlong y,uxlong c);
		void fillframe(xlong x,xlong y,uxlong fc,uxlong c);
		xlong getspritesize(xlong* s);
		xlong getspriteheight(xlong* s);
		xlong getspritewidth(xlong* s);
		void drawsprite(xlong x,xlong y,xlong* s);
		void drawspritescaled(xlong x,xlong y,xlong* s,xlong z);
		void drawspriterotated(xlong x,xlong y,xlong* s,xlong d);
		void drawspriterotated90(xlong x,xlong y,xlong* s,xlong c);
		void drawspritemirrored(xlong x,xlong y,xlong* s,xlong b);
		void drawspriteanimated(xlong x,xlong y,xlong** s,xlong i);
		void drawscreen(xlong* s);
		void drawtile(xlong x,xlong y,xlong tx,xlong ty,xlong *s);
		bool comparecolors(uxlong c1,uxlong c2);
		xlong getversion();
};

xlong CLgfx1::version = 0x00010000;

CLgfx1::CLgfx1(xlong* db)
{
	doublebuffer = db;
}

CLgfx1::~CLgfx1() { }

uxlong CLgfx1::readpixel(xlong x,xlong y)
{
	return (doublebuffer[(y*xres)+y]);
}

void CLgfx1::drawpixel(xlong x,xlong y,uxlong c)
{
	doublebuffer[(y*xres)+x] = c;
}

void CLgfx1::drawpixeldirect(xlong* b,xlong x,xlong y,uxlong c)
{
	b[(y*xres)+x] = c;
}

void CLgfx1::copypixel(xlong x1,xlong y1,xlong x2,xlong y2)
{
	doublebuffer[(y1*xres)+x1] = doublebuffer[(y2*xres)+x2];
}

void CLgfx1::drawbigpixel(xlong x,xlong y,uxlong c)
{
	doublebuffer[(y*xres)+x] = c;
	doublebuffer[(y*xres)+x+1] = c;
	doublebuffer[((y+1)*xres)+x] = c;
	doublebuffer[((y+1)*xres)+(x+1)] = c;
}

void CLgfx1::drawblpixel(xlong x,xlong y,uxlong c1,uxlong c2,xlong i)
{

}

void CLgfx1::drawhorline(xlong x1,xlong y1,xlong x2,uxlong c)
{
	xlong offsetbase = (y1*xres);

	for(int i=x1; i<=x2; i++)
	{
		
		doublebuffer[offsetbase+i] = c;
	}
}

void CLgfx1::drawverline(xlong x1,xlong y1,xlong y2,uxlong c)
{
	xlong offsetbase = (y1*xres)+x1;

	for(int i=y1; i<=y2; i++)
	{
		doublebuffer[offsetbase] = c;
		offsetbase+=xres;
	}
}

void CLgfx1::drawdialine(xlong x1,xlong y1,xlong xy,uxlong c)
{
	xlong offsetbase = (y1*xres);

	for(int i=x1; i<=xy; i++)
	{
		doublebuffer[offsetbase+i] = c;
		offsetbase+=xres+1;
	}
}

void CLgfx1::drawanyline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{

}

void CLgfx1::drawantiline(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c)
{

}

xlong CLgfx1::getversion()
{
	return version;
}

#endif
