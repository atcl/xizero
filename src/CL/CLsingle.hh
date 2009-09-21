//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLSINGLE
#define HH_CLSINGLE
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"

/* class name:	CLsingle
 * 
 * description:	Singleton base class
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */

template<class T>
class CLsingle
{
	private:
		static T* single;
		CLsingle(const CLsingle&);
	protected:
		 CLsingle() { };
	public:
		virtual ~CLsingle() { };
   		static T* instance();
};

template<class T>
T* CLsingle<T>::single = 0;

template<class T>
T* CLsingle<T>::instance()
{
	//check if existing, if not create
	if (single==0) single = new T();
	return single;
	//*
}

#endif
