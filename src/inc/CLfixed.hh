//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLFIXED
#define HH_CLFIXED
#pragma message "Compiling " __FILE__ " ! TODO: all"


union fixedp
{
	xlong f;
	xshort p[2]; //p[0]=fractional, p[1]=numeral //?switch?

	fixedp operator=(fixedp &a);
	fixedp operator=(xshort& a);

	xshort toshort();
	xlong  tolong();
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
	temp.p[1] = a;
	temp.p[0] = 0;
}

xshort fixedp::toshort()
{

}

xlong fixedp::tolong()
{

}

fixedp operator++(fixedp &a)
{

}

fixedp operator--(fixedp &a)
{

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
	temp.p[1] += b;
	return temp;
}

fixedp operator+(xshort &a,fixedp &b)
{
	fixedp temp;
	temp.f = b.f;
	temp.p[1] += a;
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
