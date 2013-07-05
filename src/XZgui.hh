///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZgui.hh
// GUI base class 
///</header>

///<include>
#pragma once
#include "XZbasic.hh"
///</include>

///<define>
class gui
{
	protected:
		      xint left;
		      xint top;
		const xint width;
		const xint height;
		const xint color;
		const xint backcolor;
		const xint framecolor;
		      bool visible;
	public:
		gui(xint l,xint t,xint w,xint h,xint c=0x00000000,xint b=0x00c0c0c0,xint f=0x00000000,bool v=1) : left(l),top(t),width(w),height(h),color(c),backcolor(b),framecolor(f),visible(v) { ; } 
		inline void pos(xint x,xint y) { left = x; top = y; };
		inline void vis(bool v) { visible = v; };
};
///</define>

