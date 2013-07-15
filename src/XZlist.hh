///<header>
// atCROSSLEVEL 2010,2011,2012,2013
// released under 2-clause BSD license
// XZlist.hh
// Double Linked List Library 
///</header>

///<include>
#pragma once
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
	public:
		list() : cur(0),fir(new member{0,0,0}),las(new member{0,0,0}),len(0) { fir->next=las->next=las; fir->prev=las->prev=fir; } 
		inline bool notlast() const { return cur!=las; }
		inline bool notfirst() const { return cur!=fir; }
		inline bool isfirst() const { return cur->prev==fir; }
		inline bool islast() const { return cur->next==las; }
		inline yint length() const { return len; }
		inline bool same() const { return cur==tmp; }
		inline void first() { cur = fir->next; }
		inline void last()  { cur = las->prev; }
		inline void prev()  { cur = cur->prev; }
		inline void next()  { cur = cur->next; }
		inline void save()  { tmp = cur; }
		inline void load()  { cur = tmp; }
		inline void clear() { fir->next=las->next=las; fir->prev=las->prev=fir; cur = 0; len = 0; }
		inline void append(T* x) { las->prev = las->prev->next = cur = new member{x,las,las->prev}; ++len; }
		inline T*   current() const { return cur->data; }
		inline T*   temp() const { return tmp->data; }
		       T*   delcurrent();
		       bool find(T* x);
		       void xsort(bool u);
};
///</define>

///<code>
template<typename T>
T* list<T>::delcurrent()
{
	T* c = cur->data;
	cur->next->prev = cur->prev;
	cur->prev->next = cur->next;
	//if(len!=0) { delete cur; }
	cur = cur->prev;
	len-=(len!=0);

	return c;
}

template<typename T>
bool list<T>::find(T* x)
{
	bool r = 0;
	for(first();notlast()&&x!=cur->data;next())
	{
		r = (x==cur->data) || r;
	}
	return r;
}

/*template<typename T>
void list<T>::xsort(bool u) //use swap
{
	for(yint i=1;i<len;++i)
	{
		for(yint j=(i+1);j<len;++j)
		{
			if( (cur->hash<cur->next->hash&&u) || (cur->hash>cur->next->hash&&!u) )
			{
				T* temp = cur->data;
				cur->data = cur->next->data;
				cur->next->data = temp;
			}
		}
	}
}*/
///</code>

