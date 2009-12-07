///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLEWAVE
#define HH_CLEWAVE
///*

///header
/* class name:	CLewave
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
class CLewave : public virtual CLcl : public CLentity<1>
{
	private:
		CLobject* ewave;
		bool updown;
		
	public:
		CLewave(bool ud);
		~CLewave();
		void update();
};
///*

///implementation
CLewave::CLewave(bool ud) //! noncritical
{
	updown = ud;
	
	rawpoly polys[9];
	
	polys[0].v[0].x = 0; polys[0].v[0].y = 0; polys[0].v[0].z = 0;
	polys[0].v[1].x = 0; polys[0].v[1].y = 0; polys[0].v[1].z = 0;
	polys[0].v[2].x = 0; polys[0].v[2].y = 0; polys[0].v[2].z = 0;
	polys[0].v[3].x = 0; polys[0].v[3].y = 0; polys[0].v[3].z = 0;
	
	polys[1].v[0].x = 0; polys[1].v[0].y = 0; polys[1].v[0].z = 0;
	polys[1].v[1].x = 0; polys[1].v[1].y = 0; polys[1].v[1].z = 0;
	polys[1].v[2].x = 0; polys[1].v[2].y = 0; polys[1].v[2].z = 0;
	polys[1].v[3].x = 0; polys[1].v[3].y = 0; polys[1].v[3].z = 0;
	
	polys[2].v[0].x = 0; polys[2].v[0].y = 0; polys[2].v[0].z = 0;
	polys[2].v[1].x = 0; polys[2].v[1].y = 0; polys[2].v[1].z = 0;
	polys[2].v[2].x = 0; polys[2].v[2].y = 0; polys[2].v[2].z = 0;
	polys[2].v[3].x = 0; polys[2].v[3].y = 0; polys[2].v[3].z = 0;
	
	polys[3].v[0].x = 0; polys[3].v[0].y = 0; polys[3].v[0].z = 0;
	polys[3].v[1].x = 0; polys[3].v[1].y = 0; polys[3].v[1].z = 0;
	polys[3].v[2].x = 0; polys[3].v[2].y = 0; polys[3].v[2].z = 0;
	polys[3].v[3].x = 0; polys[3].v[3].y = 0; polys[3].v[3].z = 0;
	
	polys[4].v[0].x = 0; polys[4].v[0].y = 0; polys[4].v[0].z = 0;
	polys[4].v[1].x = 0; polys[4].v[1].y = 0; polys[4].v[1].z = 0;
	polys[4].v[2].x = 0; polys[4].v[2].y = 0; polys[4].v[2].z = 0;
	polys[4].v[3].x = 0; polys[4].v[3].y = 0; polys[4].v[3].z = 0;
	
	polys[5].v[0].x = 0; polys[5].v[0].y = 0; polys[5].v[0].z = 0;
	polys[5].v[1].x = 0; polys[5].v[1].y = 0; polys[5].v[1].z = 0;
	polys[5].v[2].x = 0; polys[5].v[2].y = 0; polys[5].v[2].z = 0;
	polys[5].v[3].x = 0; polys[5].v[3].y = 0; polys[5].v[3].z = 0;
	
	polys[6].v[0].x = 0; polys[6].v[0].y = 0; polys[6].v[0].z = 0;
	polys[6].v[1].x = 0; polys[6].v[1].y = 0; polys[6].v[1].z = 0;
	polys[6].v[2].x = 0; polys[6].v[2].y = 0; polys[6].v[2].z = 0;
	polys[6].v[3].x = 0; polys[6].v[3].y = 0; polys[6].v[3].z = 0;
	
	polys[7].v[0].x = 0; polys[7].v[0].y = 0; polys[7].v[0].z = 0;
	polys[7].v[1].x = 0; polys[7].v[1].y = 0; polys[7].v[1].z = 0;
	polys[7].v[2].x = 0; polys[7].v[2].y = 0; polys[7].v[2].z = 0;
	polys[7].v[3].x = 0; polys[7].v[3].y = 0; polys[7].v[3].z = 0;
	
	polys[8].v[0].x = 0; polys[8].v[0].y = 0; polys[8].v[0].z = 0;
	polys[8].v[1].x = 0; polys[8].v[1].y = 0; polys[8].v[1].z = 0;
	polys[8].v[2].x = 0; polys[8].v[2].y = 0; polys[8].v[2].z = 0;
	polys[8].v[3].x = 0; polys[8].v[3].y = 0; polys[8].v[3].z = 0;
	
	ewave = new CLobject(poyls,9,0x00000040,0);
}

CLewave::~CLewave() //! noncritical
{
	delete ewave;
}

void CLewave::update() //! critical
{
	
}
///*

#endif
