//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLIST
#define HH_CLLIST
//#pragma message "Compiling " __FILE__ " ! TODO: all" 

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLapi.hh"


//make template version
struct listmember
{
	void*       data;
	listmember* next;
	listmember* prev;
	xchar*      name;
	xlong       hash;
};

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

		void append(void* e,const xchar* n=" ",xlong h=0);
		void* getcurrentdata();
		xchar* getcurrentname();
		void* delcurrent(bool smash); //test smash option
		xlong getlength();
		void setindex(xlong i);
		xlong getindex();
		void clear();
		void smash(); //test
		xlong setfirst();
		xlong setlast();
		xlong setprev();
		xlong setnext();
		bool islast();
		bool isfirst();
		void exchangesort(bool updown);
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

void CLlist::append(void* e,const xchar* n,xlong h)
{
	//move these out of the if's
	//~ current = new listmember;
	//~ current->data = e;
	//~ current->name = (xchar*)n;
	//~ current->hash = h;
	//*
	
	if(length==0)
	{
		current = new listmember;
		first = current;
		last = current;
		current->data = e;
		current->next = current;
		current->prev = current;
		current->name = (xchar*)n;
		current->hash = h;
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
		current->hash = h;
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

	void* temp = current->data;

	if(current==last && current==first)
	{
		delete current;
		current = 0;
		first = 0;
		last = 0;
	}
	else if(current==last)
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
		setnext();
		delete current->prev;
		current->prev = tempprev;
		setprev();
		current->next = tempnext;
	}

	length--;
	
	if(smash==true) return temp;
	return 0;
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

		for(uxlong j=0;j<i;j++)
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

xlong CLlist::setfirst()
{
	current = first;
	return 0;
}

xlong CLlist::setlast()
{
	current = last;
	return length;
}

xlong CLlist::setnext()
{
	if(length!=0) current = current->next;
	return 1;
}

xlong CLlist::setprev()
{
	if(length!=0) current = current->prev;
	return -1;
}

bool CLlist::isfirst()
{
	if(current==first) return true; //return current-first
	else return false;
}

bool CLlist::islast()
{
	if(current==last) return true; //return current-first
	else return false;
}

void CLlist::exchangesort(bool updown)
{
	listmember* temp;
	
	//sort descending
	if(updown)
	{
		for(uxlong i=0; i<length-1; i++)
		{
			for(uxlong j=(i+1); j<length; j++)
			{
				if(current->hash<current->next->hash)
				{
					temp = current->next;
					
					current->next = temp->next;
					temp->next = current;
					
					temp->prev = current->prev;
					current->prev = temp;
				}
			}
		}
	}
	//*
	
	//sort ascending
	else
	{
		for(uxlong i=0; i<length-1; i++)
		{
			for(uxlong j=(i+1); j<length; j++)
			{
				if(current->hash>current->next->hash)
				{
					temp = current->next;
					
					current->next = temp->next;
					temp->next = current;
					
					temp->prev = current->prev;
					current->prev = temp;
				}
			}
		}
	}
	//*
}

void CLlist::print()
{
	setfirst();

	CLsystem::print("|");

	while(!islast())
	{
		CLsystem::print("+",0);
		CLsystem::print(getcurrentname());
		setnext();
	}
}

#endif

