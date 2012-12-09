///<header>
// atCROSSLEVEL 2010,2011,2012
// released under 2-clause BSD license
// XZmatrix.hh
// Matrix Class Library
#pragma once
///</header>

///<include>
#include "XZvector.hh"
///</include>

///<define>
class fmatrix
{
	private:
		fixed align m[3][4];
		/*OK*/ void multiplicate(const fixed (&n)[3][4]);
	public:
		/*OK*/ fmatrix(fixed x=FXONE,fixed y=0);
		/*OK*/        void clear(fixed x=FXONE,fixed y=0);
		/*OK*/ inline void rotatex(fixed x);
		/*OK*/ inline void rotatey(fixed y);
		/*OK*/ inline void rotatez(fixed z);
		/*OK*/ inline void translate(fixed x,fixed y,fixed z);
		/*OK*/ inline void scale(fixed x,fixed y,fixed z);
		              void project(fixed x,fixed y,fixed z,fixed w);
		/*OK*/        void transpose();
		/*OK*/        void dyadic(const fvector& x,const fvector& y);
		/*OK*/ inline void shadow(const fvector& x,const fvector& y);
		/*OK*/ inline fixed trace() const;
		/*OK*/ inline fvector operator*(const fvector& x) const;
		/*OK*/ inline lvector operator*(const lvector& x) const;
};
///</define>

///<code>
void fmatrix::multiplicate(const fixed (&n)[3][4])
{
	const fixed align a[3][4] = { { fx::mul(m[0][0],n[0][0])+fx::mul(m[0][1],n[1][0])+fx::mul(m[0][2],n[2][0]),
				        fx::mul(m[0][0],n[0][1])+fx::mul(m[0][1],n[1][1])+fx::mul(m[0][2],n[2][1]),
				        fx::mul(m[0][0],n[0][2])+fx::mul(m[0][1],n[1][2])+fx::mul(m[0][2],n[2][2]),
				        fx::mul(m[0][0],n[0][3])+fx::mul(m[0][1],n[1][3])+fx::mul(m[0][2],n[2][3])+m[0][3] }, 
				      { fx::mul(m[1][0],n[0][0])+fx::mul(m[1][1],n[1][0])+fx::mul(m[1][2],n[2][0]),
				        fx::mul(m[1][0],n[0][1])+fx::mul(m[1][1],n[1][1])+fx::mul(m[1][2],n[2][1]),
				        fx::mul(m[1][0],n[0][2])+fx::mul(m[1][1],n[1][2])+fx::mul(m[1][2],n[2][2]),
				        fx::mul(m[1][0],n[0][3])+fx::mul(m[1][1],n[1][3])+fx::mul(m[1][2],n[2][3])+m[1][3] },
				      { fx::mul(m[2][0],n[0][0])+fx::mul(m[2][1],n[1][0])+fx::mul(m[2][2],n[2][0]),
				        fx::mul(m[2][0],n[0][1])+fx::mul(m[2][1],n[1][1])+fx::mul(m[2][2],n[2][1]),
				        fx::mul(m[2][0],n[0][2])+fx::mul(m[2][1],n[1][2])+fx::mul(m[2][2],n[2][2]),
				        fx::mul(m[2][0],n[0][3])+fx::mul(m[2][1],n[1][3])+fx::mul(m[2][2],n[2][3])+m[2][3] } }; 
	//m = std::move(n); n= {};
	//memcpy(&n,&m,48);
	m[0][0]=a[0][0]; m[0][1]=a[0][1]; m[0][2]=a[0][2]; m[0][3]=a[0][3];
	m[1][0]=a[1][0]; m[1][1]=a[1][1]; m[1][2]=a[1][2]; m[1][3]=a[1][3];
	m[2][0]=a[2][0]; m[2][1]=a[2][1]; m[2][2]=a[2][2]; m[2][3]=a[2][3];
}

fmatrix::fmatrix(fixed x,fixed y)
{
	clear(x,y);
}

void fmatrix::clear(fixed x,fixed y)
{
	m[0][0] = m[1][1] = m[2][2] = x; 
	m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] = y; 
}

void fmatrix::rotatex(fixed x)
{
	x = fx::mul(x,FXD2R);
	const fixed s = fx::sin(x);
	const fixed c = fx::cos(x);
	multiplicate({{FXONE,0,0,0},{0,c,-s,0},{0,s,c,0}});
}
	
