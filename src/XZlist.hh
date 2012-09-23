///<header>
// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZlist.hh
// Double Linked List Library 
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: "
///</header>

///<include>
#include "XZbasic.hh"
///</include>

///<define>
class list
{
	private:
		struct member
		{
			void*   data;
			member* next;
			member* prev;
			sint    hash;
		};
		member* cur;
		member* fir;
		member* las;
		uint    len;
	public:
		list() : cur(0),fir(new member{0,0,0,0}),las(new member{0,0,0,0}),len(0) { fir->next=las->next=las; fir->prev=las->prev=fir; }
		~list() { delete cur; delete fir; delete las; }
		inline bool notlast() const { return cur!=las; }
		inline bool notfirst() const { return cur!=fir; }
		inline uint length() const { return len; }
		inline void first() { cur = fir->next; }
		inline void last()  { cur = las->prev; }
		inline void prev()  { cur = cur->prev; }
		inline void next()  { cur = cur->next; }
		inline void clear() { fir->next=las->next=las; fir->prev=las->prev=fir; cur = 0; len = 0; }
		inline void append(void* x,sint h=0) { las->prev = las->prev->next = cur = new member{x,las,las->prev,h}; ++len; }
		inline void* current() const { return cur->data; }
		       void* delcurrent();
		       bool find(void* x);
		       void xsort(bool u);
};
///</define>

///<code>
void* list::delcurrent()
{
	void* c = cur->data;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	//if(len!=0) { delete cur; }
	cur = cur->prev;
	len-=(len!=0);

	return c;
}

bool list::find(void* x)
{
	bool r = 0;
	for(first();notlast()&&x!=cur->data;next())
	{
		r = (x==cur->data) || r;
	}
	return r;
}

void list::xsort(bool u) //use swap
{
	for(uint i=1;i<len;++i)
	{
		for(uint j=(i+1);j<len;++j)
		{
			if( (cur->hash<cur->next->hash&&u) || (cur->hash>cur->next->hash&&!u) )
			{
				void* temp = cur->data;
				cur->data = cur->next->data;
				cur->next->data = temp;
				//sint temp = cur->hash;
				//cur->hash = cur->next->hash;
				//cur->next->hash = hemp;
			}
		}
	}
}
///</code>

