///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLAPP
#define HH_CLAPP
///*

///sys includes
#include <cstdlib>
#include <cstdio>
///*

///api includes
#include "CLtypes.hh"
#include "CLversion.hh"
#include "CLresource.hh"
#include "CLutils.hh"
#include "CLbase.hh"
///*

///header
/* class name:	CLapp
 * 
 * description:	This class manages the application itself.
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version:		0.2
 */
///*

///definitions
class CLapp : public CLbase<CLapp,1>
{
	friend class CLbase<CLapp,1>;
	
	protected:
		const xchar* name;
		const xchar* title;
		const xchar** icon;
		uxlong size;
		const xchar* version;
		CLapp();
		~CLapp() { };
	public:
		void init(char* argv[]);
		void exit(const xlong r=0,const xchar* m="") const;
		xchar inkey(const bool b) const;
		const xchar* getname() const { return name; };
		const xchar* gettitle() const { return title; };
		const xchar** geticon() const { return icon; };
		void seticon(const xchar** i) { icon = i; }
		uxlong getsize() const { return size; };
		const xchar* getversion() const { return version; };
};
///*

///implementation
CLapp::CLapp() : title(TITLE), version(VERSION), icon(CLicon) //! noncritical
{
	//icon = CLicon;
}

void CLapp::init(char* argv[])
{
	name = argv[0];
	FILE* temp = fopen(name,"rb");
	fseek(temp,0,SEEK_END);
	size = (ftell(temp))>>10;
	fclose(temp);
	tty(title); tty(" ("); tty(name); tty(") version:"); tty(version); tty(" size:"); tty(size); say("KB");
}

void CLapp::exit(const xlong r,const xchar* m) const //! noncritical
{
	say(m);
	::exit(r);
}

xchar CLapp::inkey(const bool b) const //! noncritical
{
	return xchar(getchar());
}
///*

#endif
