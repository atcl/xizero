//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLINIT
#define HH_CLINIT
#pragma message "Compiling " __FILE__ " ! TODO: "

template<class T>
T* CLinit(T* singleton)
{
	return T::instance();
}

#endif
