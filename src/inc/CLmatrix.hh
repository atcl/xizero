//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLMATRIX
#define HH_CLMATRIX
#pragma message "Compiling " __FILE__ " ! TODO: "

#include <iostream>
#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLconsts.hh"
#include "CLstruct.hh"
#include "CLvector.hh"
#include "CLmath.hh"


class CLmatrix : public virtual CLcl
{
	protected:
		CLmath* clmath;

	private:
		float m[4][4];
		
		void multiplicate(float n11,float n12,float n13,float n14,float n21,float n22,float n23,float n24,float n31,float n32,float n33,float n34,float n41,float n42,float n43,float n44);

	public:
		CLmatrix(bool i,CLmath* clm);
		~CLmatrix();
		void rotate(xlong x,xlong y,xlong z);
		void translate(xlong x,xlong y,xlong z);
		void scale(float x,float y,float z);
		void aspectscale(float x);
		template<class clvector>clvector transform(const clvector& t);
		void clear(xlong i);
		void zero();
		void unit();
		void transpone();
		template<class clvector>void dyadic(const clvector& a,const clvector& b);
		template<class clvector>void shadow(const clvector& l,const clvector& p);
		void project();
		float trace();
		float determinate();

		void print();
};


void CLmatrix::multiplicate(float n11,float n12,float n13,float n14,float n21,float n22,float n23,float n24,float n31,float n32,float n33,float n34,float n41,float n42,float n43,float n44)
{
	float tm0; float tm1; float tm2; float tm3;

	tm0 = m[0][0]; tm1 = m[0][1]; tm2 = m[0][2]; tm3 = m[0][3]; 
	m[0][0] = (tm0*n11 + tm1*n21 + tm2*n31 + tm3*n41);
	m[0][1] = (tm0*n12 + tm1*n22 + tm2*n32 + tm3*n42);
	m[0][2] = (tm0*n13 + tm1*n23 + tm2*n33 + tm3*n43);
	m[0][3] = (tm0*n14 + tm1*n24 + tm2*n34 + tm3*n44);

	tm0 = m[1][0]; tm1 = m[1][1]; tm2 = m[1][2]; tm3 = m[1][3];
	m[1][0] = (tm0*n11 + tm1*n21 + tm2*n31 + tm3*n41);
	m[1][1] = (tm0*n12 + tm1*n22 + tm2*n32 + tm3*n42);
	m[1][2] = (tm0*n13 + tm1*n23 + tm2*n33 + tm3*n43);
	m[1][3] = (tm0*n14 + tm1*n24 + tm2*n34 + tm3*n44);

	tm0 = m[2][0]; tm1 = m[2][1]; tm2 = m[2][2]; tm3 = m[2][3];
	m[2][0] = (tm0*n11 + tm1*n21 + tm2*n31 + tm3*n41);
	m[2][1] = (tm0*n12 + tm1*n22 + tm2*n32 + tm3*n42);
	m[2][2] = (tm0*n13 + tm1*n23 + tm2*n33 + tm3*n43);
	m[2][3] = (tm0*n14 + tm1*n24 + tm2*n34 + tm3*n44);

	tm0 = m[3][0]; tm1 = m[3][1]; tm2 = m[3][2]; tm3 = m[3][3];
	m[3][0] = (tm0*n11 + tm1*n21 + tm2*n31 + tm3*n41);
	m[3][1] = (tm0*n12 + tm1*n22 + tm2*n32 + tm3*n42);
	m[3][2] = (tm0*n13 + tm1*n23 + tm2*n33 + tm3*n43);
	m[3][3] = (tm0*n14 + tm1*n24 + tm2*n34 + tm3*n44);
}

CLmatrix::CLmatrix(bool i,CLmath* clm)
{
	clmath = clm;

	if(i==false) clear(0);
	if(i==true) unit();
}

CLmatrix::~CLmatrix() { }

void CLmatrix::rotate(xlong x,xlong y,xlong z)
{
	if(x!=0) 
	{
		float sinx = clmath->sinbyarray(x);
		float cosx = clmath->cosbyarray(x);
		multiplicate(1,0,0,0,0,cosx,-sinx,0,0,sinx,cosx,0,0,0,0,1);
	}	

	if(y!=0)
	{
		float siny = clmath->sinbyarray(y);
		float cosy = clmath->cosbyarray(y);
		multiplicate(cosy,0,siny,0,0,1,0,0,-siny,0,cosy,0,0,0,0,1);
	}

	if(z!=0)
	{
		float sinz = clmath->sinbyarray(z);
		float cosz = clmath->cosbyarray(z);
		multiplicate(cosz,-sinz,0,0,sinz,cosz,0,0,0,0,1,0,0,0,0,1);
	}
}

void CLmatrix::translate(xlong x,xlong y,xlong z)
{
	multiplicate(1,0,0,float(x),0,1,0,float(y),0,0,1,float(z),0,0,0,1);
}

void CLmatrix::scale(float x,float y,float z)
{
	multiplicate(x,0,0,0,0,y,0,0,0,0,z,0,0,0,0,1);
}

