//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFIFO
#define HH_CLFIFO
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"

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
		~CLfifo();
		void in(member* f);
		member* out();
		xlong getlength() const;
		void clear();
		bool isempty() const;
};

template<class member>
CLfifo<member>::CLfifo()
{
	//init empty fifo que
	length = 0;
	//*
}

template<class member>
CLfifo<member>::~CLfifo() { }

template<class member>
void CLfifo<member>::in(member* f)
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
member* CLfifo<member>::out()
{
	//is fifo empty
	if(length == 0)
	{
		return 0;
	}
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
void CLfifo<member>::clear()
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

template<class member>
xlong CLfifo<member>::getlength() const { return length; }

template<class member>
bool CLfifo<member>::isempty() const { return (length==0); }

#endif

