///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLMATRIX
#define HH_CLMATRIX
///*

///includes
#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLvector.hh"
#include "CLmath.hh"
#include "CLmacros.hh"
///*

///header
/* class name:	CLmatrix
 * 
 * description:	A 4x4 matrix for 3d transformations
 * 
 * author:	atcl
 * 
 * notes:	finish projection.
 * 
 * version: 0.1
 */
///*

///definitions
class CLmatrix : public virtual CLcl
{
	private:
		float m[4][4];
		float t[4][4]; //todo: implement
		inline void multiplicate();
	public:
		CLmatrix(bool i=1);
		~CLmatrix();
		void rotate(xlong x,xlong y,xlong z);
		void translate(float x,float y,float z);
		void scale(float x,float y,float z);
		void aspectscale(float x);
		void superscale(float x);
		void reflect();
		template<class clvector>clvector transform(const clvector& t) const;
		void clear(xlong i);
		void zero();
		void unit();
		void transpone();
		template<class clvector>void dyadic(const clvector& a,const clvector& b);
		template<class clvector>void shadow(const clvector& l,const clvector& p);
		void project();
		float trace() const;
		float determinate() const;
		void print() const;
};
///*

///implementation
void CLmatrix::multiplicate() //! supercritical
{
	float r[4][4];
	
	r[0][0] = m[0][0]; r[0][1] = m[0][1]; r[0][2] = m[0][2]; r[0][3] = m[0][3];
	r[1][0] = m[1][0]; r[1][1] = m[1][1]; r[1][2] = m[1][2]; r[1][3] = m[1][3];
	r[2][0] = m[2][0]; r[2][1] = m[2][1]; r[2][2] = m[2][2]; r[2][3] = m[2][3];
	r[3][0] = m[3][0]; r[3][1] = m[3][1]; r[3][2] = m[3][2]; r[3][3] = m[3][3];

	m[0][0] = (r[0][0]*t[0][0]) + (r[0][1]*t[1][0]) + (r[0][2]*t[2][0]) + (r[0][3]*t[3][0]);
	m[0][1] = (r[0][0]*t[0][1]) + (r[0][1]*t[1][1]) + (r[0][2]*t[2][1]) + (r[0][3]*t[3][1]);
	m[0][2] = (r[0][0]*t[0][2]) + (r[0][1]*t[1][2]) + (r[0][2]*t[2][2]) + (r[0][3]*t[3][2]);
	m[0][3] = (r[0][0]*t[0][3]) + (r[0][1]*t[1][3]) + (r[0][2]*t[2][3]) + (r[0][3]*t[3][3]);

	m[1][0] = (r[1][0]*t[0][0]) + (r[1][1]*t[1][0]) + (r[1][2]*t[2][0]) + (r[1][3]*t[3][0]);
	m[1][1] = (r[1][0]*t[0][1]) + (r[1][1]*t[1][1]) + (r[1][2]*t[2][1]) + (r[1][3]*t[3][1]);
	m[1][2] = (r[1][0]*t[0][2]) + (r[1][1]*t[1][2]) + (r[1][2]*t[2][2]) + (r[1][3]*t[3][2]);
	m[1][3] = (r[1][0]*t[0][3]) + (r[1][1]*t[1][3]) + (r[1][2]*t[2][3]) + (r[1][3]*t[3][3]);
	
	m[2][0] = (r[2][0]*t[0][0]) + (r[2][1]*t[1][0]) + (r[2][2]*t[2][0]) + (r[2][3]*t[3][0]);
	m[2][1] = (r[2][0]*t[0][1]) + (r[2][1]*t[1][1]) + (r[2][2]*t[2][1]) + (r[2][3]*t[3][1]);
	m[2][2] = (r[2][0]*t[0][2]) + (r[2][1]*t[1][2]) + (r[2][2]*t[2][2]) + (r[2][3]*t[3][2]);
	m[2][3] = (r[2][0]*t[0][3]) + (r[2][1]*t[1][3]) + (r[2][2]*t[2][3]) + (r[2][3]*t[3][3]);

	m[3][0] = (r[3][0]*t[0][0]) + (r[3][1]*t[1][0]) + (r[3][2]*t[2][0]) + (r[3][3]*t[3][0]);
	m[3][1] = (r[3][0]*t[0][1]) + (r[3][1]*t[1][1]) + (r[3][2]*t[2][1]) + (r[3][3]*t[3][1]);
	m[3][2] = (r[3][0]*t[0][2]) + (r[3][1]*t[1][2]) + (r[3][2]*t[2][2]) + (r[3][3]*t[3][2]);
	m[3][3] = (r[3][0]*t[0][3]) + (r[3][1]*t[1][3]) + (r[3][2]*t[2][3]) + (r[3][3]*t[3][3]);
}

