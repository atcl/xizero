//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLAMMO
#define HH_CLAMMO
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLlist.hh"
#include "CLsprites.hh"
#include "CLgame.hh"
#include "CLmath.hh"
//~ #include "CLplayer.hh"
//~ #include "CLenemy.hh"


struct CLammo
{
	void(*comsprite)(xlong x,xlong y);
	CLfvector p;
	CLfvector s;
};

class CLammomanager : public virtual CLcl
{
	private:
		CLlist* ammolist;
		CLammo** ammotype;
		CLammo* currammo;
		xlong ammotypecount;
		float ammospeed;
		xlong lastupdate;
	
	public:
		CLammomanager(xlong atc,xlong* ats);
		~CLammomanager();
		
		void fire(uxlong ammotype,const CLfvector& startposition,const CLfvector direction,xlong mark);
		xlong update();
		//xlong update(CLplayer* p);
		//xlong update(CLenemy* e);
		void display();
};

CLammomanager::CLammomanager(xlong atc,xlong* ats)
{
	ammospeed = 16;
	ammotypecount = atc;
	ammolist = new CLlist();
	ammotype = new CLammo*[atc];
	for(uxlong i=0; i<atc; i++)
	{
			ammotype[i] = new CLammo;
			ammotype[i]->p = CLfvector();
			ammotype[i]->s = CLfvector();
			switch(ats[i])
			{
				case 0: ammotype[i]->comsprite = CLsprites::drawplasma; break;
				case 1: ammotype[i]->comsprite = CLsprites::drawantimatter; break;
			}
	}
}

CLammomanager::~CLammomanager()
{
	delete ammolist;
	delete[] ammotype;
}

void CLammomanager::fire(uxlong at,const CLfvector& startposition,const CLfvector direction,xlong mark)
{
	if(at<=ammotypecount)
	{
		currammo = new CLammo();
		currammo->comsprite = ammotype[at]->comsprite;
		currammo->p = startposition;
		currammo->p.y -= mark;
		currammo->s = direction*ammospeed;
		ammolist->append(currammo,"at" + xchar(at+30) );
	}
}

xlong CLammomanager::update()
{
	xlong r = 0;
	xlong time = CLsystem::getmilliseconds();

	if(time >= lastupdate + 20)
	{
		for(int i=0; i<ammolist->getlength();i++)
		{
			ammolist->setindex(i);
			currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
			if(CLgame::boundary(currammo->p)!=0) { ammolist->delcurrent(0); i--; }
			else
			{
				currammo->p.x += currammo->s.x;
				currammo->p.y -= currammo->s.y;
				currammo->p.z += currammo->s.z;
			}
		}
		lastupdate = time;
	}
	
	return r;
}

//~ xlong CLammomanager::update(CLplayer* p)
//~ {
	//~ xlong r = 0;
	//~ xlong time = CLsystem::getmilliseconds();
//~ 
	//~ if(time >= lastupdate + 20)
	//~ {
		//~ for(int i=0; i<ammolist->getlength();i++)
		//~ {
			//~ ammolist->setindex(i);
			//~ currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
			//~ if(CLgame::boundary(currammo->p)!=0) { ammolist->delcurrent(0); i--; }
			//~ else if(CLgame::collision(p->getposition(),*(p->getboundingbox()),currammo->p,CLmath::delta(i))!=0)
			//~ {
				//~ 
			//~ }
			//~ else
			//~ {
				//~ currammo->p.x += currammo->s.x;
				//~ currammo->p.y -= currammo->s.y;
				//~ currammo->p.z += currammo->s.z;
			//~ }
		//~ }
		//~ lastupdate = time;
	//~ }
	//~ 
	//~ return r;
//~ }

//~ xlong CLammomanager::update(CLenemy* e)
//~ {
	//~ xlong r = 0;
	//~ xlong time = CLsystem::getmilliseconds();
//~ 
	//~ if(time >= lastupdate + 20)
	//~ {
		//~ for(int i=0; i<ammolist->getlength();i++)
		//~ {
			//~ ammolist->setindex(i);
			//~ currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
			//~ if(CLgame::boundary(currammo->p)!=0) { ammolist->delcurrent(0); i--; }
			//~ else if(CLgame::collision(e->getposition(),*(e->getboundingbox()),currammo->p,CLmath::delta(i))!=0)
			//~ {
				//~ 
			//~ }
			//~ else
			//~ {
				//~ currammo->p.x += currammo->s.x;
				//~ currammo->p.y -= currammo->s.y;
				//~ currammo->p.z += currammo->s.z;
			//~ }
		//~ }
		//~ lastupdate = time;
	//~ }
	//~ 
	//~ return r;
//~ }

void CLammomanager::display()
{
	for(int i=0; i<ammolist->getlength();i++)
	{
		ammolist->setindex(i);
		currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
		currammo->comsprite(currammo->p.x,currammo->p.y);
	}
}

#endif
