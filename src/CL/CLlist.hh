///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLLIST
#define HH_CLLIST
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLutils.hh"
///*

///header
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
///*

///definitions
template<class member>
class CLlist : public CLbase<CLlist<member>,0>
{
	protected:
		struct listmember
		{
			member*			data;
			listmember*		next;
			listmember*		prev;
			xchar*			name;
		};
	
		listmember* current;
		listmember* first;
		listmember* last;
		xlong length;
	public:
		CLlist();
		~CLlist();

		xlong getlength() const { return length; };
		member* getcurrentdata() const;
		xchar* getcurrentname() const;
		void append(member* e,const xchar* n=u8" ");
		xlong delcurrent(bool smash=0); //test smash option
		bool seekdata(member* s);
		member* findbyname(const xchar* n); //for xml?
		void clear();
		void smash(); //test
		
		xlong setfirst();
		xlong setlast();
		xlong setprev();
		xlong setnext();
		bool islast() const { return (current==last); };
		bool isfirst() const { return (current==first); };
		
		void exchangesort(bool updown);
		
		void print() const;
};
///*

///implementation
template<class member>
CLlist<member>::CLlist() //! noncritical
{
	//initialize empty list
	length = 0;
	current = first = last = 0;
	//*
}

template<class member>
CLlist<member>::~CLlist() //! noncritical
{
	//delete list members data 
	//! smash(); //!test
	//
	
	//delete list members iteratively
	current = last;
	while(length!=0) { delcurrent(); }
	delete current;
	delete first;
	delete last;
	//*
}

template<class member>
void CLlist<member>::append(member* e,const xchar* n) //! noncritical
{
	//enter very first member into list
	if(length==0)
	{
		current = new listmember; //<member>
		first = current;
		last = current;
		current->data = e;
		current->next = current;
		current->prev = current;
		current->name = (xchar*)n;
	}
	//*
	
	//default append list
	else
	{
		setlast();
		current->next = new listmember; //<member>
		last = current->next;
		current->next->data = e;
		current->next->prev = current;
		current->next->next = current->next;
		current = current->next;
		current->name = (xchar*)n;
	}
	//*

	//adjust list length
	length++;
	//*
}

template<class member>
member* CLlist<member>::getcurrentdata() const //! noncritical
{
	//return current members data
	if(current!=0) { return current->data; } else { return 0; }
	//*
}

template<class member>
xchar* CLlist<member>::getcurrentname() const //! noncritical
{
	//return current members name
	if(current!=0) { return current->name; } else { return 0; }
	//*
}

template<class member>
xlong CLlist<member>::delcurrent(bool smash) //! noncritical
{
	//return if list is empty
	if(length==0) { return 0; }
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
		listmember* tempnext = current->next;
		listmember* tempprev = current->prev;
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
	
	return 1;
}

template<class member>
bool CLlist<member>::seekdata(member* s) //! critical
{
	setfirst();

	for(xlong j=0;j<length;j++)
	{
		if(current->data == s) { return 1; }
		setnext();
	}
	
	return 0;
}

template<class member>
void CLlist<member>::clear() //! noncritical
{
	//clear list
	//smash();
	length = 0;
	current = first = last = 0;
	//*
}

template<class member>
void CLlist<member>::smash() //! noncritical
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
xlong CLlist<member>::setfirst() //! noncritical
{
	//set current to first member
	current = first;
	return 0;
	//*
}

template<class member>
xlong CLlist<member>::setlast() //! noncritical
{
	//set current to last
	current = last;
	return length;
	//*
}

template<class member>
xlong CLlist<member>::setnext() //! noncritical
{
	//set next member from current
	if(length==0) { return 0; }
	current = current->next;
	return 1;
	//*
}

template<class member>
xlong CLlist<member>::setprev() //! noncritical
{
	//set previous member from current
	if(length==0) { return 0; }
	current = current->prev;
	return -1;
	//*
}

template<class member>
void CLlist<member>::exchangesort(bool updown) //! noncritical
{
	listmember* temp = 0;
	
	//sort descending
	if(updown)
	{
		for(xlong i=0; i<length-1; i++)
		{
			for(xlong j=(i+1); j<length; j++)
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
		for(xlong i=0; i<length-1; i++)
		{
			for(xlong j=(i+1); j<length; j++)
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
void CLlist<member>::print() const //! noncritical
{
	//print all list members names
	setfirst();

	say(u8"|");

	while(!islast())
	{
		tty(u8"+");
		say(getcurrentname());
		setnext();
	}
	//*
}
///*

#endif

