//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLIST
#define HH_CLLIST
//#pragma message "Compiling " __FILE__ " ! TODO: all" 

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLapi.hh"


template<class member>
struct listmember
{
	member*			data;
	listmember*		next;
	listmember*		prev;
	xchar*			name;
	xlong			hash;
};

template<class member>
class CLlist : public virtual CLcl
{
	private:
		listmember<member>* current;
		listmember<member>* first;
		listmember<member>* last;
		xlong length;

	public:
		CLlist();
		CLlist(member* e);
		~CLlist();

		void append(member* e,const xchar* n=" ",xlong h=0);
		member* getcurrentdata();
		xchar* getcurrentname();
		member* delcurrent(bool smash); //test smash option
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

template<class member>
CLlist<member>::CLlist()
{
	length = 0;
	current = 0;
	first = 0;
	last = 0;
}

template<class member>
CLlist<member>::~CLlist()
{
	delete current;
}

template<class member>
CLlist<member>::CLlist(member* e)
{
	length = 1;
	current = new listmember<member>;
	first = current;
	last = current;
	current->data = e;
	current->next = current;
	current->prev = current;
}

template<class member>
void CLlist<member>::append(member* e,const xchar* n,xlong h)
{
	//move these out of the if's
	//~ current = new listmember;
	//~ current->data = e;
	//~ current->name = (xchar*)n;
	//~ current->hash = h;
	//*
	
	if(length==0)
	{
		current = new listmember<member>; //<member>
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
		current->next = new listmember<member>; //<member>
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

template<class member>
member* CLlist<member>::getcurrentdata()
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

template<class member>
xchar* CLlist<member>::getcurrentname()
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

template<class member>
member* CLlist<member>::delcurrent(bool smash)
{
	if(length==0) return 0;

	member* temp = current->data;

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
		listmember<member>* tempnext = current->next;
		listmember<member>* tempprev = current->prev;
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

template<class member>
xlong CLlist<member>::getlength()
{
	return length;
}

template<class member>
void CLlist<member>::setindex(xlong i)
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

template<class member>
xlong CLlist<member>::getindex()
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

template<class member>
void CLlist<member>::clear()
{
	length = 0;
	current = 0;
	first = 0;
	last = 0;
}

template<class member>
void CLlist<member>::smash()
{
	current = first;
	while(current!=last);
	{
		//delete current->data;
		setnext();
		delete current->prev;
	}
	
}

template<class member>
xlong CLlist<member>::setfirst()
{
	current = first;
	return 0;
}

template<class member>
xlong CLlist<member>::setlast()
{
	current = last;
	return length;
}

template<class member>
xlong CLlist<member>::setnext()
{
	if(length!=0) current = current->next;
	return 1;
}

template<class member>
xlong CLlist<member>::setprev()
{
	if(length!=0) current = current->prev;
	return -1;
}

template<class member>
bool CLlist<member>::isfirst()
{
	if(current==first) return true; //return current-first
	else return false;
}

template<class member>
bool CLlist<member>::islast()
{
	if(current==last) return true; //return current-first
	else return false;
}

template<class member>
void CLlist<member>::exchangesort(bool updown)
{
	listmember<member>* temp;
	
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

template<class member>
void CLlist<member>::print()
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

