//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLXML
#define HH_CLXML
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLstructs.hh"
#include "CLapi.hh"
#include "CLtree.hh"

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


#endif

