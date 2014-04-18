///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// C String Library ( XZstring.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZmath.hh"
///</include>

///<declare>
#define alert(x) system::say(string::int2str(x),1)
#define alerf(x) system::say(string::fix2str(x),1)
///</declare>

///<define>
namespace string
{
	pure yint   length(const char* x);
	pure yint   count(const char* x,char='\n');
	pure xint   find(const char* x,const char* y);
	pure xint   str2int(const char* x);
	pure xint   hex2int(const char* x);
	pure char   toup(char x);
	     char*  copy(const char* x,xint y=0,xint z=0x7FFFFFFF);
	     char*  concat(const char* x,const char* y); //unused!
	     char*  int2str(xint x);
	     char*  fix2str(fixed x,yint y=16);
	     char*  repl(const char* x,char y,char z);
	     char** split(const char* x,char y);
	     char*  trim(const char* x,char y=' ');
}
///</define>

///<code>
yint string::length(const char* x)
{
	register yint r = -1;
	do { ++r; } while(x[r]);
	return r;
}

yint string::count(const char* x,char y)
{
	register yint r = 0;
	for(yint i=0;x[i]!=0;++i) { r += (x[i]==y); }
	return r;
}

xint string::find(const char* x,const char* y)
{
	xint r = -1;
	for(xint i=0,j=0;(x[i]!=0)&&(y[j]!=0);++i)
	{
		r = math::set(i,r<0&&x[i]==y[0]) | math::set(r,r>=0&&x[i]==y[j]) | math::set(-1,(x[i]!=y[j])||(y[j+1]!=0&&x[i+1]==0));
		j = math::set(j+1,r>=0); 
	}
	return r;
}

xint string::str2int(const char* x)
{
	xint r = 0;
	xint i = 0;
	while(x[i]==' ') { ++i; }
	const xint j = math::set(i,-1,x[i]=='-');
	i += j!=-1;
	while( (x[i]>='0') && (x[i]<='9') )
	{
		r *= 10;
		r += (x[i]-'0');
		++i;
	}
	r = math::neg(r,j!=-1);
	return r;
}

xint string::hex2int(const char* x)
{
	xint r = 0;
	xint i = 0;
	while(x[i]==' ') { ++i; }
	for(xint j=0;j<8&&x[i]!=0;++j,++i)
	{
		r += (math::set((x[i]-'0')   ,x[i]>='0'&&x[i]<='9') 
		   |  math::set((x[i]-'a'+10),x[i]>='a'&&x[i]<='f') 
		   |  math::set((x[i]-'A'+10),x[i]>='A'&&x[i]<='F'))<<((7-i)<<2); 
	}
	return r;
}

char string::toup(char x)
{
	return math::set('A',math::set(x-32,x,x>=97),x<65); 
}

char* string::copy(const char* x,xint y,xint z)
{
	const yint l = math::min(length(x),z);
	char* r = new char[l+1];
	for(yint i=0;i<l;++i) { r[i] = x[y++]; } 
	r[l] = 0;
	return r;
}

char* string::concat(const char* x,const char* y)
{
	const yint a = length(x);
	const yint b = length(y);
	char* r = new char[a+b+1];

	yint i = 0;
	yint j = 0;
	for(;i<a;++i) { r[i] = x[i]; }
	for(;j<b;++i) { r[i] = y[j++]; }
	r[++i] = 0;
	return r;
}

char* string::int2str(xint x)
{
	char* r = new char[16];
	xint  i = (x<0);
	x       = math::abs(x);
	r[0]    = math::set('-','0',i);
	i      += (x==0);
	for(xint d=1000000000,k=0;d>0;d/=10,i+=k)
	{
		const xint j = x/d;
		k = (k+j>0);
		x -= d*j;
		r[i] = j+'0';
	}
	r[i] = 0;
	return r;
}

char* string::fix2str(fixed x,yint y)
{
	char* r = new char[16];
	xint  i = xint(x<0);
	x       = math::abs(x);
	r[0]    = math::set('-','0',i);
	xint  l = x>>y;
	i      += (l==0);
	for(xint d=10000,k=0;d>0;d/=10,i+=k)
	{
		const xint j = l/d;
		k = (k+j>0);
		l -= d*j;
		r[i] = j+'0';
	}
	r[i++] = '.';
	l = x & 0x0000FFFF;
	for(xint d=10;d<10000000;d*=10,++i)
	{
		const xint j = ((l*d)>>y)%10;
		r[i] = math::set(j,j>0) +'0';
	}
	r[i] = 0;
	return r;
}

char* string::repl(const char* x,char y,char z)
{
	const yint l = length(x);
	char* r = new char[l+1];
	for(yint i=0;i<l;++i)
	{
		r[i] = math::set(z,x[i],x[i]==y);
	}
	r[l] = 0;
	return r;
}

char** string::split(const char* x,char y)
{
	const yint c = count(x,y)+1;
	char*  s = repl(x,y,0);
	char** r = new char*[c];
	r[0] = &s[0];
	yint i = 0;
	yint j = 1;
	while(x[i]!=0)
	{
		if(s[i]==0) { r[j] = &s[i+1]; }
		j += s[i++]==0;
	}
	return r;
}

char* string::trim(const char* x,char y)
{
	const yint l = length(x);
	const yint c = count(x,y);
	char* r = new char[l-c+1];
	yint j = 0;
	for(yint i=0;x[i]!=0;++i)
	{
		const bool k = x[i]!=y;
		r[j] = math::set(x[i],r[j],k);
		j+=k;
	}
	r[j] = 0;
	return r;
}
///</code>

