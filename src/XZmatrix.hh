///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Matrix Class ( XZmatrix.hh )
#pragma once
///</header>

///<include>
#include "XZmath.hh"
#include "XZvector.hh"
///</include>

///<define>
class matrix
{
	private:
		vector mat[4];

		void multiply(const vector (&n)[4]);
	public:
		matrix(fixed x=FXONE,fixed y=0);
		       void clear(fixed x=FXONE,fixed y=0);
		inline void rotatex(fixed x);
		inline void rotatey(fixed y);
		inline void rotatez(fixed z);
		inline void translate(fixed x,fixed y,fixed z);
		inline void scale(fixed x,fixed y,fixed z);
		       void transpose();
		       void dyadic(const vector& x,const vector& y);
		       void shadow(const vector& x,const vector& y);
		       void explode(const vector& n,fixed x);
		       void implode(fixed x);
		     vector operator*(const vector& x) const;
};
///</define>

///<code>
void matrix::multiply(const vector (&n)[4])
{
	mat[0] = vector{ fx::dot(mat[0],n[0]),fx::dot(mat[0],n[1]),fx::dot(mat[0],n[2]),fx::dot(mat[0],n[3]) };
	mat[1] = vector{ fx::dot(mat[1],n[0]),fx::dot(mat[1],n[1]),fx::dot(mat[1],n[2]),fx::dot(mat[1],n[3]) };
	mat[2] = vector{ fx::dot(mat[2],n[0]),fx::dot(mat[2],n[1]),fx::dot(mat[2],n[2]),fx::dot(mat[2],n[3]) };
	mat[3] = vector{ fx::dot(mat[3],n[0]),fx::dot(mat[3],n[1]),fx::dot(mat[3],n[2]),fx::dot(mat[3],n[3]) };
}

matrix::matrix(fixed x,fixed y)
{
	clear(x,y);
}

void matrix::clear(fixed x,fixed y)
{
	mat[0] = vector{x,y,y,y};
	mat[1] = vector{y,x,y,y};
	mat[2] = vector{y,y,x,y};
	mat[3] = vector{y,y,y,x};
}

void matrix::rotatex(fixed x)
{
	x = fx::mul(x,FXD2R);
	const fixed s = fx::sin(x);
	const fixed c = fx::cos(x);
	multiply({vector{FXONE,0,0,0},vector{0,c,s,0},vector{0,-s,c,0},vector{0,0,0,FXONE}});
}
	
void matrix::rotatey(fixed y)
{
	y = fx::mul(y,FXD2R);
	const fixed s = fx::sin(y);
	const fixed c = fx::cos(y);
	multiply({vector{c,0,-s,0},vector{0,FXONE,0,0},vector{s,0,c,0},vector{0,0,0,FXONE}});
}

void matrix::rotatez(fixed z)
{
	z = fx::mul(z,FXD2R);
	const fixed s = fx::sin(z);
	const fixed c = fx::cos(z);
	multiply({vector{c,s,0,0},vector{-s,c,0,0},vector{0,0,FXONE,0},vector{0,0,0,FXONE}});
}

void matrix::translate(fixed x,fixed y,fixed z)
{
	multiply({vector{FXONE,0,0,0},vector{0,FXONE,0,0},vector{0,0,FXONE,0},vector{x,y,z,FXONE}});
}

void matrix::scale(fixed x,fixed y,fixed z)
{
	multiply({vector{x,0,0,0},vector{0,y,0,0},vector{0,0,z,0},vector{0,0,0,FXONE}});
}

void matrix::transpose()
{
	math::swp(mat[0].y,mat[1].x);
	math::swp(mat[0].z,mat[2].x);
	math::swp(mat[0].e,mat[3].x);

	math::swp(mat[1].z,mat[2].y);
	math::swp(mat[1].e,mat[3].y);

	math::swp(mat[2].e,mat[3].z);
}

void matrix::dyadic(const vector& x,const vector& y)
{
	mat[0] = fx::mul(y,x.x);
	mat[1] = fx::mul(y,x.y);
	mat[2] = fx::mul(y,x.z);
	mat[3] = fx::mul(y,x.e);
}

void matrix::shadow(const vector& x,const vector& y)
{
	dyadic(-x,y);
	const fixed xy = fx::dot(x,y);
	mat[0].x += xy;
	mat[1].y += xy;
	mat[2].z += xy;
}

void matrix::explode(const vector& n,fixed x)
{
	const fixed  l = fx::div(x,n.e);
	const vector m = fx::mul(n,l);
	mat[0] = vector{FXONE,0,0,m.x};
	mat[1] = vector{0,FXONE,0,m.y};
	mat[2] = vector{0,0,FXONE,m.z};
	mat[3] = vector{0,0,0,FXONE};
}

void matrix::implode(fixed x) //fix
{
	x = math::rnd(x);

	mat[0] = vector{FXONE-x,0,0,0};
	mat[1] = vector{0,FXONE-x,0,0};
	mat[2] = vector{0,0,FXONE-x,0};
	mat[3] = vector{0,0,0,FXONE};
}

vector matrix::operator*(const vector& x) const
{
	return vector{fx::dot(mat[0],x),fx::dot(mat[1],x),fx::dot(mat[2],x),FXONE};
}
///</code>

