///license
//atCROSSLEVEL studios
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLGLOBALS
#define HH_CLGLOBALS
///*

///includes
#include "CLtypes.hh"
#include "CLsingle.hh"
///*

///header
/* class name:	CLglobal
 * 
 * description:	base class containing all singletons and global variables.
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///declarations
template<typename t> class CLbuffer; typedef CLbuffer<float> CLfbuffer; typedef CLbuffer<xlong> CLlbuffer; typedef CLbuffer<uxlong> CLubuffer;
template<typename t> class CLvector; typedef CLvector<float> CLfvector; typedef CLvector<xlong> CLlvector;

class CLwindow;
class CLbench;
class CLsystem;
class CLformat;
class CLmath;
class CLgame;
class CLmisc3d;
class CLfloor;
class CLdetect;
class CLfsprogress;
class CLgamepad;
class CLgfx1;
class CLgfx2;
class CLtransitions;
class CLsound;
class CLsprites;
class CLid;
class CLstring;
class CLapp;
class CLmsgbox;
class CLintro;
///*

///definitions
class CLglobal
{
	private:
		static CLglobal* global;
	protected:
		CLglobal() { };
		~CLglobal() { };
	public:
		static uxlong screenbytewidth;
		static uxlong screenbyteheight;
		static uxlong screenbytedepth;
		static uxlong screenbytesize;
		static uxlong screenpixelwidth;
		static uxlong screenpixelheight;
		static uxlong screenpixelsize;
		
		static CLubuffer cldoublebuffer;
		static CLubuffer cltriplebuffer;
		static CLubuffer clstencilbuffer;
		static CLfbuffer clzbuffer;
		
		static CLfvector light;
		static CLfvector slight;
		static CLfvector plane;
		
		static CLwindow* clwindow;
		static CLbench*  clbench;
		static CLsystem* clsystem;
		static CLformat* clformat;
		static CLmath*   clmath;
		static CLgame*   clgame;
		static CLmisc3d* clmisc3d;
		static CLfloor*  clfloor;
		static CLdetect* cldetect;
		static CLfsprogress* clfsprogress;
		static CLgamepad* clgamepad;
		static CLgfx1*   clgfx1;
		static CLgfx2*   clgfx2;
		static CLtransitions* cltransitions;
		static CLsound*  clsound;
		static CLsprites* clsprites;
		static CLid*     clid;
		static CLstring* clstring;
		static CLapp*    clapp;
		static CLmsgbox* clmsgbox;
		static CLintro*  clintro;
		
		static CLglobal* getinstance();
};
///*

#endif
