///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLVOXEL
#define HH_CLVOXEL
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLvector.hh"
///*

///header
/* class name:	CLvoxel
 * 
 * description:	
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*

///definitions
struct CLvoxel
{
	CLfvector position;
	CLfvector normal;
	uxlong    color;
};

class CLvoxels : public CLbase<CLid,0>
{
	friend class CLbase<CLvoxels,0>;
	
	private:
		
	protected:
		CLvoxel** voxel;
		
		CLvoxel** getbyvisiblenormal(CLvoxel** v);
		CLvoxel*  getbyvidiblez(CLvoxel** v)
	public:
		CLvoxel() { };
		~CLvoxel() { };

};
///*

///implementation



///*

#endif
