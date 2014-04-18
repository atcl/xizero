///<header>
// Ξ0 - xizero ( Version 0.2 )
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Fixed Point 16.16 Type Library ( XZfixed.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
#include "XZmath.hh"
///</include>

///<declare>
#define FIXED 16
#define FXONE 0x00010000 // 1
#define FXTWO 0x00020000 // 2
#define FXMON 0xFFFF0000 // -1
#define FXHLF 0x00008000 // 0.5
#define FXQRT 0x00004000 // 0.25
#define FXTNT 0x00001999 // 0.1
#define FXCEN 0x0000028F // 0.01
#define FXEPS 0x00000001 // eps
#define FXTAU 0x0006487E // tau = 2pi
#define FXTA2 0x0003243F // tau/2
#define FXTA4 0x0001921F // tau/4
#define FXITG 0x00009B71 // 0.607200
#define FXIHG 0x000134A3 // 1.205614
#define FXD2R 0x00000477 // tau/360 (deg2rad)
#define FXSQ2 0x00016A09 // 2^0.5
#define FXRS1 0x00000126 // 0.0045
#define FXRS2 0x00012902 // 1.1602

#define HSEL16(x) ((x==4)||(x==7)||(x==10)||(x==13))	// (((i-1)%3)==0)&&(i!=1)
///</declare>

///<define>
typedef xint fixed;

constexpr fixed FX(xint x) { return x<<FIXED; }

namespace fx 
{
	inline pure xint  f2l(fixed x) { return x>>FIXED; }
	inline pure xint  r2l(fixed x) { return (x+FXHLF)>>FIXED; }
	inline pure fixed l2f(xint x)  { return x<<FIXED; }
	inline pure fixed mul(fixed x,fixed y) { return ((long long)(x)*(long long)(y))>>FIXED; }
	inline pure fixed div(fixed x,fixed y) { return (((long long)(x))<<FIXED)/y; }

	inline pure fixed round(fixed x)  { return (x + FXHLF)&FXMON; }
	inline pure fixed rsq(fixed x)    { return div(math::set(FXRS1,FXRS2,x<=FXONE),x); } //rough estimate!
	inline pure fixed sqr(fixed x)    { return math::sqr(x)<<(FIXED>>1); }

	inline pure fixed ain(fixed x)    { fixed y = math::abs(x); return x+math::neg(math::set(-FXTA2,math::set(-FXTAU,0,y>(FXTA2+FXTA4)),(y>FXTA4)&&(y<(FXTA2+FXTA4))),x<0); }
	inline pure fixed aout(fixed x,fixed y) { y = math::abs(y); return math::neg(x,(y>FXTA4)&&(y<(FXTA2+FXTA4))); }

	       void cordic(fixed& x,fixed& y,fixed& z,fixed v,bool h);

	inline fixed sin(fixed a)         { a = a%FXTAU; fixed x = FXITG; fixed y = 0; fixed z = ain(a); cordic(x,y,z,FXMON,0); return aout(y,a); } // |a| < 1.74
	inline fixed cos(fixed a)         { a = a%FXTAU; fixed x = FXITG; fixed y = 0; fixed z = ain(a); cordic(x,y,z,FXMON,0); return aout(x,a); } // |a| < 1.74 
	inline fixed tan(fixed a)         { a = a%FXTAU; fixed x = FXITG; fixed y = 0; fixed z = ain(a); cordic(x,y,z,FXMON,0); return aout(div(y,x),a); } // |a| < 1.74 
	inline fixed arcsin(fixed a)      { fixed x = FXITG; fixed y = 0; fixed z = 0; cordic(x,y,z,math::abs(a),0); return math::neg(z,a<0); } // |a| < 0.98
	inline fixed arccos(fixed a)      { return FXTA4 - arcsin(a); }
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
///</define>

///<code>
void fx::cordic(fixed& x,fixed& y,fixed& z,fixed v,bool h)
{
	//0.785398,0.463648,0.244979,0.124355,0.0624188,0.0312398,0.0156237,0.00781234,0.00390623,0.00195312,0.000976562,0.000488281,0.000244141,0.00012207,0.0000610352,0.0000305176 
	static const fixed at[FIXED] = { 0x0000C90F,0x000076B1,0x00003EB6,0x00001FD5,0x00000FFA,0x000007FF,0x000003FF,0x000001FF,0x000000FF,0x0000007F,0x0000003F,0x0000001F,0x00000010,0x00000007,0x00000004,0x00000002 }; 

	//0.549306,0.255413,0.125657,0.0625816,0.0312602,0.0156263,0.00781266,0.00390627,0.00195313,0.000976563,0.000488281,0.000244141,0.00012207,0.0000610352,0.0000305176,0.0000152588
	static const fixed ah[FIXED] = { 0x00008C9F,0x00004162,0x0000202B,0x00001005,0x00000800,0x00000400,0x00000200,0x00000100,0x00000080,0x00000040,0x0000001F,0x00000010,0x00000007,0x00000004,0x00000002,0x00000001 }; 

	fixed t = FXONE>>h;
	register bool r = 0;

	switch(h)
	{
		case false: // trigonometric
			for(yint i=0;i<FIXED;++i)
			{
				const bool s = (v>=0 && y<v) || (v<0 && z>=0);
				const fixed w = x + math::neg(mul(y,t),s);
				y -= math::neg(mul(x,t),s);
				z += math::neg(at[i],s);
				x = w;
				t >>= 1;
			}
			break;

		case true: // hyperbolic
			for(yint i=0;i<FIXED;++i)
			{
				r = HSEL16(i)&&(!r);
				const bool s = (v>=0 && y<0) || (v<0 && z>=0);
				const fixed w = x - math::neg(mul(y,t),s);
				y -= math::neg(mul(x,t),s);
				z += math::neg(ah[i],s);
				x = w;
				i -= r;
				t >>= 1;
			}
			break;
	}
}
///</code>

