//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license  
#ifndef HH_CLGFX2
#define HH_CLGFX2
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
#include "CLbuffer.hh"
#include "CLformat.hh"

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

#define TELEFONT 0;
#define MONOFONT 1;
#define LINEFONT 2;
#define TERMFONT 3;
#define SEGMFONT 4;

class CLgfx2 : public virtual CLcl, public CLsingle<CLgfx2> 
{
	friend class CLsingle<CLgfx2>;
	
	private:
		CLfont* tele;
		CLfont* mono;
		CLfont* line;
		CLfont* term;
		CLfont* segm;
		CLgfx2();
		~CLgfx2() { };
	public:
		void drawguirectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c1,uxlong c2,bool f);
		xlong drawfontchar(xlong x,xlong y,const xchar a,xlong ft,uxlong fc,uxlong bc=0);
		void drawfontstring(xlong x,xlong y,const xchar* a,xlong ft,uxlong fc,uxlong bc=0);
		xlong getfontstringwidth(const char* a,xlong ft);
		xlong getfontstringheight(const char* a,xlong ft);
		uxlong getEGAcolor(xchar c);
		bool comparecolors(uxlong c1,uxlong c2);
		uxlong blendcolors(xlong mode,uxlong c1,uxlong c2=0xFF000000);
		uxlong getgradient(uxlong s,uxlong e,xchar i);
		sprite* savescreen();
};

CLgfx2::CLgfx2()
{
	CLfile* temp = clsystem->getfile("../dat/other/CLteletype.fnt");
	tele = clformat->loadfont(temp);
};

void CLgfx2::drawguirectangle(xlong x1,xlong y1,xlong x2,xlong y2,uxlong c1,uxlong c2,bool f)
{
	//draw to top level of zbuffer!
	
	if(x1>x2) x1 ^= x2 ^= x1 ^= x2;
	if(y1>y2) y1 ^= y2 ^= y1 ^= y2;
	if(x1<0 || y1<0 || x2>xres || y2>yres || x2<0 || y2<0 || x1>xres || y1>yres) return;
	
	xlong offset1 = (y1*xres)+x1;
	xlong offset2 = offset1;
	xlong doffset = ((y2-y1)*xres);
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
		offset1 += xres - diff;
	}

	for(int k=x1; k<x2; k++)
	{
		cldoublebuffer[offset2] = c2;
		cldoublebuffer[offset2+doffset] = c3;
		offset2++;
	}
}

xlong CLgfx2::drawfontchar(xlong x,xlong y,const xchar a,xlong ft,uxlong fc,uxlong bc)
{
	//is on screen
	if( x<0 || y<0 || x>xres || y>yres) return -1;
	//*
	
	//select font
	CLfont* f = 0;
	switch(ft)
	{
		case 0: f = tele; break;
		case 1: f = mono; break;
		case 2: f = line; break;
		case 3: f = term; break;
		case 4: f = segm; break;
	}
	//*
	
	//find (font) tile
	uxlong srcoff = (a * f->tilewidth)-1; 
	//*

	//draw vars
	xlong hordiff = (f->width) - (f->tilewidth);
	xlong drawwidth = f->tilewidth;
	xlong drawheight = f->tileheight;
	uxlong dstoff = (y * xres) + x;
	xlong rx = x;
	//*
	
	//is completely on screen
	if(x+drawwidth>xres || y+drawheight>yres) return -1;
	//*

	//drawloop
	uxlong curroff = dstoff;
	for(uxlong i=0; i<drawheight; i++)
	{
		for(uxlong j=0; j<drawwidth; j++)
		{
			if(f->data[srcoff] == 0xFFFF0000) cldoublebuffer[curroff] = fc;
			if(bc!=0 && f->data[srcoff] == 0xFFFFFFFF) cldoublebuffer[curroff] = bc;
			if(f->data[srcoff] != 0xFF000000)
			{
				if(i==0) rx++;
				curroff++;
			}
			srcoff++; 
		}
		curroff = dstoff += xres;
		srcoff += hordiff;
	}
	//*
	
	return rx;
}

void CLgfx2::drawfontstring(xlong x,xlong y,const xchar* a,xlong ft,uxlong fc,uxlong bc)
{
	xlong l = clutils->chararraylength(a);
	xlong t = x;
	
	for(uxlong i=0; i<l; i++)
	{
		if(a[i]=='\n') { t = x; y += 16; }
		else t = drawfontchar(t,y,a[i],ft,fc,bc);
	}
}