void fmatrix::rotatey(fixed y)
{
	y = fx::mul(y,FXD2R);
	const fixed s = fx::sin(y);
	const fixed c = fx::cos(y);
	multiplicate({{c,0,s,0},{0,FXONE,0,0},{-s,0,c,0}});
}

void fmatrix::rotatez(fixed z)
{
	z = fx::mul(z,FXD2R);
	const fixed s = fx::sin(z);
	const fixed c = fx::cos(z);
	multiplicate({{c,-s,0,0},{s,c,0,0},{0,0,FXONE,0}});
}

void fmatrix::translate(fixed x,fixed y,fixed z)
{
	multiplicate({{FXONE,0,0,x},{0,FXONE,0,y},{0,0,FXONE,z}});
}

void fmatrix::scale(fixed x,fixed y,fixed z)
{
	multiplicate({{x,0,0,0},{0,y,0,0},{0,0,z,0}});
}

void fmatrix::transpose()
{
	const fixed a = m[0][1]; m[0][1] = m[1][0]; m[1][0] = a;
	const fixed b = m[0][2]; m[0][2] = m[2][0]; m[2][0] = b;
	const fixed c = m[1][2]; m[1][2] = m[2][1]; m[2][1] = c;
}

void fmatrix::project(fixed x,fixed y,fixed z,fixed w) //check! 
{
	m[0][0] = fx::div(z,x)<<1; m[0][1] = 0;               m[0][2] = 0;                m[0][3] = 0; 
	m[1][0] = 0;               m[1][1] = fx::div(z,y)<<1; m[1][2] = 0;                m[1][3] = 0; 
	m[2][0] = 0;               m[2][1] = 0;               m[2][2] = fx::div(w,(w-z)); m[2][3] = fx::div(fx::mul(-w,z),(w-z));  
}

void fmatrix::dyadic(const fvector& x,const fvector& y)
{
	m[0][0] = fx::mul(x.x,y.x); m[0][1] = fx::mul(x.x,y.y); m[0][2] = fx::mul(x.x,y.z); m[0][3] = 0; 
	m[1][0] = fx::mul(x.y,y.x); m[1][1] = fx::mul(x.y,y.y); m[1][2] = fx::mul(x.y,y.z); m[1][3] = 0; 
	m[2][0] = fx::mul(x.z,y.x); m[2][1] = fx::mul(x.z,y.y); m[2][2] = fx::mul(x.z,y.z); m[2][3] = 0; 
}

void fmatrix::shadow(const fvector& x,const fvector& y)
{
	dyadic(-x,y);
	const fixed xy = x.dot(y);
	m[0][0] += xy;
	m[1][1] += xy;
	m[2][2] += xy;
}

fixed fmatrix::trace() const
{
	return m[0][0]+m[1][1]+m[2][2];
}

fvector fmatrix::operator*(const fvector& x) const
{
	return fvector(fx::mul(m[0][0],x.x)+fx::mul(m[0][1],x.y)+fx::mul(m[0][2],x.z)+m[0][3],
		       fx::mul(m[1][0],x.x)+fx::mul(m[1][1],x.y)+fx::mul(m[1][2],x.z)+m[1][3],
		       fx::mul(m[2][0],x.x)+fx::mul(m[2][1],x.y)+fx::mul(m[2][2],x.z)+m[2][3],x.e);
}

lvector fmatrix::operator*(const lvector& x) const
{
	return lvector(fx::r2l(fx::mul(m[0][0],fx::l2f(x.x))+fx::mul(m[0][1],fx::l2f(x.y))+fx::mul(m[0][2],fx::l2f(x.z))+m[0][3]),
		       fx::r2l(fx::mul(m[1][0],fx::l2f(x.x))+fx::mul(m[1][1],fx::l2f(x.y))+fx::mul(m[1][2],fx::l2f(x.z))+m[1][3]),
		       fx::r2l(fx::mul(m[2][0],fx::l2f(x.x))+fx::mul(m[2][1],fx::l2f(x.y))+fx::mul(m[2][2],fx::l2f(x.z))+m[2][3]),x.e);
}
///</code>

