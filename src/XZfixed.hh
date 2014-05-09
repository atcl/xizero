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
constexpr yint FIXED = 16;

constexpr xint FXONE = 0x00010000; // 1
constexpr xint FXTWO = 0x00020000; // 2
constexpr xint FXMON = 0xFFFF0000; // -1
constexpr xint FXHLF = 0x00008000; // 0.5
constexpr xint FXQRT = 0x00004000; // 0.25
constexpr xint FXTNT = 0x00001999; // 0.1
constexpr xint FXCEN = 0x0000028F; // 0.01
constexpr xint FXEPS = 0x00000001; // eps
constexpr xint FXTAU = 0x0006487E; // tau = 2pi
constexpr xint FXTA2 = 0x0003243F; // tau/2
constexpr xint FXTA4 = 0x0001921F; // tau/4
constexpr xint FXITG = 0x00009B71; // 0.607200
constexpr xint FXIHG = 0x000134A3; // 1.205614
constexpr xint FXD2R = 0x00000477; // tau/360 (deg2rad)
constexpr xint FXSQ2 = 0x00016A09; // 2^0.5
constexpr xint FXRS1 = 0x00000126; // 0.0045
constexpr xint FXRS2 = 0x00012902; // 1.1602

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
	inline pure fixed mul(fixed x,fixed y) { const long long r = (long long)(x)*(long long)(y); return r>>FIXED; }
	inline pure fixed div(fixed x,fixed y) { const long long r = ((long long)(x)<<FIXED)/y; return r; }

	inline pure fixed round(fixed x)  { return (x + FXHLF)&FXMON; }
	inline pure fixed rsq(fixed x)    { return div(math::set(FXRS1,FXRS2,x<=FXONE),x); } //rough estimate!
	inline pure fixed sqr(fixed x)    { return math::sqr(x)<<(FIXED/2); }

	void cordic_t(fixed& x,fixed& y,fixed& z,fixed v);
	void cordic_h(fixed& x,fixed& y,fixed& z,fixed v);

	inline pure fixed ain(fixed x)    { const fixed y = math::abs(x); return x+math::neg(math::set(-FXTA2,math::set(-FXTAU,0,y>(FXTA2+FXTA4)),(y>FXTA4)&&(y<(FXTA2+FXTA4))),x<0); }
	inline pure fixed aout(fixed x,fixed y) { y = math::abs(y); return math::neg(x,(y>FXTA4)&&(y<(FXTA2+FXTA4))); }
	
	fixed sin(fixed a);		// |a| < 1.74
	fixed cos(fixed a);		// |a| < 1.74
	fixed tan(fixed a);		// |a| < 1.74
	fixed arcsin(fixed a);		// |a| < 0.98
	fixed arccos(fixed a);
	fixed arctan(fixed a);
	fixed sinc(fixed a);
	fixed sinh(fixed a);		// |a| < 1.13
	fixed cosh(fixed a);		// |a| < 1.13
	fixed tanh(fixed a);		// |a| < 1.13
	fixed artanh(fixed a);
	fixed arsinh(fixed a);
	fixed arcosh(fixed a);
	fixed exp(fixed a);
	fixed log(fixed a);		// 0.1 < a < 9.58
	fixed log(fixed a,fixed b);
	fixed pow(fixed a,fixed b);
	fixed hvs(fixed a);
}
///</define>

///<code>
void fx::cordic_t(fixed& x,fixed& y,fixed& z,fixed v)
{
	static const fixed at[FIXED] = { 0x0000C90F,   // 0.785398
					 0x000076B1,   // 0.463648
					 0x00003EB6,   // 0.244979
					 0x00001FD5,   // 0.124355
					 0x00000FFA,   // 0.0624188
					 0x000007FF,   // 0.0312398
					 0x000003FF,   // 0.0156237
					 0x000001FF,   // 0.00781234
					 0x000000FF,   // 0.00390623
					 0x0000007F,   // 0.00195312
					 0x0000003F,   // 0.000976562
					 0x0000001F,   // 0.000488281
					 0x00000010,   // 0.000244141
					 0x00000007,   // 0.00012207
					 0x00000004,   // 0.0000610352
					 0x00000002 }; // 0.0000305176

	fixed t = FXONE;
	register bool r = 0;

	for(yint i=0;i<FIXED;++i)
	{
		const bool s = (v>=0 && y<v) || (v<0 && z>=0);
		const fixed w = x + math::neg(mul(y,t),s);
		y -= math::neg(mul(x,t),s);
		z += math::neg(at[i],s);
		x = w;
		t >>= 1;
	}
}

