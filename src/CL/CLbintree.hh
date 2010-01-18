///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLBINTREE
#define HH_CLBINTREE
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
///*

///header
/* class name:	CLbintree
 * 
 * description:	A (sorted) binary search tree.
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
class CLbintree : CLbase<CLbintree<member>,0>
{
	private:
	
	protected:
		struct node
		{
			member*	data;
			xlong   key;
			node*	left;
			node*	right;
		};
	
		node* search(xlong k,node* n);
		void insert(xlong k,member* d,node* n);
		node* root;
	public:
		CLbintree() { root = 0; };
		~CLbintree() { };
		member* search(xlong k);
		void insert(xlong k,member* d);
};
///*

///implementation
template<class member>
typename CLbintree<member>::node* CLbintree<member>::search(xlong k,node* n) //! critical
{

}

template<class member>
void CLbintree<member>::insert(xlong k,member* d,node* n) //! noncritical
{
	
}

template<class member>
member* CLbintree<member>::search(xlong k) //! critical
{
	
}

template<class member>
void CLbintree<member>::insert(xlong k,member* d) //! noncritical
{
	
}
///*


#endif
