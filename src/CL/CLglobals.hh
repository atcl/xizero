//atCROSSLEVEL studios
//licensed under zlib/libpng license
#ifndef HH_CLGLOBALS
#define HH_CLGLOBALS
#pragma message "Compiling " __FILE__ " ! TODO: ..."

#include "CLtypes.hh"
#include "CLsingle.hh"

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

template<typename t> class CLbuffer; typedef CLbuffer<float> CLfbuffer; typedef CLbuffer<xlong> CLlbuffer;
template<typename t> class CLvector; typedef CLvector<float> CLfvector; typedef CLvector<xlong> CLlvector;

class CLwindow;
class CLbench;
class CLsystem;
class CLutils;
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
class CLintro;
class CLsprites;
class CLid;
class CLcursor;

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
		
		static CLlbuffer cldoublebuffer;
		static CLlbuffer clstencilbuffer;
		static CLfbuffer clzbuffer;
		
		static CLfvector light;
		static CLfvector slight;
		static CLfvector plane;
		
		static CLwindow* clwindow;
		static CLbench*  clbench;
		static CLsystem* clsystem;
		static CLformat* clformat;
		static CLutils*  clutils;
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
		static CLintro*  clintro;
		static CLid*     clid;
		static CLcursor* clcursor;
		
		static CLglobal* getinstance();
}; 

#endif