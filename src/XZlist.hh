// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZlist.hh
// Double Linked List Library 

///guard
#ifndef HH_XZLIST
#define HH_XZLIST
//#pragma message "Compiling " __FILE__ "..." " TODO: "
///*

///includes
#include "XZbasic.hh"
///*

///definition
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
		inline list() : cur(0),fir(new member{0,0,0,0}),las(new member{0,0,0,0}),len(0) { fir->next=las->next=las; fir->prev=las->prev=fir; }
		inline ~list() { delete fir; delete las; }
		inline bool notlast() const { return cur!=las; }
		inline bool notfirst() const { return cur!=fir; }
		inline sint length() const { return len; }
		inline void first() { cur = fir->next; }
		inline void last() { cur = las->prev; }
		inline void prev() { cur = cur->prev; /*prefetch(cur->prev);*/ }
		inline void next() { cur = cur->next; /*prefetch(cur->next);*/ }
		inline void clear() { cur = fir = las = 0; len = 0; }
		inline void* current() const { guard(len==0,0); return cur->data; }
		       void* delcurrent();
		       void append(void* x,sint h=0);
		       bool find(void* x);
		       void exchangesort(bool u);
};
///*

///implementation
void* list::delcurrent()
{
	//return if list is empty
	guard(len==0,0);
	//*

	void* c = cur->data;

	//catch special cases
	switch( (cur==fir) - (cur==las) + ((cur!=las && cur!=fir && las!=fir)<<1) )
	{
		case -1: //del last
			las->prev = cur->prev;
			las->prev->next = las;
		break;

		case 0: //del single
			cur = fir = las = 0;
		break;

		case 1: //del first
			fir->next = cur->next;
			fir->next->prev = fir;
		break;

		default: //del middle 
			cur->next->prev = cur->prev;
			cur->prev->next = cur->next;
			cur = cur->prev;
		break;
	}
	//*

	//adjust length
	--len;
	//*

	return c;
}

void list::append(void* x,sint h)
{
	las->prev = las->prev->next = cur = new member{x,las,las->prev,h};

	//adjust length
	++len;
	//*
}

bool list::find(void* x) //test
{
	bool r = 0;

	for(first();notlast()&&x!=cur->data;next())
	{
		r = (x==cur->data) || r;
	}

	return r;
}

void list::exchangesort(bool u) //use swap
{
	for(uint i=1;i<len;++i)
	{
		for(uint j=(i+1);j<len;++j)
		{
			if( (cur->hash<cur->next->hash&&u) || (cur->hash>cur->next->hash&&!u) )
			{
				member* temp = cur->next;
					
				cur->next  = temp->next;
				temp->next = cur;
				
				temp->prev = cur->prev;
				cur->prev  = temp;
			}
		}
	}
}
///*

#endif

