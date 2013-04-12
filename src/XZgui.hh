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
		      sint left;
		      sint top;
		const sint width;
		const sint height;
		const sint color;
		const sint backcolor;
		const sint framecolor;
		      bool visible;
	public:
		gui(sint l,sint t,sint w,sint h,sint c=0x00000000,sint b=0x00c0c0c0,sint f=0x00000000,bool v=1) : left(l),top(t),width(w),height(h),color(c),backcolor(b),framecolor(f),visible(v) { ; } 
		inline void pos(sint x,sint y) { left = x; top = y; };
		inline void vis(bool v) { visible = v; };
};
///</define>

