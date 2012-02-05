// atCROSSLEVEL 2010,2011,2012
// licensed under zlib-license
// XZbutton.hh
// GUI Button Library 

///guard
#ifndef HH_XZBUTTON
#define HH_XZBUTTON
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include "XZbasic.hh"
#include "XZutil.hh"
#include "XZmath.hh"
#include "XZgui.hh"
#include "XZlist.hh"
#include "XZgfx.hh"
#include "XZfont.hh"
#include "XZstring.hh"
#include "XZgame.hh"
///*

///definitions
class button : public gui
{
	private:
		char* text;						//Button Caption
		sint (*action)();					//Onclick Function Pointer
		bool depth;						//Flat or Relief
		const sint textleft;					//Relative Y Coordinate of Caption	
		const sint texttop;					//Relative X Coordinate of Caption
		static list bl;						//List of all Buttons
	public:
		button(const char* x,sint (*a)(),bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v); //Constructor
		~button();						//Destructor
		void draw() const;					//Draw Button
		static sint check(sint x,sint y,sint b);		//Check If Button Clicked
};
///*

///implementation
list button::bl = list();

button::button(const char* x,sint (*a)(),bool d,sint l,sint t,sint w,sint h,sint c,sint b,sint f,bool v) :
	gui(l,t,w,h,c,b,f,v),
	text(string::copy(x)),
	action(a),
	depth(d),
	textleft( (math::max(w-font::width(x,SYSFONT),0))>>1 ),
	texttop( (math::max(h-font::height(x,SYSFONT),0))>>1 )
{
	bl.append(this);
}

button::~button()
{
	bl.find(this);
	bl.delcurrent();
	delete text;
}

sint button::check(sint x,sint y,sint b)
{
	const sint e = math::neg(bl.length(),b==0);
	for(sint i=bl.first();i<e;i+=bl.next())
	{
		const button* temp = (button*)bl.current();
		if(temp->visible!=0 && game::inside(x,y,temp->left,temp->top,temp->left+temp->width,temp->top+temp->height)) 
		{
			return temp->action();
		}
	}
	return 0;
}

void button::draw() const
{
	guard(visible==0);
	gfx::rect(left,top,left+width,top+height,framecolor,backcolor,1,depth);
	font::draw(left+textleft,top+texttop,text,color,backcolor,SYSFONT);
}
///*

#endif

