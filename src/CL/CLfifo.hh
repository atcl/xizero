//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFIFO
#define HH_CLFIFO
//#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"

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
		xlong getlength();
		void clear();
		bool isempty();
};

template<class member>
CLfifo<member>::CLfifo()
{
	length = 0;
}

template<class member>
CLfifo<member>::~CLfifo() { }

template<class member>
void CLfifo<member>::in(member* f)
{
	fifomember<member>* t = new fifomember<member>;

	if(length == 0)
	{
		tail = t;
	}
	else
	{
		head->prev = t;
		head = t;
	}

	t->data = f;
	t->prev = 0;
	head = t;
	length++;
}

template<class member>
member* CLfifo<member>::out()
{
	if(length == 0)
	{
		return 0;
	}
	else if(length==1)
	{
		length--;
		return tail->data;
	}
	else
	{
		length--;
		void* t = tail->data;
		tail  = tail->prev;
		return t;
	}
}

template<class member>
void CLfifo<member>::clear()
{
	for(uxlong i=0; i<length; i++)
	{
		fifomember<member>* t = tail;
		tail = tail->prev;
		delete t;
		length--;
	}
}

template<class member>
xlong CLfifo<member>::getlength()
{
	return length;
}

template<class member>
bool CLfifo<member>::isempty()
{
	if(length==0) return true;

	return false;
}

#endif

