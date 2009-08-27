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

template<int I> class CLentity; //forward declaration

struct CLammo
{
	void(*comsprite)(xlong x,xlong y);
	CLfvector p;
	CLfvector s;
};

typedef CLlist<CLammo> CLammolist;

class CLammomanager : public virtual CLcl
{
	private:
		CLammolist* ammolist;
		CLammo** ammotype;
		xlong ammotypecount;
		xlong lastupdate;
		xlong* mark;
	
	public:
		CLammomanager(xlong atc,xlong* ats,xlong* m);
		~CLammomanager();
		
		void fire(uxlong ammotype,const CLfvector& startposition,const CLfvector direction);
		void update();
		template<int I>void collision(CLentity<I>* e);
		void display();
};

CLammomanager::CLammomanager(xlong atc,xlong* ats,xlong* m)
{
	mark = m;
	ammotypecount = atc;
	ammolist = new CLammolist();
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

void CLammomanager::fire(uxlong at,const CLfvector& startposition,const CLfvector direction)
{
	if(at<=ammotypecount)
	{
		CLammo* currammo = new CLammo();
		currammo->comsprite = ammotype[at]->comsprite;
		currammo->p = startposition;
		currammo->s = direction;
		ammolist->append(currammo,"at" + xchar(at+30) );
	}
}

void CLammomanager::update()
{
	xlong time = CLsystem::getmilliseconds();

	bool listfix=0;
	CLammo* currammo = 0;
	for(xlong i=ammolist->setfirst(); i<ammolist->getlength(); i+=ammolist->setnext())
	{
		if(listfix) { i+=ammolist->setprev(); listfix=0; }
		currammo = ammolist->getcurrentdata();
		
		float inter = time-lastupdate;
		currammo->p.x += inter*currammo->s.x;
		currammo->p.y -= inter*currammo->s.y;
		currammo->p.z += inter*currammo->s.z;
		
		if(CLgame::boundary(currammo->p,*mark)!=0)
		{
			ammolist->delcurrent(0);
			listfix = ammolist->isfirst();
		}
	}
	lastupdate = time;

}

template<int I>
void CLammomanager::collision(CLentity<I>* e)
{
	xlong r = 0;
	xlong time = CLsystem::getmilliseconds();

	bool listfix=0;
	CLammo* currammo = 0;
	for(xlong i=ammolist->setfirst(); i<ammolist->getlength(); i+=ammolist->setnext())
	{
		if(listfix) { i+=ammolist->setprev(); listfix=0; }
		currammo = ammolist->getcurrentdata();
		
		if(e->isvisible() && CLgame::collision2d(*(e->getposition()),*(e->getboundingbox()),currammo->p,CLmath::delta(i))==0)
		{
			r++;
			ammolist->delcurrent(0);
			listfix = ammolist->isfirst();
		}
	}
	e->hit(r);
}

void CLammomanager::display()
{
	CLammo* currammo = 0;
	for(xlong i=ammolist->setfirst(); i<ammolist->getlength();i+=ammolist->setnext())
	{
		currammo = ammolist->getcurrentdata();
		currammo->comsprite(currammo->p.x,currammo->p.y-(*mark));
	}
}

#endif
