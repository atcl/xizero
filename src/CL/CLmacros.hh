//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMACROS
#define HH_CLMACROS
#pragma message "Compiling " __FILE__ " ! TODO: ..." 

#include <iostream>

#include "CLtypes.hh"
#include "CLconsts.hh"

xlong inline x20(xlong f)
{
	//fast multiplication with 20
	return ( (f<<4) + (f<<2) );
	//*
}

xlong inline x40(xlong f)
{
	//fast ultiplication with 40
	return ( (f<<5) + (f<<3) );
	//*
}

xlong inline x180(xlong f)
{
	//fast multiplication with 180
	return ( (f<<7) + (f<<5) + (f<<4) + (f<<2) );
	//*
}

xlong inline x360(xlong f)
{
	//fast multilication with 360
	return ( (f<<8) + (f<<6) + (f<<5) + (f<<3) );
	//*
}

xlong inline x600(xlong f)
{
	//fast multiplication with 600
	return ( (f<<9) + (f<<6) + (f<<4) + (f<<3) );
	//*
}

xlong inline x800(xlong f)
{
	//fast multiplication with 800
	return ( (f<<9) + (f<<8) + (f<<5) );
	//*
}

void inline tty(const xchar* c="\n")
{
	//console output without lineend
	std::cout << c;
	//*
}

void inline say(const xchar* c="hi")
{
	//console output with lineend
	std::cout << c << std::endl;
	//*
}

void inline say(xlong l)
{
	//console output with lineend
	std::cout << l << std::endl;
	//*
}

void inline say(uxlong l)
{
	//console output with lineend
	std::cout << l << std::endl;
	//*
}

void inline say(float f)
{
	//console output with lineend
	std::cout << f << std::endl;
	//*
}

void inline debug(const xchar* c,xlong v)
{
	//output name and integer to console
	std::cout << c << ": " << v << std::endl;
	//*
}

void inline bye()
{
	//force program exit
	CLsystem::exit(0,0,__func__,"bye");
	//*
}

void inline CLneg(xlong& x)
{
	//alternative integer negation
	 __asm__ __volatile__("notl %%eax, inc %%eax;":"=a"(x) :"a"(x):"%eax");
	//*
}

void inline CLstosd(xlong* dst,xlong val)
{
	//write val to dst
	 __asm__ __volatile__("stosd": :"a"(val),"D"(dst):"%eax","%edi");
	//*
}

void inline CLlodsd(xlong* src,xlong& var)
{
	//read value from src to val
	__asm__ __volatile__("lodsd":"=a"(var):"S"(src):"%eax","%esi");
	//*
}

#endif

