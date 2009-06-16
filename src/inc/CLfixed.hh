//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFIXED
#define HH_CLFIXED
#warning "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"


union fixedp
{
	xlong f;
	xshort p[2]; //p[1]=fractional, p[0]=numeral

	fixedp operator=(fixedp &a);
	fixedp operator=(xshort& a);

	xshort toshort();
	xlong  tolong();

	fixedp round();
	fixedp roundup();
	fixedp rounddown();
};

fixedp fixedp::operator=(fixedp &a)
{
	fixedp temp;
	temp.f = a.f;
	return temp;
}

fixedp fixedp::operator=(xshort &a)
{
	fixedp temp;
	temp.p[0] = a;
	temp.p[1] = 0;
}

xshort fixedp::toshort()
{

}

xlong fixedp::tolong()
{

}

fixedp fixedp::round()
{

}

fixedp fixedp::roundup()
{

}

fixedp fixedp::rounddown()
{

}

fixedp& operator++(fixedp &a)
{
	a.p[0]++;
}

fixedp& operator--(fixedp &a)
{
	a.p[0]--;
}

fixedp operator+=(fixedp &a,fixedp &b)
{

}

fixedp operator+=(fixedp &a,xshort &b)
{

}

fixedp operator-=(fixedp &a,fixedp &b)
{

}

fixedp operator-=(fixedp &a,xshort &b)
{

}

fixedp operator+(fixedp &a,fixedp &b)
{
	fixedp temp;
	temp.f =  a.f + b.f;
	return temp;
}

fixedp operator+(fixedp &a,xshort &b)
{
	fixedp temp;
	temp.f = a.f;
	temp.p[0] += b;
	return temp;
}

fixedp operator+(xshort &a,fixedp &b)
{
	fixedp temp;
	temp.f = b.f;
	temp.p[0] += a;
	return temp;
}

fixedp operator-(fixedp &a,fixedp &b)
{

}

fixedp operator-(fixedp &a,xshort &b)
{

}

fixedp operator-(xshort &a,fixedp &b)
{

}

fixedp operator*(fixedp &a,fixedp &b)
{

}

fixedp operator*(fixedp &a,xshort &b)
{

}

fixedp operator*(xshort &a,fixedp &b)
{

}

fixedp operator/(fixedp &a,fixedp &b)
{

}

fixedp operator/(fixedp &a,xshort &b)
{

}

fixedp operator/(xshort &a,fixedp &b)
{

}

bool operator==(fixedp &a,fixedp &b)
{

}

bool operator!=(fixedp &a,fixedp &b)
{

}

bool operator<(fixedp &a,fixedp &b)
{

}

bool operator<(fixedp &a,xshort &b)
{

}

bool operator<(xshort &a,fixedp &b)
{

}

bool operator>(fixedp &a,fixedp &b)
{

}

bool operator>(fixedp &a,xshort &b)
{

}

bool operator>(xshort &a,fixedp &b)
{

}

bool operator<=(fixedp &a,fixedp &b)
{

}

bool operator<=(fixedp &a,xshort &b)
{

}

bool operator<=(xshort &a,fixedp &b)
{

}

bool operator>=(fixedp &a,fixedp &b)
{

}

bool operator>=(fixedp &a,xshort &b)
{

}

bool operator>=(xshort &a,fixedp &b)
{

}

#endif
