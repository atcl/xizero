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
 * description:	wave object roaming through levels 
 * 
 * author:		atcl
 * 
 * notes:		...
 * 
 * version: 	0.2
 */
///*
 
///definitions
class CLewave : public CLentity<1>
{
	private:
		CLwindow& clwindow;
	protected:
		bool updown;
	public:
		CLewave(bool ud);
		~CLewave();
		xlong update();
};
///*

///implementation
CLewave::CLewave(bool ud) //! noncritical
: clwindow(CLwindow::instance())
{
	updown = ud;
	
	xlong m = 1;
	if(updown) { m = -1; }
	
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
	
	model[0] = new CLobject(polys,9,0x00000040,0);
	
	maxspeed = 20;
	
	position.x = 0;
	position.y = 0;
	position.z = 50;
	
	speed.x = 0;
	speed.y = maxspeed;
	speed.z = 0;
	
	speeddir = m;
	
	//set remaining entity attributes
	visible = active = 0;
	lastupdate = clwindow.getmilliseconds();
	//*
}

CLewave::~CLewave() { } //! noncritical

xlong CLewave::update() //! critical
{
	//check if to activate
	if(active==0 && ( (*mark)-100)<position.y) { active = 1; }
	//*
	
	if(active==1)
	{
		xlong time = clwindow.getmilliseconds();
		
		//update position
		float inter = time-lastupdate;
		tposition.x = position.x - (inter*speed.x);
		tposition.y = position.y + (inter*speed.y);
		tposition.z = position.z - (inter*speed.z);
		lastupdate = time;	
		//*
		
	}
	else { lastupdate = clwindow.getmilliseconds(); }
	
	return -1;
}
///*

#endif
