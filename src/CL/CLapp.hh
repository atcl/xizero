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
	friend class CLglobal;
	
	protected:
		const xchar* title;
		const xchar** icon;
		const xchar* exename;
		uxlong size;
		const xchar* version;
		CLapp();
		~CLapp() { };
		void setexename(const xchar* n) { exename = n; }
	public:
		void exit(const xlong r=0,const xchar* m="") const;
		xchar inkey(const bool b) const { return xchar(getchar()); };
		const xchar* gettitle() const { return title; };
		const xchar** geticon() const { return icon; };
		void seticon(const xchar** i) { icon = i; }
		uxlong getsize() const { return size; };
		const xchar* getversion() const { return version; };
};
///*

///implementation
CLapp::CLapp() : title(TITLE), version(VERSION), icon(CLicon), exename(NAME) //! noncritical
{
	FILE* temp = fopen(exename,"rb");
	fseek(temp,0,SEEK_END);
	size = (ftell(temp))>>10;
	fclose(temp);
	tty(title); tty(" ("); tty(exename); tty(") version:"); tty(version); tty(" size:"); tty(size); say("KB");
}

void CLapp::exit(const xlong r,const xchar* m) const //! noncritical
{
	say(m);
	::exit(r);
}
///*

#endif
