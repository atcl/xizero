///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLSINGLE
#define HH_CLSINGLE
///*

///includes
#include "CLtypes.hh"
///*

///header
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
///*

///definitions
template<typename T,class clsingle>
class CLsinglebase
{
	public:
		static T* instance() { static_cast<clsingle*>(&CLsinglebase::instance)->instance(); };
};

template<class T>
class CLsingle : public CLsinglebase<T,CLsingle<T> >
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
///*

///implementation
template<class T>
T* CLsingle<T>::instance() //! noncritical
{
	//check if existing, if not create
	if (single==0) single = new T();
	return single;
	//*
}
///*

#endif
