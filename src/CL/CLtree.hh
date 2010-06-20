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
///*

///header
/* class name:	CLtree
 * 
 * description:	A tree type
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
template<class member>
class CLtree : public CLbase<CLtree<member>,0>
{
	protected:
		struct node
		{
			node* parent;
			node* next;
			node* prev;
			node* firstchild;
			node* lastchild;
			xlong  childs;
			member*  data;
			
			//~node() //todo!
		};
	
		node* rootnode;
		node* current;
	public:
		CLtree();
		~CLtree() { /*delete rootnode;*/ };

		void setroot() { current = rootnode; };
		bool isroot() const { return (current==rootnode); };
		
		void addchild(member* d);
		void delchild(xlong i);
		bool setchild(xlong i);
		bool setchild(const xchar* n);
		xlong setfirstchild() { current = current->firstchild; return 0; };
		xlong setlastchild() { current = current->lastchild; return current->childs; };
		xlong getchilds() const { return current->childs; };
		
		bool setparent() { if(current!=rootnode) { current = current->parent; return 1; } else {return 0; } };
		
		void delcur();
		xlong setfirstsibling() { current = current->parent->firstchild; return 0; };
		xlong setlastsibling() { current = current->parent->lastchild; return current->parent->childs; };
		xlong setnextsibling() { current = current->next; return 1; };
		xlong setprevsibling() { current = current->prev; return -1; };
		xlong getsiblingcount() const { return current->parent->childs; };
		bool isfirstsibling() const { return (current->parent->firstchild==current); };
		bool islastsibling() const { return (current->parent->lastchild==current); };
	
		void setdata(member* d) { current->data = d; };
		member* getdata() const { return current->data; };
		const xchar* getname() const { return current->name; }
		
		void print(xlong d=0);
};
///*

///implementation
template<class member>
CLtree<member>::CLtree() //! noncritical
{
	rootnode = new node;
	current =  rootnode->prev = rootnode->next = rootnode->parent = rootnode;
	rootnode->childcount = 0;
	rootnode->data = 0;
}

template<class member>
void CLtree<member>::addchild(member* d) //! noncritical
{
	node* newnode = new node;
	newnode->parent = current;
	newnode->next = newnode;
	if(current->childs==0) { newnode->prev = newnode->next = current->firstchild = current->lastchild = newnode; }
	else { newnode->prev = current->lastchild; current->lastchild = newnode; }
	newnode->child = 0;
	newnode->childs = 0;
	newnode->data = d;
	current->childs++;
}

template<class member>
void CLtree<member>::delchild(xlong i) //! noncritical
{
	if(i<current->childs)
	{
		setchild(i);
		node* temp = current;
		node* prev = current->prev;
		node* next = current->next;
		current->next->prev = prev;
		current->prev->next = next;
		current = current->parent;
		current->childs--;
		//delete temp;
	}
}

template<class member>
void CLtree<member>::delcur() //! noncritical
{
	if(current!=rootnode)
	{
		node* temp = current;
		node* prev = current->prev;
		node* next = current->next;
		current->next->prev = prev;
		current->prev->next = next;
		current = current->parent;
		current->childs--;
		//delete temp;
	}
}

template<class member>
bool CLtree<member>::setchild(xlong i) //! noncritical
{
	if(i<current->childs)
	{
		current = current->child[0];
		for(xlong j=0;j<i;j++) { setnextsibling(); }
		return 1;
	}
	else { return 0; }
}

template<class member>
void CLtree<member>::print(xlong d) //! noncritical
{
	setroot();
	for(xlong j=0;j<d;j++) { tty("+"); }
	say("X");
	setfirstchild();
	for(uxlong i=0;i<current->parent->childs;i++)
	{
		print(d+1);
		setnextsibling();
	}
	current = current->parent;
}
///*

#endif

