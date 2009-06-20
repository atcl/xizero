//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLIST
#define HH_CLLIST
#pragma message "Compiling " __FILE__ " ! TODO: all" 

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLapi.hh"


class CLlist : public virtual CLcl
{
	private:
		listmember* current;
		listmember* first;
		listmember* last;
		xlong length;

	public:
		CLlist();
		CLlist(void* e);
		~CLlist();

		void append(void* e,const xchar* n);
		void* getcurrentdata();
		xchar* getcurrentname();
		void* delcurrent(bool smash); //test smash option
		xlong getlength();
		void setindex(xlong i);
		xlong getindex();
		void clear();
		void smash(); //test
		void setfirst();
		void setlast();
		void setprev();
		void setnext();
		bool islast();
		bool isfirst();
		void radixsort();
		void print();
};

CLlist::CLlist()
{
	length = 0;
	current = 0;
	first = 0;
	last = 0;
}

CLlist::~CLlist()
{
	delete current;
}

CLlist::CLlist(void* e)
{
	length = 1;
	current = new listmember;
	first = current;
	last = current;
	current->data = e;
	current->next = current;
	current->prev = current;
}

void CLlist::append(void* e,const xchar* n)
{
	if(length==0)
	{
		current = new listmember;
		first = current;
		last = current;
		current->data = e;
		current->next = current;
		current->prev = current;
		current->name = (xchar*)n;
	}
	else
	{
		setlast();
		current->next = new listmember;
		last = current->next;
		current->next->data = e;
		current->next->prev = current;
		current->next->next = current->next;
		current = current->next;
		current->name = (xchar*)n;
	}

	length++;
}

void* CLlist::getcurrentdata()
{
	if(current!=0)
	{
		return current->data;
	}
	else
	{
		return 0;
	}
}

xchar* CLlist::getcurrentname()
{
	if(current!=0)
	{
		return current->name;
	}
	else
	{
		return 0;
	}
}

void* CLlist::delcurrent(bool smash)
{
	if(length==0) return 0;

	if(current==last)
	{
		setprev();
		delete current->next;
		current->next = current;
		last = current;
	}
	else if(current==first)
	{
		setnext();
		delete current->prev;
		current->prev = current;
		first = current;
	}
	else
	{
		listmember* tempnext = current->next;
		listmember* tempprev = current->prev;
		setprev();
		delete current->next;
		current->next = tempnext;
		setnext();
		current->prev = tempprev;
	}

	length--;

	if(smash==true) return current->data;
}

xlong CLlist::getlength()
{
	return length;
}

void CLlist::setindex(xlong i)
{
	if(i<=length)
	{
		setfirst();

		for(int j=0;j<i;j++)
		{
			setnext();
		}
	}
}

xlong CLlist::getindex()
{
	xlong i = 0;

	while(current!=first)
	{
		i++;
		setprev();
	}

	setindex(i);

	return i;
}

void CLlist::clear()
{
	length = 0;
	current = 0;
	first = 0;
	last = 0;
}

void CLlist::smash()
{
	current = first;
	while(current!=last);
	{
		//delete current->data;
		setnext();
		delete current->prev;
	}
	
}

void CLlist::setfirst()
{
	current = first;
}

void CLlist::setlast()
{
	current = last;
}

void CLlist::setnext()
{
	current = current->next;
}

void CLlist::setprev()
{
	current = current->prev;
}

bool CLlist::isfirst()
{
	if(current==first) return true;
	else return false;
}

bool CLlist::islast()
{
	if(current==last) return true;
	else return false;
}

void CLlist::radixsort()
{

}

void CLlist::print()
{
	setfirst();

	CLprint_("|");

	while(!islast())
	{
		CLttyout_("+");
		CLprint_(getcurrentname());
		setnext();
	}
}

#endif

