///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLFIFO
#define HH_CLFIFO
///*

///includes
#include "CLtypes.hh"
#include "CLcl.hh"
///*

///header
/* class name:	CLfifo
 * 
 * description:	A first-in-first-out type
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
template<class member>
struct fifomember
{
	member*     data;
	fifomember* prev;
};

template<class member>
class CLfifo : public virtual CLcl
{
	private:
		fifomember<member>* head;
		fifomember<member>* tail;
		xlong length;
		member** que;
	public:
		CLfifo();
		~CLfifo() { }
		void in(member* f);
		member* out();
		xlong getlength() const { return length; };
		void clear();
		bool isempty() const { return (length==0); };
};
///*

///implementation
template<class member>
CLfifo<member>::CLfifo() //! noncritical
{
	//init empty fifo que
	length = 0;
	//*
}

template<class member>
void CLfifo<member>::in(member* f) //! critical
{
	//create empty fifo member
	fifomember<member>* t = new fifomember<member>;
	//*

	//place first fifo member
	if(length == 0)
	{
		tail = t;
	}
	//
	
	//place default fifo member
	else
	{
		head->prev = t;
		head = t;
	}
	//*

	//set other fifo member attributes
	t->data = f;
	t->prev = 0;
	head = t;
	length++;
	//*
}

template<class member>
member* CLfifo<member>::out() //! critical
{
	//is fifo empty
	if(length == 0) return 0;
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
		member* t = tail->data;
		tail  = tail->prev;
		return t;
	}
	//*
}

template<class member>
void CLfifo<member>::clear() //! critical
{
	//clear complete fifo que
	for(uxlong i=0; i<length; i++)
	{
		fifomember<member>* t = tail;
		tail = tail->prev;
		delete t;
		length--;
	}
	//*
}
///*

#endif

