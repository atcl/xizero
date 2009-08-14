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

class CLammomanager : public virtual CLcl
{
	private:
		CLlist* ammolist;
		CLammo** ammotype;
		xlong ammotypecount;
		xlong lastupdate;
		xlong* mark;
	
	public:
		CLammomanager(xlong atc,xlong* ats,xlong* m);
		~CLammomanager();
		
		void fire(uxlong ammotype,const CLfvector& startposition,const CLfvector direction);
		template<int I>void update(CLentity<I>* e,bool n=1);
		void display();
};

CLammomanager::CLammomanager(xlong atc,xlong* ats,xlong* m)
{
	mark = m;
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

template<int I>
void CLammomanager::update(CLentity<I>* e,bool n)
{
	xlong r = 0;
	xlong time = CLsystem::getmilliseconds();

	CLammo* currammo = 0;
	for(xlong i=ammolist->setfirst(); i<ammolist->getlength(); i+=ammolist->setnext())
	{
		currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
		if(CLgame::boundary(currammo->p,*mark)!=0 && n)
		{
			ammolist->delcurrent(0);
		}
		else if(e->isvisible() && CLgame::collision2d(*(e->getposition()),*(e->getboundingbox()),currammo->p,CLmath::delta(i))==0)
		{
			r++;
			ammolist->delcurrent(0);
		}
		else if(n)
		{
			float inter = time-lastupdate;
			currammo->p.x += inter*currammo->s.x;
			currammo->p.y -= inter*currammo->s.y;
			currammo->p.z += inter*currammo->s.z;
		}
	}
	lastupdate = time;
	e->hit(r);
}

void CLammomanager::display()
{
	CLammo* currammo = 0;
	for(xlong i=ammolist->setfirst(); i<ammolist->getlength();i+=ammolist->setnext())
	{
		currammo = static_cast<CLammo*>(ammolist->getcurrentdata());
		currammo->comsprite(currammo->p.x,currammo->p.y-(*mark));
	}
}

#endif
