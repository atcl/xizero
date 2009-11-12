//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license  
#ifndef HH_CLGFX2
#define HH_CLGFX2
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLar.hh"
#include "CLbuffer.hh"
#include "CLformat.hh"
#include "CLmacros.hh"

/* class name:	CLgfx2
 * 
 * description:	gui 2d graphics routines
 * 
 * author:	atcl
 * 
 * notes:	improve font handling.
 * 
 * version: 0.1
 */

class CLgfx2 : public virtual CLcl, public CLsingle<CLgfx2> 
{
	friend class CLsingle<CLgfx2>;
	
	private:
		CLfont* tele;
		CLfont* mono;
		CLfont* line;
		CLfont* term;
		CLfont* segm;
		CLfont** fonts;
		CLgfx2();
		~CLgfx2() { };
	public:
		void loadfonts(CLfile* sf);
		void drawguirectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c1,uxlong c2,bool f) const;
		xlong drawfontchar(xlong x,xlong y,const xchar a,xlong f,uxlong fc,uxlong bc=0) const;
		void drawfontstring(xlong x,xlong y,const xchar* a,xlong f,uxlong fc,uxlong bc=0) const;
		xlong getfontstringwidth(const char* a,xlong f) const;
		xlong getfontstringheight(const char* a,xlong f) const;
		uxlong getEGAcolor(xchar c) const;
		bool comparecolors(uxlong c1,uxlong c2) const;
		uxlong getgradient(uxlong s,uxlong e,xchar i) const;
		sprite* savescreen() const;
};

CLgfx2::CLgfx2() { tele = mono = line = term = segm  = 0; };

void CLgfx2::loadfonts(CLfile* sf)
{
	fonts = new CLfont*[4];
	CLar* fontsa = new CLar(sf);
	//tele = clformat->loadtileset(fontsa->findbyname("CLteletype.fnt"));
	fonts[2] = line = clformat->loadtileset("dat/fonts/CLlinetype.im24",16,16); //temp
}

void CLgfx2::drawguirectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c1,uxlong c2,bool f) const
{
	//draw to top level of zbuffer!
	
	if(x1>x2) x1 ^= x2 ^= x1 ^= x2;
	if(y1>y2) y1 ^= y2 ^= y1 ^= y2;
	if(x1<0 || y1<0 || x2>XRES || y2>YRES || x2<0 || y2<0 || x1>XRES || y1>YRES) return;
	
	xlong offset1 = (y1*XRES)+x1;
	xlong offset2 = offset1;
	xlong doffset = ((y2-y1)*XRES);
	xlong diff    = x2-x1;
	uxlong c3     = c2;
	if(f) c3 = 0x000FFFFFF - c2;

	for(int i=y1; i<=y2; i++)
	{
		cldoublebuffer[offset1] = c2;
		offset1++;
		for(int j=x1+1; j<x2; j++)
		{
			cldoublebuffer[offset1] = c1;
			offset1++;
		}
		cldoublebuffer[offset1] = c3;
		offset1 += XRES - diff;
	}

	for(int k=x1; k<x2; k++)
	{
		cldoublebuffer[offset2] = c2;
		cldoublebuffer[offset2+doffset] = c3;
		offset2++;
	}
}

xlong CLgfx2::drawfontchar(xlong x,xlong y,const xchar a,xlong f,uxlong fc,uxlong bc) const
{
	//select font
	if(f>3) f = 0;
	CLfont* t = fonts[f];
	//*
	
	//xlong ssize = s->size;
	xlong swidth = t->tilewidth;
	xlong sheight = t->tileheight;
	xlong xs = x;
	xlong ys = y;
	xlong xe = x + swidth;
	xlong ye = y + sheight;
	//*

	//clipping against screen borders
	if(isoff(xs,ys,xe,ye)) return -1;
	clip(xs,ys);
	clip(xe,ye);
	//*

	//set up variables
	xlong xoffset = (ys * XRES) + xs;
	xlong linearc = 0;
	xlong rx = x;
	uxlong srcval = 0;
	//*

	//drawloop
	for(uxlong i=0; i<sheight ;i++)
	{
		for(uxlong j=0; j<swidth ;j++)
		{
			srcval = t->tiledata[a][linearc];
			if(srcval == 0x00FF0000) cldoublebuffer[xoffset+j] = fc;
			else if(bc!=0 && srcval == 0x00FFFFFF) cldoublebuffer[xoffset+j] = bc;
			if(i==0 && srcval != 0x00000000) rx++;
			linearc++;
		}
		xoffset += XRES;
	}
	//*
	
	return rx;
}

void CLgfx2::drawfontstring(xlong x,xlong y,const xchar* a,xlong f,uxlong fc,uxlong bc) const
{
	xlong l = clutils->chararraylength(a);
	xlong t = x;
	
	for(uxlong i=0; i<l; i++)
	{
		if(a[i]=='\n') { t = x; y += 16; }
		else
		{
			t = drawfontchar(t,y,a[i],f,fc,bc);
			if(t==-1) return;
		}
	}
}

xlong CLgfx2::getfontstringwidth(const char* a,xlong f) const
{
	//select font
	if(f>3) f = 0;
	CLfont* t = fonts[f];
	//*
	
	xlong l = clutils->chararraylength(a);
	xlong r = 0;
	
	uxlong srcoff = 0;
	
	for(uxlong i=0; i<l; i++)
	{
		for(uxlong j=0; j<t->tilewidth; j++)
		{
			if(t->tiledata[i][j] != 0x00000000) r++;
		}
	}
	
	return r;
}

xlong CLgfx2::getfontstringheight(const char* a,xlong f) const
{
	//select font
	if(f>3) f = 0;
	CLfont* t = fonts[f];
	//*
	
	xlong l = clutils->chararraylength(a);
	xlong r = t->tileheight;
	
	for(uxlong i=0; i<l; i++) { if(a[i]=='\n') r += t->tileheight; }
	
	return r;
}

uxlong CLgfx2::getEGAcolor(xchar c) const
{
	//return the 16 EGA colors
	switch(c)
	{
		case 0:  return 0x00000000; break; //black
		case 1:  return 0x000000FF; break; //blue
		case 2:  return 0x0000FF00; break; //green
		case 3:  return 0x0000FFFF; break; //cyan
		case 4:  return 0x00FF0000; break; //red
		case 5:  return 0x00FF00FF; break; //magenta
		case 6:  return 0x00FFFF00; break; //yellow
		case 7:  return 0x00808080; break; //dark white?
		case 8:  return 0x00404040; break; //gray
		case 9:  return 0x008080FF; break; //light blue
		case 10: return 0x0080FF80; break; //light green
		case 11: return 0x0080FFFF; break; //light cyan
		case 12: return 0x00FF8080; break; //light red
		case 13: return 0x00FF80FF; break; //light magenta
		case 14: return 0x00FFFF80; break; //light yellow
		case 15: return 0x00FFFFFF; break; //white
		default: return 0;
	}
	//*
}

bool CLgfx2::comparecolors(uxlong c1,uxlong c2) const
{
	return 0;
}

uxlong CLgfx2::getgradient(uxlong s,uxlong e,xchar i) const
{
	//((s.r-e.r)/255)*i, ((s.g - e.g)/255)*i, ((s.b - e.b)/255)*i
}

sprite* CLgfx2::savescreen() const
{
	sprite* r = new sprite;
	r->width = XRES;
	r->height = YRES;
	r->size = SCRS;
	r->data = new uxlong[r->size];
	
	for(uxlong i=0; i<r->size; i++) { r->data[i] = cldoublebuffer[i]; }
	
	return r;
}

#endif

