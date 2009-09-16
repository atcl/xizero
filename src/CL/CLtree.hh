//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLTREE
#define HH_CLTREE
#pragma message "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"

template<class member>
struct node
{
	node*  parent;
	node*  next;
	node*  prev;
	node** child;
	xlong  childcount;
	member*  data;
	xchar* name;
	xlong* hash;
};

template<class member>
class CLtree : public virtual CLcl
{
	private:
		node<member>* rootnode;
		node<member>* current;

	public:
		CLtree();
		~CLtree();

		void root();
		void child(xlong i);
		xlong childcount();
		void parent();
		void next();	//next sibling
		member* data();
		void adddata(member* d);
		void addchild();
		void delchild(xlong i);
		bool isroot();
		void print();
};

template<class member>
CLtree<member>::CLtree()
{
	rootnode = new node<member>;
	rootnode->parent = 0;
	rootnode->childcount = 0;

	current = rootnode;
}

template<class member>
CLtree<member>::~CLtree()
{
	delete rootnode;
}

template<class member>
void CLtree<member>::root()
{
	current = rootnode;
}

template<class member>
void CLtree<member>::child(xlong i)
{
	if(i<current->childcount)
	{
		current = current->child[i];
	}
}

template<class member>
xlong CLtree<member>::childcount()
{
	return current->childcount;
}

template<class member>
void CLtree<member>::parent()
{
	current = current->parent;
}

template<class member>
void CLtree<member>::next()
{
	if(current->next!=0)
	{
		current = current->next;
	}
}

template<class member>
member* CLtree<member>::data()
{
	return current->data;
}

template<class member>
void CLtree<member>::adddata(member* d)
{
	current->data = d;
}

template<class member>
void CLtree<member>::addchild()
{
	node<member>* newnode = new node<member>;

	newnode->parent = current;
	newnode->next = newnode;
	if(current->childcount==0)
	{
		newnode->prev = newnode;
	}
	else
	{
		newnode->prev = current->child[current->childcount-1];
	}
	newnode->child = 0;
	newnode->childcount = 0;
	newnode->data = 0;

	current->childcount++;

	

}

template<class member>
void CLtree<member>::delchild(xlong i)
{
	if(i >= current->childcount)
	{
		delete current->child[i-1]->next;
		current->child[i-1]->next = current->child[i-1];
	}
}

template<class member>
bool CLtree<member>::isroot()
{
	if(current==rootnode) return true;
	else return false;
}

template<class member>
void CLtree<member>::print()
{
	if(current->name!=0) CLsystem::print(current->name);
	else CLsystem::print(0);
	for(uxlong i=0; i<current->childcount; i++)
	{
		current = current->child[i];
		print();
	}
	current = current->parent;
}

#endif

