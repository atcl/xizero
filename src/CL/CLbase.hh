///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLBASE
#define HH_CLBASE
///*

///api includes
#include "CLtypes.hh"
#include "CLversion.hh"
///*

///header
/* class name:	CLbase
 * 
 * description:	The universal base class of all other classes
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///defintions
template<class T,bool S>
class CLbase
{
	private:
		static T* base;
		static uxlong version;
	protected:
		CLbase() { }; 
		CLbase(const CLbase&) { };	
	public:
		virtual ~CLbase() { };
   		static T& instance();
		static uxlong getversion() { return version; };
};

template<class T,bool S> T* CLbase<T,S>::base = 0;
template<class T,bool S> uxlong CLbase<T,S>::version = CLversion;
///*

///implementation
template<class T,bool S>
T& CLbase<T,S>::instance() //! noncritical
{
	//check if existing and singleton, if not create
	if(base==0 && S==1) { base = new T(); }
	return *base;
	//*
}
///*

#endif
