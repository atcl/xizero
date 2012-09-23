///<header>
// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZgui.hh
// GUI base class 
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///</header>

///<include>
#include "XZbasic.hh"
///</include>

///<define>
class gui
{
	public:
		gui(sint l,sint t,sint w,sint h,sint c=0x00000000,sint b=0x00c0c0c0,sint f=0x00000000,bool v=1) : left(l),top(t),width(w),height(h),color(c),backcolor(b),framecolor(f),visible(v) { ; } 
		sint left;
		sint top;
		sint width;
		sint height;
		sint color;
		sint backcolor;
		sint framecolor;
		bool visible;
};
///</define>

