///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLXML
#define HH_CLXML
///*

///includes
#include "CLtypes.hh"
#include "CLsystem.hh"
#include "CLtree.hh"
///*

///header
/* class name:	CLxml
 * 
 * description:	
 * 
 * author:	atcl
 * 
 * notes:	
 * 
 * version: 0.1
 */
///*

///definitions
struct xmlelement
{
	xchar*  value;
	xchar** attributes;
};

class CLxml : public virtual CLcl
{
	protected:
		CLtree xmltree;

	public:
		CLxml(const xchar* filename);
		CLxml(CLfile* fileptr);
		~CLxml();

		void root();
		xchar* rootname();
		
		void firstchild();
		void lastchild();
		void nextchild();
		void prevchild();
		void firstchild(const xchar* n);
		void lastchild(const xchar* n);
		void nextchild(const xchar* n);
		void prevchild(const xchar* n);
		xchar* childname();
		
		xchar* data();
		bool havedata();
		
		void firstattribute();
		void lastattribute();
		void nextattribute();
		void prevattribute();
		void firstattribute(const xchar* n);
		void lastattribute(const xchar* n);
		void nextattribute(const xchar* n);
		void prevattribute(const xchar* n);
		xchar* attributename();
		
		bool parent(); //if root return 1 else 0

		xchar* getnamespace();
		xchar* getencoding();
		xchar* getversion();
		
		//output and manipulate:
		//
		//addchild
		//delchild
		//addattribute
		//delattribute
		//adddata
		//deldata
		//write
		//header
};
///*

///implementation



///*

#endif

