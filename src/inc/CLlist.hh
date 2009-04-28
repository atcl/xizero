//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLIST
#define HH_CLLIST
#pragma message "Compiling " __FILE__ " ! TODO: all" 

#include "CLtypes.hh"
#include "CLcl.hh"


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

		void append(void* e);
		void* getcurrent();
		void delcurrent(bool smash);
		xlong getlength();
		void settoindex(xlong i);
		void clear();
		void smash();
		void setfirst();
		void setlast();
		void setprev();
		void setnext();
		bool islast();
		bool isfirst();
};

CLlist::CLlist()
{
	length = 0;
	current = 0;
	first = 0;
	last = 0;
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

CLlist::~CLlist() { }

void CLlist::append(void* e)
{
	if(length==0)
	{
		length++;
		current = new listmember;
		first = current;
		last = current;
		current->data = e;
		current->next = current;
		current->prev = current;
	}
	else
	{
		length++;
		listmember* temp = new listmember;
		setlast();
		current->next = temp;
		last = temp;
		temp->prev = current;
		temp->next = temp;
		current = temp;
	}
}

void* CLlist::getcurrent()
{
	return current->data;
}

void CLlist::delcurrent(bool smash)
{
	if(smash==true) delete current->data;

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
}

xlong CLlist::getlength()
{
	return length;
}

void CLlist::settoindex(xlong i)
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
		delete current->data;
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

#endif

