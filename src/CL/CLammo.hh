//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLAMMO
#define HH_CLAMMO
//#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLlist.hh"
#include "CLsprites.hh"
#include "CLgame.hh"


struct CLammo
{
	void(*comsprite)(xlong x,xlong y);
	CLfvector p;
	CLfvector d;
	float v;
};

class CLammomanager
{
	private:
		CLlist* ammolist;
		CLammo** ammotype;
		CLammo* currammo;
		xlong ammotypecount;
		xlong lastupdate;
	
	public:
		CLammomanager(xlong atc,xlong* ats);
		~CLammomanager();
		
		void fire(uxlong ammotype,const CLfvector& startposition,const CLfvector direction);
		void update(xlong mark);
		void display(xlong mark);
};

CLammomanager::CLammomanager(xlong atc,xlong* ats)
{
	ammotypecount = atc;
	ammolist = new CLlist();
	ammotype = new CLammo*[atc];
	for(uxlong i=0; i<atc; i++)
	{
			ammotype[i] = new CLammo;
			ammotype[i]->p = CLfvector();
			ammotype[i]->d = CLfvector();
			ammotype[i]->v = 16;
			switch(ats[i])
			{
				case 0: ammotype[i]->comsprite = CLsprites::drawplasma; break;
				case 1: ammotype[i]->comsprite = CLsprites::drawantimatter; break;
			}
	}
	lastupdate = CLsystem::getmilliseconds();
}

CLammomanager::~CLammomanager()
{
	delete ammolist;
}

void CLammomanager::fire(uxlong at,const CLfvector& startposition,const CLfvector direction)
{
	if(at<=ammotypecount)
	{
		currammo = new CLammo();
		currammo->comsprite = ammotype[at]->comsprite;
		currammo->v = ammotype[at]->v;
		currammo->p = startposition;
		currammo->d = direction;
		ammolist->append(currammo,"at" + xchar(at+30) );
	}
}

void CLammomanager::update(xlong mark)
{
	xlong time = CLsystem::getmilliseconds();
	
	if(time >= lastupdate + 20)
	{
		for(int i=0; i<ammolist->getlength();i++)
		{
			ammolist->setindex(i);
			currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
			currammo->p.y -= mark;
			if(CLgame::boundary(currammo->p)!=0) ammolist->delcurrent(0);
			else
			{
				currammo->p.x += currammo->v * currammo->d.x;
				currammo->p.y -= currammo->v * currammo->d.y;
				currammo->p.z += currammo->v * currammo->d.z;
			}
			currammo->p.y += mark;
		}
		lastupdate = time;
	}
}

void CLammomanager::display(xlong mark)
{
	for(int i=0; i<ammolist->getlength();i++)
	{
		ammolist->setindex(i);
		currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
		currammo->p.y -= mark;
		currammo->comsprite(currammo->p.x,currammo->p.y);
		currammo->p.y += mark;
	}
}

#endif
