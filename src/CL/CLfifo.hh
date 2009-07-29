//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFIFO
#define HH_CLFIFO
//#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


struct fifomember
{
	void*       data;
	fifomember* prev;
};

class CLfifo : public virtual CLcl
{
	private:
		fifomember* head;
		fifomember* tail;
		xlong length;
		void** que;

	public:
		CLfifo();
		~CLfifo();
		void in(void* f);
		void* out();
		xlong getlength();
		void clear();
		bool isempty();
};


CLfifo::CLfifo()
{
	length = 0;
}

CLfifo::~CLfifo() { }

void CLfifo::in(void* f)
{
	fifomember* t = new fifomember;

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

void* CLfifo::out()
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

void CLfifo::clear()
{
	for(int i=0; i<length; i++)
	{
		fifomember* t = tail;
		tail = tail->prev;
		delete t;
		length--;
	}
}

xlong CLfifo::getlength()
{
	return length;
}

bool CLfifo::isempty()
{
	if(length==0) return true;

	return false;
}

#endif

