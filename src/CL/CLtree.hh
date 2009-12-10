///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLTREE
#define HH_CLTREE
///*

///includes
#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"
///*

///header
/* class name:	CLtree
 * 
 * description:	A tree type
 * 
 * author:	atcl
 * 
 * notes:	test.
 * 
 * version: 0.1
 */
///*

///definitions
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
		member* data() const;
		void adddata(member* d);
		void addchild();
		void delchild(xlong i);
		bool isroot() const;
		void print() const;
};
///*

///implementation
template<class member>
CLtree<member>::CLtree() //! noncritical
{
	rootnode = new node<member>;
	rootnode->parent = 0;
	rootnode->childcount = 0;

	current = rootnode;
}

template<class member>
CLtree<member>::~CLtree() { delete rootnode; } //! noncritical

template<class member>
void CLtree<member>::root() { current = rootnode; } //! noncritical

template<class member>
void CLtree<member>::child(xlong i) { if(i<current->childcount) { current = current->child[i]; } }  //! noncritical

template<class member>
xlong CLtree<member>::childcount() { return current->childcount; } //! noncritical

template<class member>
void CLtree<member>::parent() { current = current->parent; }  //! noncritical

template<class member>
void CLtree<member>::next()
{ if(current->next!=0) { current = current->next;	} }  //! noncritical

template<class member>
member* CLtree<member>::data() const { return current->data; } //! noncritical

template<class member>
void CLtree<member>::adddata(member* d) { current->data = d; } //! noncritical

template<class member>
void CLtree<member>::addchild() //! noncritical
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
void CLtree<member>::delchild(xlong i) //! noncritical
{
	if(i >= current->childcount)
	{
		delete current->child[i-1]->next;
		current->child[i-1]->next = current->child[i-1];
	}
}

template<class member>
bool CLtree<member>::isroot() const { return (current==rootnode); } //! noncritical

template<class member>
void CLtree<member>::print() const //! noncritical
{
	if(current->name!=0) clsystem->print(current->name);
	else clsystem->print(0);
	for(uxlong i=0; i<current->childcount; i++)
	{
		current = current->child[i];
		print();
	}
	current = current->parent;
}
///*

#endif

