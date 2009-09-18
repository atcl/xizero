//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLLIST
#define HH_CLLIST
#pragma message "Compiling " __FILE__ " ! TODO: all" 

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLsystem.hh"

/* class name:	CLlist
 * 
 * description:	A double-linked-list type
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */

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
		~CLlist();

		void append(member* e,const xchar* n=" ",xlong h=0);
		member* getcurrentdata() const;
		xchar* getcurrentname() const;
		void delcurrent(bool smash=0); //test smash option
		xlong getlength() const;
		void setindex(xlong i);
		xlong getindex();
		void clear();
		void smash(); //test
		xlong setfirst();
		xlong setlast();
		xlong setprev();
		xlong setnext();
		bool islast() const;
		bool isfirst() const;
		void exchangesort(bool updown);
		void print() const;
};

template<class member>
CLlist<member>::CLlist()
{
	//initialize empty list
	length = 0;
	current = 0;
	first = 0;
	last = 0;
	//*
}

template<class member>
CLlist<member>::~CLlist()
{
	//delete list members data 
	//! smash(); //!test
	//
	
	//delete list members iteratively
	current = last;
	while(length!=0)
	{
		delcurrent();
	}
	delete current;
	delete first;
	delete last;
	//*
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
	
	//enter very first member into list
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
	//*
	
	//default append list
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
	//*

	//adjust list length
	length++;
	//*
}

template<class member>
member* CLlist<member>::getcurrentdata() const
{
	//return current members data
	if(current!=0) return current->data;
	//*
	
	//return zero if list is empty
	else return 0;
	//*
}

template<class member>
xchar* CLlist<member>::getcurrentname() const
{
	//return current members name
	if(current!=0) return current->name;
	//*
	
	//return zero if list is empty
	else return 0;
	//*
}

template<class member>
void CLlist<member>::delcurrent(bool smash)
{
	//return if list is empty
	if(length==0) return;
	//*

	//if smash delete current members data pointer
	member* temp = current->data;
	//! if(smash) delete temp; //!test
	//*

	//delete very (l)only element in list
	if(current==last && current==first)
	{
		delete current;
		current = 0;
		first = 0;
		last = 0;
	}
	//*
	
	//delete last member
	else if(current==last)
	{
		setprev();
		delete current->next;
		current->next = current;
		last = current;
	}
	//*
	
	//delete first member
	else if(current==first)
	{
		setnext();
		delete current->prev;
		current->prev = current;
		first = current;
	}
	//*
	
	//delete some member in the middle
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
	//*

	//adjust list length
	length--;
	//*
	
	return;
}

template<class member>
xlong CLlist<member>::getlength() const { return length; }

template<class member>
void CLlist<member>::setindex(xlong i)
{
	//setting current member to index position (SLOW)
	if(i<=length)
	{
		setfirst();

		for(uxlong j=0;j<i;j++)
		{
			setnext();
		}
	}
	//*
}

template<class member>
xlong CLlist<member>::getindex()
{
	//get index of current member (VERY SLOW)
	xlong i = 0;
	
	while(current!=first)
	{
		i++;
		setprev();
	}
	setindex(i);
	//*

	return i;
}

template<class member>
void CLlist<member>::clear()
{
	//clear list
	//smash();
	length = 0;
	current = 0;
	first = 0;
	last = 0;
	//*
}

template<class member>
void CLlist<member>::smash()
{
	//delete all list members data
	current = first;
	while(current!=last);
	{
		//delete current->data;
		setnext();
		delete current->prev;
	}
	//*
}

template<class member>
xlong CLlist<member>::setfirst()
{
	//set current to first member
	current = first;
	return 0;
	//*
}

template<class member>
xlong CLlist<member>::setlast()
{
	//set current to last
	current = last;
	return length;
	//*
}

template<class member>
xlong CLlist<member>::setnext()
{
	//set next member from current
	if(length!=0) current = current->next;
	return 1;
	//*
}

template<class member>
xlong CLlist<member>::setprev()
{
	//set previous member from current
	if(length!=0) current = current->prev;
	return -1;
	//*
}

template<class member>
bool CLlist<member>::isfirst() const
{
	//check if current member is first
	return (current==first);
	//*
}

template<class member>
bool CLlist<member>::islast() const
{
	//check if current member is last
	return (current==last);
	//*
}

template<class member>
void CLlist<member>::exchangesort(bool updown)
{
	listmember<member>* temp = 0;
	
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
void CLlist<member>::print() const
{
	//print all list members names
	setfirst();

	clsystem->print("|");

	while(!islast())
	{
		clsystem->print("+",0);
		clsystem->print(getcurrentname());
		setnext();
	}
	//*
}

#endif

