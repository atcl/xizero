// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZmatrix.hh
// Matrix Class Library

///guard
#ifndef HH_XZMATRIX
#define HH_XZMATRIX
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZvector.hh"
///* 

///definition
class fmatrix
{
	private:
		fixed _m[4][4];
		/*OK*/ void multiplicate(const fixed (&n)[4][4]);
	public:
		/*OK*/ fmatrix(fixed x=FXONE,fixed y=0);
		/*OK*/        void clear(fixed x=FXONE,fixed y=0);
		/*OK*/ inline void rotatex(fixed x);
		/*OK*/ inline void rotatey(fixed y);
		/*OK*/ inline void rotatez(fixed z);
		/*OK*/ inline void translate(fixed x,fixed y,fixed z);
		/*OK*/ inline void scale(fixed x,fixed y,fixed z);
		/*OK*/ inline fixed trace();
		              void project(fixed x,fixed y,fixed z,fixed w);
		              void transpose();
		/*OK*/        void dyadic(const fvector& x,const fvector& y);
		/*OK*/ inline void shadow(const fvector& x,const fvector& y);
		/*OK*/ inline fvector transform(const fvector& x) const;
		/*OK*/ inline lvector transform(const lvector& x) const;
};
///*

///implementation
void fmatrix::multiplicate(const fixed (&n)[4][4])
{
	const fixed a[4] = {
	fx::mul(_m[0][0],n[0][0])+fx::mul(_m[0][1],n[1][0])+fx::mul(_m[0][2],n[2][0])+fx::mul(_m[0][3],n[3][0]),
	fx::mul(_m[0][0],n[0][1])+fx::mul(_m[0][1],n[1][1])+fx::mul(_m[0][2],n[2][1])+fx::mul(_m[0][3],n[3][1]),
	fx::mul(_m[0][0],n[0][2])+fx::mul(_m[0][1],n[1][2])+fx::mul(_m[0][2],n[2][2])+fx::mul(_m[0][3],n[3][2]),
	fx::mul(_m[0][0],n[0][3])+fx::mul(_m[0][1],n[1][3])+fx::mul(_m[0][2],n[2][3])+fx::mul(_m[0][3],n[3][3])};
	_m[0][0] = a[0]; 
	_m[0][1] = a[1];
	_m[0][2] = a[2];
	_m[0][3] = a[3];
	const fixed b[4] = {
	fx::mul(_m[1][0],n[0][0])+fx::mul(_m[1][1],n[1][0])+fx::mul(_m[1][2],n[2][0])+fx::mul(_m[1][3],n[3][0]),
	fx::mul(_m[1][0],n[0][1])+fx::mul(_m[1][1],n[1][1])+fx::mul(_m[1][2],n[2][1])+fx::mul(_m[1][3],n[3][1]),
	fx::mul(_m[1][0],n[0][2])+fx::mul(_m[1][1],n[1][2])+fx::mul(_m[1][2],n[2][2])+fx::mul(_m[1][3],n[3][2]),
	fx::mul(_m[1][0],n[0][3])+fx::mul(_m[1][1],n[1][3])+fx::mul(_m[1][2],n[2][3])+fx::mul(_m[1][3],n[3][3])};
	_m[1][0] = b[0];
	_m[1][1] = b[1];
	_m[1][2] = b[2];
	_m[1][3] = b[3];
	const fixed c[4] = {
	fx::mul(_m[2][0],n[0][0])+fx::mul(_m[2][1],n[1][0])+fx::mul(_m[2][2],n[2][0])+fx::mul(_m[2][3],n[3][0]),
	fx::mul(_m[2][0],n[0][1])+fx::mul(_m[2][1],n[1][1])+fx::mul(_m[2][2],n[2][1])+fx::mul(_m[2][3],n[3][1]),
	fx::mul(_m[2][0],n[0][2])+fx::mul(_m[2][1],n[1][2])+fx::mul(_m[2][2],n[2][2])+fx::mul(_m[2][3],n[3][2]),
	fx::mul(_m[2][0],n[0][3])+fx::mul(_m[2][1],n[1][3])+fx::mul(_m[2][2],n[2][3])+fx::mul(_m[2][3],n[3][3])};
	_m[2][0] = c[0];
	_m[2][1] = c[1];
	_m[2][2] = c[2];
	_m[2][3] = c[3];
	//const fixed d[4] = {
	//fx::mul(_m[3][0],n[0][0])+fx::mul(_m[3][1],n[1][0])+fx::mul(_m[3][2],n[2][0])+fx::mul(_m[3][3],n[3][0]),
	//fx::mul(_m[3][0],n[0][1])+fx::mul(_m[3][1],n[1][1])+fx::mul(_m[3][2],n[2][1])+fx::mul(_m[3][3],n[3][1]),
	//fx::mul(_m[3][0],n[0][2])+fx::mul(_m[3][1],n[1][2])+fx::mul(_m[3][2],n[2][2])+fx::mul(_m[3][3],n[3][2]),
	//fx::mul(_m[3][0],n[0][3])+fx::mul(_m[3][1],n[1][3])+fx::mul(_m[3][2],n[2][3])+fx::mul(_m[3][3],n[3][3])};
	//_m[3][0] = d[0];
	//_m[3][1] = d[1];
	//_m[3][2] = d[2];
	//_m[3][3] = d[3];
}

fmatrix::fmatrix(fixed x,fixed y)
{
	clear(x,y);
}