CLmatrix::CLmatrix(bool i) //! noncritical
{
	if(i==false) clear(0);
	if(i==true) unit();
}

CLmatrix::~CLmatrix() { } //! noncritical

void CLmatrix::rotate(xlong x,xlong y,xlong z) //! noncritical
{
	if(x!=0) 
	{
		float sinx = clmath->sin(x);
		float cosx = clmath->cos(x);
		t[0][0] = 1;	t[0][1] = 0;	t[0][2] = 0;	t[0][3] = 0;
		t[1][0] = 0;	t[1][1] = cosx;	t[1][2] =-sinx;	t[1][3] = 0;
		t[2][0] = 0;	t[2][1] = sinx;	t[2][2] = cosx;	t[2][3] = 0;
		t[3][0] = 0;	t[3][1] = 0;	t[3][2] = 0;	t[3][3] = 1;
		multiplicate();
	}	

	if(y!=0)
	{
		float siny = clmath->sin(y);
		float cosy = clmath->cos(y);
		t[0][0] = cosy;	t[0][1] = 0;	t[0][2] = siny;	t[0][3] = 0;
		t[1][0] = 0;	t[1][1] = 1;	t[1][2] = 0;	t[1][3] = 0;
		t[2][0] =-siny;	t[2][1] = 0;	t[2][2] = cosy;	t[2][3] = 0;
		t[3][0] = 0;	t[3][1] = 0;	t[3][2] = 0;	t[3][3] = 1;
		multiplicate();
	}

	if(z!=0)
	{
		float sinz = clmath->sin(z);
		float cosz = clmath->cos(z);
		t[0][0] = cosz;	t[0][1] =-sinz;	t[0][2] = 0;	t[0][3] = 0;
		t[1][0] = sinz;	t[1][1] = cosz;	t[1][2] = 0;	t[1][3] = 0;
		t[2][0] = 0;	t[2][1] = 0;	t[2][2] = 1;	t[2][3] = 0;
		t[3][0] = 0;	t[3][1] = 0;	t[3][2] = 0;	t[3][3] = 1;
		multiplicate();
	}
}

void CLmatrix::translate(float x,float y,float z) //! noncritical
{
	//translate using fourth column of matrix
	t[0][0] = 1;	t[0][1] = 0;	t[0][2] = 0;	t[0][3] = x;
	t[1][0] = 0;	t[1][1] = 1;	t[1][2] = 0;	t[1][3] = y;
	t[2][0] = 0;	t[2][1] = 0;	t[2][2] = 1;	t[2][3] = z;
	t[3][0] = 0;	t[3][1] = 0;	t[3][2] = 0;	t[3][3] = 1;
	multiplicate();
	//*
}

void CLmatrix::scale(float x,float y,float z) //! noncritical
{
	//scale x,y,z seperately
	t[0][0] = x;	t[0][1] = 0;	t[0][2] = 0;	t[0][3] = 0;
	t[1][0] = 0;	t[1][1] = y;	t[1][2] = 0;	t[1][3] = 0;
	t[2][0] = 0;	t[2][1] = 0;	t[2][2] = z;	t[2][3] = 0;
	t[3][0] = 0;	t[3][1] = 0;	t[3][2] = 0;	t[3][3] = 1;
	multiplicate();
	//*
}

