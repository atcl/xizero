// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZfixed.hh
// Fixed Point 16.16 Type Library 

///guard
#ifndef HH_XZFIXED
#define HH_XZFIXED
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZbasic.hh"
#include "XZmath.hh"
///*

///declaration
#define FX    16
#define FXONE 0x00010000 //1
#define FXMON 0xFFFF0000 //-1
#define FXHLF 0x00008000 //0.5
#define FXTWO 0x00020000 //2
#define FXTNT 0x00001999 //0.1
#define FXHUN 0x0000028F //0.01
#define FXRES 0x00000001 //eps
#define FXPI  0x0003243F //pi
#define FX2PI 0x0006487E //2*pi
#define FXPI2 0x0001921F //pi/2
#define FXITG 0x00009B71 //0.607200
#define FXIHG 0x000134A3 //1.205614
#define FXD2R 0x00000477 //pi/180 deg2rad
#define FXSQ2 0x00016A09 //2^0.5
#define FXRS1 0x00000126 //0.0045
#define FXRS2 0x00012902 //1.1602

typedef sint fixed;
///*

namespace fx
{
	inline sint  f2l(fixed x) { return x>>FX; } CONST
	inline sint  r2l(fixed x) { return (x+FXHLF)>>FX; } CONST
	inline fixed l2f(sint x)  { return x<<FX; } CONST
	inline fixed mul(fixed x,fixed y) { return ((long long)(x)*(long long)(y))>>FX; } CONST
	inline fixed div(fixed x,fixed y) { return (((long long)(x))<<FX)/y; } CONST

	void cordic(fixed& x,fixed& y,fixed& z,fixed v,bool h);

	inline fixed round(fixed x)       { return ((x + FXHLF)>>FX)<<FX; }
	inline fixed sqr(fixed x)         { return math::sqr(x)<<(FX>>1); }
	inline fixed rsq(fixed x)         { return div(math::set(FXRS1,FXRS2,x<=FXONE),x); } //rough estimate
	inline fixed sin(fixed a)         { fixed x = FXITG; fixed y = 0; fixed z = a; cordic(x,y,z,FXMON,0); return y; } // |a| < 1.74
	inline fixed cos(fixed a)         { fixed x = FXITG; fixed y = 0; fixed z = a; cordic(x,y,z,FXMON,0); return x; } // |a| < 1.74 
	inline fixed tan(fixed a)         { fixed x = FXITG; fixed y = 0; fixed z = a; cordic(x,y,z,FXMON,0); return div(y,x); } // |a| < 1.74 
	inline fixed arcsin(fixed a)      { fixed x = FXITG; fixed y = 0; fixed z = 0; cordic(x,y,z,math::abs(a),0); return math::neg(z,a<0); } // |a| < 0.98
	inline fixed arccos(fixed a)      { return FXPI2 - arcsin(a); }
	inline fixed arctan(fixed a)      { fixed x = FXONE; fixed y = a; fixed z = 0; cordic(x,y,z,0,0); return z; }
	inline fixed sinc(fixed a)        { return div(sin(a),a); }
	inline fixed sinh(fixed a)        { fixed x = FXIHG; fixed y = 0; fixed z = a; cordic(x,y,z,FXMON,1); return y; } // |a| < 1.13
	inline fixed cosh(fixed a)        { fixed x = FXIHG; fixed y = 0; fixed z = a; cordic(x,y,z,FXMON,1); return x; } // |a| < 1.13
	inline fixed tanh(fixed a)        { fixed x = FXIHG; fixed y = 0; fixed z = a; cordic(x,y,z,FXMON,1); return div(y,x); } // |a| < 1.13
	inline fixed artanh(fixed a)      { fixed x = FXONE; fixed y = a; fixed z = 0; cordic(x,y,z,0,1); return z; }
	inline fixed arsinh(fixed a)      { return artanh(div(a,sqr(mul(a,a)+FXONE))); }
	inline fixed arcosh(fixed a)      { return artanh(div(sqr(mul(a,a)-FXONE),a)); }
	inline fixed exp(fixed a)         { fixed x = FXIHG; fixed y = 0; fixed z = a; cordic(x,y,z,FXMON,1); return x+y; }
	inline fixed log(fixed a)         { fixed x = a+FXONE; fixed y = a-FXONE; fixed z = 0; cordic(x,y,z,0,1); return z<<1; } // 0.1 < a < 9.58
	inline fixed log(fixed a,fixed b) { return fx::div(log(a),log(b)); }
	inline fixed pow(fixed a,fixed b) { return exp(mul(log(a),b)); }
	inline fixed hvs(fixed a)         { return (FXONE - cos(a))>>1; }
}

///implementation
void fx::cordic(fixed& x,fixed& y,fixed& z,fixed v,bool h)
{
	//0.785398,0.463648,0.244979,0.124355,0.0624188,0.0312398,0.0156237,0.00781234,0.00390623,0.00195312,0.000976562,0.000488281,0.000244141,0.00012207,0.0000610352,0.0000305176 
	static const fixed at[16] = { 0x0000C90F,0x000076B1,0x00003EB6,0x00001FD5,0x00000FFA,0x000007FF,0x000003FF,0x000001FF,0x000000FF,0x0000007F,0x0000003F,0x0000001F,0x00000010,0x00000007,0x00000004,0x00000002 }; 

	//0.549306,0.255413,0.125657,0.0625816,0.0312602,0.0156263,0.00781266,0.00390627,0.00195313,0.000976563,0.000488281,0.000244141,0.00012207,0.0000610352,0.0000305176,0.0000152588
	static const fixed ah[16] = { 0x00008C9F,0x00004162,0x0000202B,0x00001005,0x00000800,0x00000400,0x00000200,0x00000100,0x00000080,0x00000040,0x0000001F,0x00000010,0x00000007,0x00000004,0x00000002,0x00000001 }; 

	fixed t = FXONE>>h;
	bool  r = 0;

	for(uint i=0;i<FX;++i)
	{
		r = (((i-1)%3)==0)&&i!=1&&!r&&h;
		const bool  s = (v>=0 && y<v) || (v<0 && z>=0);
		const fixed w = x + math::neg(mul(y,t),s^h);
		y -= math::neg(mul(x,t),s);	
		z += math::neg(math::set(ah[i],at[i],h),s);
		x = w;
		i -= r;
		t >>= !r;
	}
}
///*

#endif

