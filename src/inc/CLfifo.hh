//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFIFO
#define HH_CLFIFO
#warning "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


class CLfifo : public virtual CLcl
{
	private:
		listmember* head;
		listmember* tail;
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
	listmember* t = new listmember;

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
	t->next = 0;
	t->prev = 0;
	t->name = 0;
	t->hash = 0;
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
		listmember* t = tail;
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

