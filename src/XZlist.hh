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
//#include <memory>
#include "XZbasic.hh"
///*

///definition
class list
{
	private:
		struct member
		{
			void*   data;
			member* /*std::shared_ptr<member>*/ next;
			member* /*std::shared_ptr<member>*/ prev;
			sint    hash;
		};
		member* /*std::shared_ptr<member>*/ _cur;
		member* /*std::shared_ptr<member>*/ _fir;
		member* /*std::shared_ptr<member>*/ _las;
		uint    _len;
	public:
		inline list() : _cur(0),_fir(0),_las(0),_len(0) { ; }
		inline ~list() { ; }
		inline bool notlast() const { return _cur!=_las; }
		inline bool notfirst() const { return _cur!=_fir; }
		inline sint length() const { return _len; }
		inline sint first() { _cur = _fir; return 0; }
		inline sint last() { _cur = _las; return _len; }
		inline sint prev() { guard(_len==0,0); _cur = _cur->prev; /*prefetch(_cur->prev);*/ return -1; }
		inline sint next() { guard(_len==0,0); _cur = _cur->next; /*prefetch(_cur->next);*/ return  1; }
		inline void clear() { _cur = _fir = _las = 0; _len = 0; }
		inline void* current() const { guard(_len==0,0); return _cur->data; }
		       void* delcurrent();
		       void append(void* x,sint h=0);
		       void exchangesort(bool u);
		       bool find(void* t);
};
///*

///implementation
void* list::delcurrent()
{
	//return if list is empty
	guard(_len==0,0);
	//*

	void* c = _cur->data;

	//catch special cases
	switch( (_cur==_fir) - (_cur==_las) + ((_cur!=_las && _cur!=_fir && _las!=_fir)<<1) )
	{
		case -1: //del last
			_cur = _cur->prev;
			_cur->next = _las = _cur;
		break;

		case 0: //del single
			_cur = _fir = _las = 0;
		break;

		case 1: //del first
			_cur = _cur->next;
			_cur->prev = _fir = _cur;
		break;

		default: //del middle 
			_cur->next->prev = _cur->prev;
			_cur->prev->next = _cur->next;
			_cur = _cur->prev;
		break;
	}
	//*

	//adjust length
	--_len;
	//*

	return c;
}

void list::append(void* x,sint h)
{
	//very first member
	if(_len==0)
	{
		_cur = _las = _fir = /*std::shared_ptr<member>*/(new member);
		_cur->next = _cur->prev = _cur;
		_cur->data = x;
		_cur->hash = h;
	}
	//*
	
	//default append
	else
	{
		_cur = _las;
		_las = _cur->next = /*std::shared_ptr<member>*/(new member);
		_las->next = _las;
		_las->prev = _cur;
		_las->data = x;
		_las->hash = h;
		_cur = _las;
	}
	//*

	//adjust length
	++_len;
	//*
}

void list::exchangesort(bool u) //use swap
{
	for(uint i=1;i<_len;++i)
	{
		for(uint j=(i+1);j<_len;++j)
		{
			if( (_cur->hash<_cur->next->hash&&u) || (_cur->hash>_cur->next->hash&&!u) )
			{
				member* /*std::shared_ptr<member>*/ temp = _cur->next;
					
				_cur->next = temp->next;
				temp->next = _cur;
				
				temp->prev = _cur->prev;
				_cur->prev = temp;
			}
		}
	}
}

bool list::find(void* t) //test
{
	bool r = 0;

	for(uint i=first();i<_len&&t!=_cur->data;i+=next())
	{
		r = (t==_cur->data) || r;
	}

	return r;
}
///*

#endif