void CLmatrix::aspectscale(float x) //! noncritical
{
	//scale x,y,z alike
	t[0][0] = x;	t[0][1] = 0;	t[0][2] = 0;	t[0][3] = 0;
	t[1][0] = 0;	t[1][1] = x;	t[1][2] = 0;	t[1][3] = 0;
	t[2][0] = 0;	t[2][1] = 0;	t[2][2] = x;	t[2][3] = 0;
	t[3][0] = 0;	t[3][1] = 0;	t[3][2] = 0;	t[3][3] = 1;
	multiplicate();
	//*
}

void CLmatrix::superscale(float x) //! noncritical
{
	//scale all matrix elements
	m[0][0] *= x; m[0][1] *= x; m[0][2] *= x; m[0][3] *= x;
	m[1][0] *= x; m[1][1] *= x; m[1][2] *= x; m[1][3] *= x;
	m[2][0] *= x; m[2][1] *= x; m[2][2] *= x; m[2][3] *= x;
	m[3][0] *= x; m[3][1] *= x; m[3][2] *= x; m[3][3] *= x;
	//*
}

template<class clvector>
clvector CLmatrix::transform(const clvector& t) const //! critical
{
	clvector r;
	r.x = (m[0][0] * float(t.x) + m[0][1] * float(t.y) + m[0][2] * float(t.z) + m[0][3]);
	r.y = (m[1][0] * float(t.x) + m[1][1] * float(t.y) + m[1][2] * float(t.z) + m[1][3]);
	r.z = (m[2][0] * float(t.x) + m[2][1] * float(t.y) + m[2][2] * float(t.z) + m[2][3]);
	return r;
}

void CLmatrix::clear(xlong i) //! noncritical
{
	//set matrix to given value
	m[0][0]=m[0][1]=m[0][2]=m[0][3]=m[1][0]=m[1][1]=m[1][2]=m[1][3]=m[2][0]=m[2][1]=m[2][2]=m[2][3]=m[3][0]=m[3][1]=m[3][2]=m[3][3]=i;
	//*
}

void CLmatrix::zero() //! noncritical
{
	//zero out matrix
	m[0][0]=m[0][1]=m[0][2]=m[0][3]=m[1][0]=m[1][1]=m[1][2]=m[1][3]=m[2][0]=m[2][1]=m[2][2]=m[2][3]=m[3][0]=m[3][1]=m[3][2]=m[3][3]=0;
	//*
}

void CLmatrix::unit() //! noncritical
{
	//set to unit matrix
	m[0][1]=m[0][2]=m[0][3]=m[1][0]=m[1][2]=m[1][3]=m[2][0]=m[2][1]=m[2][3]=m[3][0]=m[3][1]=m[3][2]=0;
	m[0][0]=m[1][1]=m[2][2]=m[3][3]=1;
	//*
}

void CLmatrix::transpone() //! noncritical
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
void CLmatrix::shadow(const clvector& l,const clvector& p) //! critical
{
	float ldotp = l*p;

	//set matrix to shadow matrix of light l and plane p 
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
	//*
}

void CLmatrix::project() //! critical
{
	float zmin = 1;
	float zmax = ZRES-1;

	//set matrix to linear projection 
	m[0][0] = (2 * zmin / XRES);
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = 0;
	m[1][0] = 0;
	m[1][1] = (2 * zmin / YRES);
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
	//*
}

template<class clvector>
void CLmatrix::dyadic(const clvector& a,const clvector& b) //! critical
{
	//set matrix to 3x3 dyadic product of a and b
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
	//*
}

float CLmatrix::trace() const //! noncritical 
{
	//sum of all main diagonal elements
	return (m[0][0] + m[1][1] + m[2][2] + m[3][3]);
	//*
}

void CLmatrix::print() const //! noncritical
{
	tty(m[0][0]); tty(' '); tty(m[0][1]); tty(' '); tty(m[0][2]); tty(' '); say(m[0][3]);
	tty(m[1][0]); tty(' '); tty(m[1][1]); tty(' '); tty(m[1][2]); tty(' '); say(m[1][3]);
	tty(m[2][0]); tty(' '); tty(m[2][1]); tty(' '); tty(m[2][2]); tty(' '); say(m[2][3]);
	tty(m[3][0]); tty(' '); tty(m[3][1]); tty(' '); tty(m[3][2]); tty(' '); say(m[3][3]);
	tty();
}
///*

#endif

