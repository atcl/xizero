///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLTREE
#define HH_CLTREE
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
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
class CLtree : public CLbase<CLtree<member>,0>
{
	protected:
		struct node
		{
			node*  parent;
			node*  next;
			node*  prev;
			node** child;
			xlong  childcount;
			member*  data;
			xchar* name;
		};
	
		node* rootnode;
		node* current;
	public:
		CLtree();
		~CLtree();

		void setroot() { current = rootnode; };
		bool isroot() const { return (current==rootnode); };
		
		void addchild();
		void delchild(xlong i);
		void setchild(xlong i);
		void setfirstchild();
		void setlastchild();
		xlong getchildcount() const { return current->childcount; };
		
		void setparent() { current = current->parent; };
		
		void addsibling();
		void delcurrentsibling();
		xlong setfirstsibling();
		xlong setlastsibling();
		xlong setnextsibling();
		xlong setprevsibling();
		xlong getsiblingcount() const { return current->parrent->childcount; };
		bool isfirstsibling() const;
		bool islastsibling() const;
	
		void adddata(member* d);
		member* getdata() const { return current->data; };
		
		void print() const;
};
///*

///implementation
template<class member>
CLtree<member>::CLtree() //! noncritical
{
	rootnode = new node;
	rootnode->parent = 0;
	rootnode->childcount = 0;

	current = rootnode;
}

template<class member>
CLtree<member>::~CLtree() { /*delete rootnode;*/ } //! noncritical

template<class member>
void CLtree<member>::adddata(member* d) { current->data = d; } //! noncritical

template<class member>
void CLtree<member>::addchild() //! noncritical
{
	node* newnode = new node;

	newnode->parent = current;
	newnode->next = newnode;
	if(current->childcount==0) { newnode->prev = newnode; }
	else { newnode->prev = current->child[current->childcount-1]; }
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
		//delete current->child[i-1]->next;
		current->child[i-1]->next = current->child[i-1];
	}
}

template<class member>
void CLtree<member>::print() const //! noncritical
{
	setroot();
	
	if(current->name!=0) say(current->name);
	else say(0);
	for(uxlong i=0; i<current->childcount; i++)
	{
		current = current->child[i];
		say("...");
	}
	current = current->parent;
}
///*

#endif

