//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFIFO
#define HH_CLFIFO
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"


class CLfifo : public virtual CLcl
{
	private:
		xlong head;
		xlong tail;
		xlong length;
		xlong current;
		void** que;

	public:
		CLfifo(xlong l);
		~CLfifo();
		void in(void* f);
		void* out();
		void clear();
		bool isfull();
		bool isempty();
};

CLfifo::CLfifo(xlong l)
{
	length = l;
	current = -1;
	que = new void*[l];
}

CLfifo::~CLfifo() { }

void CLfifo::in(void* f)
{
	if(current==-1)
	{
		current = 0;
		que[current] = f;
		head = current;
		tail = current;
	}
	else if(isfull()==false)
	{
		
	}
}

void* CLfifo::out()
{
	void* t = 0;

	if(isempty()==false)
	{
		if(tail!=head)
		{
			
		}
		else
		{
			
		}
	}

	return t;
}

void CLfifo::clear()
{

}

bool CLfifo::isfull()
{

}

bool CLfifo::isempty()
{

}

#endif

