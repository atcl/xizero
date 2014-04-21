///<header>
// Ξ0 - xizero ( Version 0.2 ) 
// atCROSSLEVEL 2010-2014 ( http://atcrosslevel.de )
// released under 2-clause BSD license
// Double Linked List Class ( XZlist.hh )
#pragma once
///</header>

///<include>
#include "XZbasic.hh"
///</include>

///<define>
template<typename T>
class list
{
	private:
		struct member
		{
			T*      data;
			member* next;
			member* prev;
		};
		member* cur;
		member* fir;
		member* las;
		member* tmp;
		yint    len;

		//list(const list& l);
	public:
		list() : cur(0),fir(new member{0,0,0}),las(new member{0,0,0}),tmp(0),len(0) { fir->next=las->next=las; fir->prev=las->prev=fir; } 
		inline bool notlast()  const { return cur!=las; }
		inline bool notfirst() const { return cur!=fir; }
		inline bool isfirst()  const { return cur->prev==fir; }
		inline bool islast()   const { return cur->next==las; }
		inline yint length()   const { return len; }
		inline bool same()     const { return cur==tmp; }
		inline void first() { cur = fir->next; }
		inline void last()  { cur = las->prev; }
		inline void prev()  { cur = cur->prev; }
		inline void next()  { cur = cur->next; }
		inline void save()  { tmp = cur; }
		inline void load()  { cur = tmp; }
		inline void clear() { fir->next=las->next=las; fir->prev=las->prev=fir; cur = 0; len = 0; }
		inline void append(T* x) { las->prev = las->prev->next = cur = new member{x,las,las->prev}; ++len; }
		inline T*   current() const { return cur->data; }
		       T*   delcurrent(bool s=0);
};
///</define>

///<code>
template<typename T>
T* list<T>::delcurrent(bool s)
{
	T* c = cur->data;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	cur = cur->prev;
	len-=(len!=0);

	if(s) { delete c; return 0; } else return c;
}
///</code>