xlong CLgfx2::getfontstringwidth(const char* a,xlong ft)
{
	//select font
	CLfont* f = 0;
	switch(ft)
	{
		case 0: f = tele; break;
		case 1: f = mono; break;
		case 2: f = line; break;
		case 3: f = term; break;
		case 4: f = segm; break;
	}
	//*
	
	xlong l = clutils->chararraylength(a);
	xlong r = 0;
	
	uxlong srcoff = 0;
	
	for(uxlong i=0; i<l; i++)
	{
		srcoff = (a[i] * f->tilewidth)-1; 

		for(uxlong j=0; j<f->tilewidth; j++)
		{
			if(f->data[srcoff] != 0xFF000000) r++;
			srcoff++; 
		}
	}
	
	return r;
}

xlong CLgfx2::getfontstringheight(const char* a,xlong ft)
{
	//select font
	CLfont* f = 0;
	switch(ft)
	{
		case 0: f = tele; break;
		case 1: f = mono; break;
		case 2: f = line; break;
		case 3: f = term; break;
		case 4: f = segm; break;
	}
	//*
	
	xlong l = clutils->chararraylength(a);
	xlong r = f->tileheight;
	
	for(uxlong i=0; i<l; i++) { if(a[i]=='\n') r += f->tileheight; }
	
	return r;
}

uxlong CLgfx2::getEGAcolor(xchar c)
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

bool CLgfx2::comparecolors(uxlong c1,uxlong c2)
{
	return 0;
}

uxlong CLgfx2::blendcolors(xlong mode,uxlong c1,uxlong c2)
{
	xchar a1 = 0;
	xchar a2 = 0;
	xchar a3 = 0;
	xchar a4 = 0;
	xchar b1 = 0;
	xchar b2 = 0;
	xchar b3 = 0;
	xchar b4 = 0;
	
	switch(mode)
	{
		case 0: //not
			return (!c1);
		break;
		
		case 1: //add
			a1 = xchar(c1);
			a2 = xchar(c1>>8);
			a3 = xchar(c1>>16);
			a4 = xchar(c1>>24);
			b1 = xchar(c2);
			b2 = xchar(c2>>8);
			b3 = xchar(c2>>16);
			b4 = xchar(c2>>24);
			return ( (xlong(a4+b4)<<24) + (xlong(a3+b3)<<16) + (xlong(a2+b2)<<8) + xlong(a1+b1) );
		break;
		
		case 2: //sub
			a1 = xchar(c1);
			a2 = xchar(c1>>8);
			a3 = xchar(c1>>16);
			a4 = xchar(c1>>24);
			b1 = xchar(c2);
			b2 = xchar(c2>>8);
			b3 = xchar(c2>>16);
			b4 = xchar(c2>>24);
			return ( (xlong(a4-b4)<<24) + (xlong(a3-b3)<<16) + (xlong(a2-b2)<<8) + xlong(a1-b1) );
		break;
		
		case 3: //mul
			a1 = xchar(c1);
			a2 = xchar(c1>>8);
			a3 = xchar(c1>>16);
			a4 = xchar(c1>>24);
			b1 = xchar(c2);
			b2 = xchar(c2>>8);
			b3 = xchar(c2>>16);
			b4 = xchar(c2>>24);
			return ( (xlong(a4*b4)<<24) + (xlong(a3*b3)<<16) + (xlong(a2*b2)<<8) + xlong(a1*b1) );
		break;
		
		case 4: //alpha
		
		break;
		
		case 5: //and
			return (c1 && c2);
		break;
		
		case 6: //or
			return (c1 || c2);
		break;
		
		case 7: //xor
			return (c1 ^ c2);
		break;
		
		case 8: //nand
			return !(c1 && c2);
		break;
		
		case 9: //nor
			return !(c1 || c2);
		break;
	}
	
	return 0;
}

uxlong CLgfx2::getgradient(uxlong s,uxlong e,xchar i)
{
	//((s.r-e.r)/255)*i, ((s.g - e.g)/255)*i, ((s.b - e.b)/255)*i
}

sprite* CLgfx2::savescreen()
{
	sprite* r = new sprite;
	r->width = xres;
	r->height = yres;
	r->size = (xres*yres);
	r->data = new uxlong(r->size);
	
	for(uxlong i=0; i<r->size; i++)
	{
		r->data[i] = cldoublebuffer[i];
	}
	
	return r;
}

#endif

