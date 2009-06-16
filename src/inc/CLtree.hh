//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLTREE
#define HH_CLTREE
#warning "Compiling " __FILE__ " ! TODO: all"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLstruct.hh"

class CLtree : public virtual CLcl
{
	private:
		node* rootnode;
		node* current;

	public:
		CLtree();
		~CLtree();

		void root();
		void child(xlong i);
		xlong childcount();
		void parent();
		void next();	//next sibling
		void* data();
		void data(void* d);
		void addchild();
		void delchild(xlong i);
		bool isroot();
		void print();
};

CLtree::CLtree()
{
	rootnode = new node;
	rootnode->parent = 0;
}

CLtree::~CLtree()
{
	delete rootnode;
}

void CLtree::root()
{
	current = rootnode;
}

void CLtree::child(xlong i)
{
	if(i<current->childcount)
	{
		current = current->child[i];
	}
}

xlong CLtree::childcount()
{
	return current->childcount;
}

void CLtree::parent()
{
	current = current->parent;
}

void CLtree::next()
{
	if(current->next!=0)
	{
		current = current->next;
	}
}

void* CLtree::data()
{
	return current->data;
}

void CLtree::data(void* d)
{
	current->data = d;
}

void CLtree::addchild()
{
	node* newnode = new node;

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

void CLtree::delchild(xlong i)
{
	if(i >= current->childcount)
	{
		delete current->child[i-1]->next;
		current->child[i-1]->next = current->child[i-1];
	}
}

bool CLtree::isroot()
{
	if(current==rootnode) return true;
	else return false;
}

void CLtree::print()
{

}

#endif

