///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLFIFO
#define HH_CLFIFO
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
///*

///header
/* class name:	CLfifo
 * 
 * description:	A first-in-first-out type
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
template<class T>
struct fifomember
{
	T*          data;
	fifomember* prev;
};

template<class T>
class CLfifo : public CLbase<CLfifo<T>,0>
{
	protected:
		fifomember<T>* head;
		fifomember<T>* tail;
		xlong length;
		T** que;
	public:
		CLfifo() { length = 0; };
		~CLfifo() { delete[] que; };
		xlong getlength() const { return length; };
		bool isempty() const { return (length==0); };
		void in(T* f);
		T* out();
		void clear();
};
///*

///implementation
template<class T>
void CLfifo<T>::in(T* f) //! critical
{
	//create empty fifo member
	fifomember<T>* t = new fifomember<T>;
	//*

	//place first fifo member
	if(length == 0) { tail = t; }
	//
	
	//place default fifo member
	else { head->prev = head = t; }
	//*

	//set other fifo member attributes
	t->data = f;
	t->prev = 0;
	head = t;
	length++;
	//*
}

template<class T>
T* CLfifo<T>::out() //! critical
{
	//is fifo empty
	if(length == 0) { return 0; }
	//*
	
	//is fifo almost empty
	else if(length==1)
	{
		length--;
		return tail->data;
	}
	//*
	
	//default deque
	else
	{
		length--;
		T* t = tail->data;
		tail  = tail->prev;
		return t;
	}
	//*
}

template<class T>
void CLfifo<T>::clear() //! critical
{
	//clear complete fifo que
	for(uxlong i=0; i<length; i++)
	{
		fifomember<T>* t = tail;
		tail = tail->prev;
		//delete t;
		length--;
	}
	//*
}
///*

#endif

