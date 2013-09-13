///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZmatrix.hh
// Matrix Class Library
#pragma once
///</header>

///<include>
#include "XZvector.hh"
///</include>

///<define>
struct matrix
{
	private:
		alignas(16) fixed m[3][4];
		/*OK*/ void multiplicate(const fixed (&n)[3][4]);
	public:
		/*OK*/ matrix(fixed x=FXONE,fixed y=0);
		/*OK*/        void clear(fixed x=FXONE,fixed y=0);
		/*OK*/ inline void rotatex(fixed x);
		/*OK*/ inline void rotatey(fixed y);
		/*OK*/ inline void rotatez(fixed z);
		/*OK*/ inline void translate(fixed x,fixed y,fixed z);
		/*OK*/ inline void scale(fixed x,fixed y,fixed z);
		              void project(fixed x,fixed y,fixed z,fixed w);
		/*OK*/        void transpose();
		/*OK*/        void dyadic(const vector& x,const vector& y);
		/*OK*/ inline void shadow(const vector& x,const vector& y);
		/*OK*/ inline fixed trace() const;
		/*OK*/ inline vector operator*(const vector& x) const;
};
///</define>

///<code>
void matrix::multiplicate(const fixed (&n)[3][4])
{
	alignas(16) const fixed a[3][4] = { {	fx::mul(m[0][0],n[0][0])+fx::mul(m[0][1],n[1][0])+fx::mul(m[0][2],n[2][0]),
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

matrix::matrix(fixed x,fixed y)
{
	clear(x,y);
}

void matrix::clear(fixed x,fixed y)
{
	m[0][0] = m[1][1] = m[2][2] = x; 
	m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] = y; 
}

void matrix::rotatex(fixed x)
{
	x = fx::mul(x,FXD2R);
	const fixed s = fx::sin(x);
	const fixed c = fx::cos(x);
	multiplicate({{FXONE,0,0,0},{0,c,-s,0},{0,s,c,0}});
}
	
void matrix::rotatey(fixed y)
{
	y = fx::mul(y,FXD2R);
	const fixed s = fx::sin(y);
	const fixed c = fx::cos(y);
	multiplicate({{c,0,s,0},{0,FXONE,0,0},{-s,0,c,0}});
}

void matrix::rotatez(fixed z)
{
	z = fx::mul(z,FXD2R);
	const fixed s = fx::sin(z);
	const fixed c = fx::cos(z);
	multiplicate({{c,-s,0,0},{s,c,0,0},{0,0,FXONE,0}});
}

void matrix::translate(fixed x,fixed y,fixed z)
{
	multiplicate({{FXONE,0,0,x},{0,FXONE,0,y},{0,0,FXONE,z}});
}

void matrix::scale(fixed x,fixed y,fixed z)
{
	multiplicate({{x,0,0,0},{0,y,0,0},{0,0,z,0}});
}

void matrix::transpose()
{
	const fixed a = m[0][1]; m[0][1] = m[1][0]; m[1][0] = a;
	const fixed b = m[0][2]; m[0][2] = m[2][0]; m[2][0] = b;
	const fixed c = m[1][2]; m[1][2] = m[2][1]; m[2][1] = c;
}

void matrix::project(fixed x,fixed y,fixed z,fixed w) //check! 
{
	m[0][0] = fx::div(z,x)<<1; m[0][1] = 0;               m[0][2] = 0;                m[0][3] = 0; 
	m[1][0] = 0;               m[1][1] = fx::div(z,y)<<1; m[1][2] = 0;                m[1][3] = 0; 
	m[2][0] = 0;               m[2][1] = 0;               m[2][2] = fx::div(w,(w-z)); m[2][3] = fx::div(fx::mul(-w,z),(w-z));  
}

void matrix::dyadic(const vector& x,const vector& y)
{
	m[0][0] = fx::mul(x.x,y.x); m[0][1] = fx::mul(x.x,y.y); m[0][2] = fx::mul(x.x,y.z); m[0][3] = 0; 
	m[1][0] = fx::mul(x.y,y.x); m[1][1] = fx::mul(x.y,y.y); m[1][2] = fx::mul(x.y,y.z); m[1][3] = 0; 
	m[2][0] = fx::mul(x.z,y.x); m[2][1] = fx::mul(x.z,y.y); m[2][2] = fx::mul(x.z,y.z); m[2][3] = 0; 
}

void matrix::shadow(const vector& x,const vector& y)
{
	dyadic(-x,y);
	const fixed xy = x.dot(y);
	m[0][0] += xy;
	m[1][1] += xy;
	m[2][2] += xy;
}

fixed matrix::trace() const
{
	return m[0][0]+m[1][1]+m[2][2];
}

vector matrix::operator*(const vector& x) const
{
	return vector(fx::mul(m[0][0],x.x)+fx::mul(m[0][1],x.y)+fx::mul(m[0][2],x.z)+m[0][3],
		      fx::mul(m[1][0],x.x)+fx::mul(m[1][1],x.y)+fx::mul(m[1][2],x.z)+m[1][3],
		      fx::mul(m[2][0],x.x)+fx::mul(m[2][1],x.y)+fx::mul(m[2][2],x.z)+m[2][3],x.e);
}
///</code>