void CLmatrix::aspectscale(float x)
{
	multiplicate(x,0,0,0,0,x,0,0,0,0,x,0,0,0,0,1);
}

template<class clvector>
clvector CLmatrix::transform(const clvector& t)
{
	clvector r;

	r.x = xlong(m[0][0] * float(t.x) + m[0][1] * float(t.y) + m[0][2] * float(t.z) + m[0][3]);
	r.y = xlong(m[1][0] * float(t.x) + m[1][1] * float(t.y) + m[1][2] * float(t.z) + m[1][3]);
	r.z = xlong(m[2][0] * float(t.x) + m[2][1] * float(t.y) + m[2][2] * float(t.z) + m[2][3]);

	return r;
}

void CLmatrix::clear(xlong i)
{
	m[0][0]=m[0][1]=m[0][2]=m[0][3]=m[1][0]=m[1][1]=m[1][2]=m[1][3]=m[2][0]=m[2][1]=m[2][2]=m[2][3]=m[3][0]=m[3][1]=m[3][2]=m[3][3]=i;
}

void CLmatrix::zero()
{
	m[0][0]=m[0][1]=m[0][2]=m[0][3]=m[1][0]=m[1][1]=m[1][2]=m[1][3]=m[2][0]=m[2][1]=m[2][2]=m[2][3]=m[3][0]=m[3][1]=m[3][2]=m[3][3]=0;
}

void CLmatrix::unit()
{
	m[0][1]=m[0][2]=m[0][3]=m[1][0]=m[1][2]=m[1][3]=m[2][0]=m[2][1]=m[2][3]=m[3][0]=m[3][1]=m[3][2]=0;
	m[0][0]=m[1][1]=m[2][2]=m[3][3]=1;
}

void CLmatrix::transpone()
{
	float temp;

	temp = m[0][1]; m[0][1] = m[1][0]; m[1][0] = temp;
	temp = m[0][2]; m[0][2] = m[2][0]; m[2][0] = temp;
	temp = m[1][2]; m[1][2] = m[2][1]; m[2][1] = temp;

	temp = m[0][3]; m[0][3] = m[3][0]; m[3][0] = temp;
	temp = m[1][3]; m[1][3] = m[3][1]; m[3][1] = temp;
	temp = m[2][3]; m[2][3] = m[3][2]; m[3][2] = temp;
}

template<class clvector>
void CLmatrix::shadow(const clvector& l,const clvector& p)
{
	float ldotp = l*p;

	m[0][0] = (ldotp - (l.x * p.x));
	m[0][1] = -(l.x * p.y);
	m[0][2] = -(l.x * p.z);
	m[0][3] = 0;
	m[1][0] = -(l.y * p.x);
	m[1][1] = (ldotp - (l.y * p.y));
	m[1][2] = -(l.y * p.z);
	m[1][3] = 0;
	m[2][0] = -(l.z * p.x);
	m[2][1] = -(l.z * p.y);
	m[2][2] = (ldotp - (l.z * p.z));
	m[2][3] = 0;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

void CLmatrix::project()
{
	float zmin = 1;
	float zmax = zres-1;

	m[0][0] = (2 * zmin / xres);
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;
	m[1][0] = 0;
	m[1][1] = (2 * zmin / yres);
	m[1][2] = 0;
	m[1][3] = 0;
	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = ( zmax / (zmax - zmin) );
	m[2][3] = ( -zmax * zmin / (zmax - zmin) );
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 1;
	m[3][3] = 0;
}

template<class clvector>
void CLmatrix::dyadic(const clvector& a,const clvector& b)
{
	m[0][0] = float(a.x * b.x);
	m[0][1] = float(a.x * b.y);
	m[0][2] = float(a.x * b.z);
	m[0][3] = 0;
	m[1][0] = float(a.y * b.x);
	m[1][1] = float(a.y * b.y);
	m[1][2] = float(a.y * b.z);
	m[1][3] = 0;
	m[2][0] = float(a.z * b.x);
	m[2][1] = float(a.z * b.y);
	m[2][2] = float(a.z * b.z);
	m[2][3] = 0;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
}

float CLmatrix::trace()
{
	return (m[1][1] + m[2][2] + m[3][3] + m[4][4]);
}

void CLmatrix::print()
{
	std::cout << std::setw(7) << m[0][0] <<" "<< std::setw(7) << m[0][1] <<" "<< std::setw(7) << m[0][2] <<" "<< std::setw(7) << m[0][3] << std::endl;
	std::cout << std::setw(7) << m[1][0] <<" "<< std::setw(7) << m[1][1] <<" "<< std::setw(7) << m[1][2] <<" "<< std::setw(7) << m[1][3] << std::endl;
	std::cout << std::setw(7) << m[2][0] <<" "<< std::setw(7) << m[2][1] <<" "<< std::setw(7) << m[2][2] <<" "<< std::setw(7) << m[2][3] << std::endl;
	std::cout << std::setw(7) << m[3][0] <<" "<< std::setw(7) << m[3][1] <<" "<< std::setw(7) << m[3][2] <<" "<< std::setw(7) << m[3][3] << std::endl;
	std::cout << std::endl;
}

#endif

