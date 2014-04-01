///<header>
// atCROSSLEVEL 2010-2014
// released under 2-clause BSD license
// XZgui.hh
// GUI base class
#pragma once
///</header>

///<include>
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
		const xint alertcolor;
		const xint backcolor;
		const xint framecolor;
		      bool visible;
	public:
		gui(xint l,xint t,xint w,xint h,xint c=0x00000000,xint a=0x00ff0000,xint b=0x00c0c0c0,xint f=0x00000000,bool v=1) : left(l),top(t),width(w),height(h),color(c),alertcolor(a),backcolor(b),framecolor(f),visible(v) { ; } 
		inline void pos(xint x,xint y) { left = x; top = y; };
		inline void vis(bool v) { visible = v; };
};
///</define>