void fmatrix::clear(fixed x,fixed y)
{
	_m[0][0] = _m[1][1] = _m[2][2] = _m[3][3] = x; 
	_m[0][1] = _m[0][2] = _m[0][3] = _m[1][0] = _m[1][2] = _m[1][3] = _m[2][0] = _m[2][1] = _m[2][3] = _m[3][0] = _m[3][1] = _m[3][2] = y; 
}

void fmatrix::rotatex(fixed x)
{
	x = fx::mul(x,FXD2R);
	const fixed s = fx::sin(x);
	const fixed c = fx::cos(x);
	multiplicate({{FXONE,0,0,0},{0,c,-s,0},{0,s,c,0},{0,0,0,FXONE}});
}
	
void fmatrix::rotatey(fixed y)
{
	y = fx::mul(y,FXD2R);
	const fixed s = fx::sin(y);
	const fixed c = fx::cos(y);
	multiplicate({{c,0,s,0},{0,FXONE,0,0},{-s,0,c,0},{0,0,0,FXONE}});
}

void fmatrix::rotatez(fixed z)
{
	z = fx::mul(z,FXD2R);
	const fixed s = fx::sin(z);
	const fixed c = fx::cos(z);
	multiplicate({{c,-s,0,0},{s,c,0,0},{0,0,FXONE,0},{0,0,0,FXONE}});
}

void fmatrix::translate(fixed x,fixed y,fixed z)
{
	multiplicate({{FXONE,0,0,x},{0,FXONE,0,y},{0,0,FXONE,z},{0,0,0,FXONE}});
}

void fmatrix::scale(fixed x,fixed y,fixed z)
{
	multiplicate({{x,0,0,0},{0,y,0,0},{0,0,z,0},{0,0,0,FXONE}});
}

fixed fmatrix::trace()
{
	return _m[0][0]+_m[1][1]+_m[2][2]+_m[3][3];
}

void fmatrix::transpose()
{
	const fixed a = _m[0][1];
	_m[0][1] = _m[1][0];
	_m[1][0] = a;
	const fixed b = _m[0][2];
	_m[0][2] = _m[2][0];
	_m[2][0] = b;
	const fixed c = _m[1][2];
	_m[1][2] = _m[2][1];
	_m[2][1] = c;
}

void fmatrix::project(fixed x,fixed y,fixed z,fixed w) //check! 
{
	_m[0][0] = fx::div(z,x)<<1; _m[0][1] = 0;               _m[0][2] = 0;                _m[0][3] = 0; 
	_m[1][0] = 0;               _m[1][1] = fx::div(z,y)<<1; _m[1][2] = 0;                _m[1][3] = 0; 
	_m[2][0] = 0;               _m[2][1] = 0;               _m[2][2] = fx::div(w,(w-z)); _m[2][3] = fx::div(fx::mul(-w,z),(w-z)); 
	_m[3][0] = 0;               _m[3][1] = 0;               _m[3][2] = FXONE;            _m[3][3] = 0; 
}

void fmatrix::dyadic(const fvector& x,const fvector& y)
{
	_m[0][0] = fx::mul(x.x,y.x); _m[0][1] = fx::mul(x.x,y.y); _m[0][2] = fx::mul(x.x,y.z); _m[0][3] = 0; 
	_m[1][0] = fx::mul(x.y,y.x); _m[1][1] = fx::mul(x.y,y.y); _m[1][2] = fx::mul(x.y,y.z); _m[1][3] = 0; 
	_m[2][0] = fx::mul(x.z,y.x); _m[2][1] = fx::mul(x.z,y.y); _m[2][2] = fx::mul(x.z,y.z); _m[2][3] = 0; 
	_m[3][0] = 0;                _m[3][1] = 0;                _m[3][2] = 0;                _m[3][3] = FXONE; 
}

void fmatrix::shadow(const fvector& x,const fvector& y)
{
	dyadic(-x,y);
	const fixed xy = x.dot(y);
	_m[0][0] += xy;
	_m[1][1] += xy;
	_m[2][2] += xy;
}

fvector fmatrix::transform(const fvector& x) const
{
	return fvector(fx::mul(_m[0][0],x.x)+fx::mul(_m[0][1],x.y)+fx::mul(_m[0][2],x.z)+_m[0][3],
		       fx::mul(_m[1][0],x.x)+fx::mul(_m[1][1],x.y)+fx::mul(_m[1][2],x.z)+_m[1][3],
		       fx::mul(_m[2][0],x.x)+fx::mul(_m[2][1],x.y)+fx::mul(_m[2][2],x.z)+_m[2][3],x.e);
}

lvector fmatrix::transform(const lvector& x) const
{
	return lvector(fx::r2l(fx::mul(_m[0][0],fx::l2f(x.x))+fx::mul(_m[0][1],fx::l2f(x.y))+fx::mul(_m[0][2],fx::l2f(x.z))+_m[0][3]),
		       fx::r2l(fx::mul(_m[1][0],fx::l2f(x.x))+fx::mul(_m[1][1],fx::l2f(x.y))+fx::mul(_m[1][2],fx::l2f(x.z))+_m[1][3]),
		       fx::r2l(fx::mul(_m[2][0],fx::l2f(x.x))+fx::mul(_m[2][1],fx::l2f(x.y))+fx::mul(_m[2][2],fx::l2f(x.z))+_m[2][3]),x.e);
}
///*

#endif

