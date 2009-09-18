//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLSINGLE
#define HH_CLSINGLE
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"

//add static polymorphism?

template<class T>
class CLsingle
{
	private:
		static T* single;
		CLsingle(const CLsingle&);
	protected:
		 virtual ~CLsingle();
		 CLsingle();
	public:
   		static T* instance();
};

template<class T>
T* CLsingle<T>::single = 0;

template<class T>
CLsingle<T>::~CLsingle() { }

template<class T>
CLsingle<T>::CLsingle() { }

template<class T>
T* CLsingle<T>::instance()
{
	//check if existing, if not create
	if (single==0) single = new T();
	return single;
	//*
}

#endif