void fx::cordic_h(fixed& x,fixed& y,fixed& z,fixed v)
{
	static const fixed ah[FIXED] = { 0x00008C9F,   // 0.549306
					 0x00004162,   // 0.255413
					 0x0000202B,   // 0.125657
					 0x00001005,   // 0.0625816
					 0x00000800,   // 0.0312602
					 0x00000400,   // 0.0156263
					 0x00000200,   // 0.00781266
					 0x00000100,   // 0.00390627
					 0x00000080,   // 0.00195313
					 0x00000040,   // 0.000976563
					 0x0000001F,   // 0.000488281
					 0x00000010,   // 0.000244141
					 0x00000007,   // 0.00012207
					 0x00000004,   // 0.0000610352
					 0x00000002,   // 0.0000305176
					 0x00000001 }; // 0.0000152588

	fixed t = 0;
	register bool r = 0;
	
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
}

fixed fx::sin(fixed a)
{
	a = a%FXTAU;
	fixed x = FXITG;
	fixed y = 0;
	fixed z = ain(a);
	cordic_t(x,y,z,FXMON);
	return aout(y,a);
}

fixed fx::cos(fixed a)
{
	a = a%FXTAU;
	fixed x = FXITG;
	fixed y = 0;
	fixed z = ain(a);
	cordic_t(x,y,z,FXMON);
	return aout(x,a);
}

fixed fx::tan(fixed a)
{
	a = a%FXTAU;
	fixed x = FXITG;
	fixed y = 0;
	fixed z = ain(a);
	cordic_t(x,y,z,FXMON);
	return aout(div(y,x),a);
}

fixed fx::arcsin(fixed a)
{
	fixed x = FXITG;
	fixed y = 0;
	fixed z = 0;
	cordic_t(x,y,z,math::abs(a));
	return math::neg(z,a<0);
}

fixed fx::arccos(fixed a)
{
	return FXTA4 - arcsin(a);
}

fixed fx::arctan(fixed a)
{
	fixed x = FXONE;
	fixed y = a;
	fixed z = 0;
	cordic_t(x,y,z,0);
	return z;
}

fixed fx::sinc(fixed a)
{
	return div(sin(a),a);
}

fixed fx::sinh(fixed a)
{
	fixed x = FXIHG;
	fixed y = 0;
	fixed z = a;
	cordic_h(x,y,z,FXMON);
	return y;
}

fixed fx::cosh(fixed a)
{
	fixed x = FXIHG;
	fixed y = 0;
	fixed z = a;
	cordic_h(x,y,z,FXMON);
	return x;
}

fixed fx::tanh(fixed a)
{
	fixed x = FXIHG;
	fixed y = 0;
	fixed z = a;
	cordic_h(x,y,z,FXMON);
	return div(y,x);
}

fixed fx::artanh(fixed a)
{
	fixed x = FXONE;
	fixed y = a;
	fixed z = 0;
	cordic_h(x,y,z,0);
	return z;
}

fixed fx::arsinh(fixed a)
{
	return artanh(div(a,sqr(mul(a,a)+FXONE)));
}

fixed fx::arcosh(fixed a)
{
	return artanh(div(sqr(mul(a,a)-FXONE),a));
}

fixed fx::exp(fixed a)
{
	fixed x = FXIHG;
	fixed y = 0;
	fixed z = a;
	cordic_h(x,y,z,FXMON);
	return x+y;
}

fixed fx::log(fixed a)
{
	fixed x = a+FXONE;
	fixed y = a-FXONE;
	fixed z = 0;
	cordic_h(x,y,z,0);
	return z/2;
	
}

fixed fx::log(fixed a,fixed b)
{
	return fx::div(log(a),log(b));
}

fixed fx::pow(fixed a,fixed b)
{
	return exp(mul(log(a),b));
}

fixed fx::hvs(fixed a)
{
	return (FXONE - cos(a))/2;
}
///</code>

