///license
//atCROSSLEVEL studios 2009,2010
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
class CLewave : public CLentity<1>
{
	private:
	
	protected:
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
	
	xlong m = 1;
	if(updown) m = -1;
	
	rawpoly polys[9];
	
	polys[0].v[0].x = -300; polys[0].v[0].y = m*20; polys[0].v[0].z = 20;
	polys[0].v[1].x = -300; polys[0].v[1].y = 0;    polys[0].v[1].z = 0;
	polys[0].v[2].x = -400; polys[0].v[2].y = 0;    polys[0].v[2].z = 20;
	polys[0].v[3].x = -400; polys[0].v[3].y = 0;    polys[0].v[3].z = 20;
	
	polys[1].v[0].x = -300; polys[1].v[0].y = 0;    polys[1].v[0].z = 0;
	polys[1].v[1].x = -300; polys[1].v[1].y = m*20; polys[1].v[1].z = 20;
	polys[1].v[2].x = 300;  polys[1].v[2].y = m*20; polys[1].v[2].z = 20;
	polys[1].v[3].x = 300;  polys[1].v[3].y = 0;    polys[1].v[3].z = 0;
	
	polys[2].v[0].x = 300; polys[2].v[0].y = m*20; polys[2].v[0].z = 20;
	polys[2].v[1].x = 400; polys[2].v[1].y = 0;   polys[2].v[1].z = 20;
	polys[2].v[2].x = 300; polys[2].v[2].y = 0;   polys[2].v[2].z = 0;
	polys[2].v[3].x = 300; polys[2].v[3].y = 0;   polys[2].v[3].z = 0;
	
	polys[3].v[0].x = -300; polys[4].v[0].y = m*20; polys[4].v[0].z = 80;
	polys[3].v[1].x = -300; polys[4].v[1].y = m*20; polys[4].v[1].z = 20;
	polys[3].v[2].x = -400; polys[4].v[2].y = 0;    polys[4].v[2].z = 20;
	polys[3].v[3].x = -400; polys[4].v[3].y = 0;    polys[4].v[3].z = 80;
	
	polys[4].v[0].x = -300; polys[5].v[0].y = m*20; polys[5].v[0].z = 20;
	polys[4].v[1].x = -300; polys[5].v[1].y = m*20; polys[5].v[1].z = 80;
	polys[4].v[2].x = 300;  polys[5].v[2].y = m*20; polys[5].v[2].z = 80;
	polys[4].v[3].x = 300;  polys[5].v[3].y = m*20; polys[5].v[3].z = 20;
	
	polys[5].v[0].x = 300;  polys[6].v[0].y = m*20; polys[6].v[0].z = 20;
	polys[5].v[1].x = 300;  polys[6].v[1].y = m*20; polys[6].v[1].z = 80;
	polys[5].v[2].x = 400;  polys[6].v[2].y = 0;    polys[6].v[2].z = 80;
	polys[5].v[3].x = 400;  polys[6].v[3].y = 0;    polys[6].v[3].z = 20;
	
	polys[6].v[0].x = -300; polys[8].v[0].y = m*20; polys[8].v[0].z = 80;
	polys[6].v[1].x = -400; polys[8].v[1].y = 0;    polys[8].v[1].z = 80;
	polys[6].v[2].x = -300; polys[8].v[2].y = 0;    polys[8].v[2].z = 100;
	polys[6].v[3].x = -300; polys[8].v[3].y = 0;    polys[8].v[3].z = 100;
	
	polys[7].v[0].x = -300; polys[7].v[0].y = m*20; polys[7].v[0].z = 80;
	polys[7].v[1].x = -300; polys[7].v[1].y = 0;    polys[7].v[1].z = 100;
	polys[7].v[2].x = 300;  polys[7].v[2].y = 0;    polys[7].v[2].z = 100;
	polys[7].v[3].x = 300;  polys[7].v[3].y = m*20; polys[7].v[3].z = 80;
	
	polys[8].v[0].x = 300; polys[3].v[0].y = m*20; polys[3].v[0].z = 80;
	polys[8].v[1].x = 300; polys[3].v[1].y = 0;    polys[3].v[1].z = 100;
	polys[8].v[2].x = 400; polys[3].v[2].y = 0;    polys[3].v[2].z = 80;
	polys[8].v[3].x = 400; polys[3].v[3].y = 0;    polys[3].v[3].z = 80;
	
	model[0] = new CLobject(poyls,9,0x00000040,0);
	
	position.x = 0;
	position.y = 0;
	position.z = 50;
}

CLewave::~CLewave() { } //! noncritical

void CLewave::update() //! critical
{
	
}
///*

#endif
