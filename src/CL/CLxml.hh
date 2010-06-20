///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLXML
#define HH_CLXML
///*

///includes
#include "CLtypes.hh"
#include "CLstring.hh"
#include "CLtree.hh"
#include "CLbase.hh"
///*

///header
/* class name:	CLxml
 * 
 * description:	
 * 
 * author:		atcl
 * 
 * notes:		implement
 * 
 * version: 	0.1
 */
///*

///definitions
class CLxml : public CLbase<CLxml,0>
{
	private:
		CLstring& clstring;
	protected:
		struct CLelement
		{
			const xchar*  name;
			const xchar*  data;
			const xchar** attributes;
		};
	
		CLtree<CLelement> xmltree;
	public:
		CLxml(CLfile* fileptr);
		~CLxml();

		void root();
		const xchar* rootname();
		
		void child(const xchar* n,xlong i=0) const;
		void child(xlong i) const;
		void addchild(const xchar* n);
		void delchild(const xchar* n,xlong i);
		
		const xchar* attr(const xchar* a) const;
		const xchar* attr(xlong i) const;
		void setattr(const xchar* a,const xchar* v);
		void delattr(const xchar* a);
		
		const xchar* data() const;
		void setdata(const xchar* d);

		const xchar* encoding() const;
		const xchar* version() const;
		
		void write(const xchar* f) const;
};
///*

///implementation



///*

#endif

