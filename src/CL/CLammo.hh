///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLAMMO
#define HH_CLAMMO
///*

///includes
#include "CLtypes.hh"
#include "CLsystem.hh"
#include "CLlist.hh"
#include "CLsprites.hh"
#include "CLgame.hh"
#include "CLmath.hh"
///*

///header
/* class name:	CLammo
 * 
 * description:	This class manages all ammo fired by entities.
 * 
 * author:	atcl
 * 
 * notes:	some issues when twin ammo fired, one being slower.
 * 
 * version: 0.1
 */
///*

///declarations
template<int I> class CLentity; 
///*

///definitions
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
		void display() const;
		void pause();
};
///*

///implementation
CLammomanager::CLammomanager(xlong atc,xlong* ats,xlong* m) //! noncritical
{	
	//set up attributes
	mark = m;
	ammotypecount = atc;
	ammolist = new CLammolist();
	ammotype = new CLammo*[atc];
	//*
	
	//fill ammotypes
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
	//*
}

CLammomanager::~CLammomanager() //! noncritical
{
	delete ammolist;
	delete[] ammotype;
}

void CLammomanager::fire(uxlong at,const CLfvector& startposition,const CLfvector direction) //! critical
{
	//append ammolist if ammotype exists
	if(at<ammotypecount)
	{
		CLammo* currammo = new CLammo();
		currammo->comsprite = ammotype[at]->comsprite;
		currammo->p = startposition;
		currammo->s = direction;
		ammolist->append(currammo,"at" + xchar(at+30) );
	}
	//*
}

void CLammomanager::update() //! critical
{
	xlong time = clsystem->getmilliseconds();
	bool listfix=0;
	CLammo* currammo = 0;
	
	//update all ammolist members
	for(xlong i=ammolist->setfirst(); i<ammolist->getlength(); i+=ammolist->setnext())
	{
		//place the ammolist index correctly after deleting ammo from the list
		if(listfix) { i+=ammolist->setprev(); listfix=0; }
		currammo = ammolist->getcurrentdata();
		//*
		
		//update current ammo position
		float inter = time-lastupdate;
		currammo->p.x += inter*currammo->s.x;
		currammo->p.y -= inter*currammo->s.y;
		currammo->p.z += inter*currammo->s.z;
		//*
		
		//check if current ammo left screen
		if(clgame->boundary(currammo->p,*mark)!=0)
		{
			ammolist->delcurrent(0);
			listfix = ammolist->isfirst();
		}
		//*
	}
	//*
	
	//save time
	lastupdate = time;
	//*
}

template<int I>
void CLammomanager::collision(CLentity<I>* e) //! critical
{
	xlong r = 0;
	bool listfix=0;
	CLammo* currammo = 0;
	
	//test all ammolist members for collisions
	for(xlong i=ammolist->setfirst(); i<ammolist->getlength(); i+=ammolist->setnext())
	{
		//place the ammolist index correctly after deleting ammo from the list
		if(listfix) { i+=ammolist->setprev(); listfix=0; }
		currammo = ammolist->getcurrentdata();
		//*
		
		//test the current ammo for collision with any opposite entity
		if(e->isvisible() && clgame->collision2d(*(e->getposition()),*(e->getboundingbox()),currammo->p,clmath->delta(i))==0)
		{
			r++;
			ammolist->delcurrent(0);
			listfix = ammolist->isfirst();
		}
		//*
	}
	//*
	
	//if hit opposing entity let it know
	if(r!=0) e->hit(r);
	//*
}

void CLammomanager::display() const //! critical
{
	CLammo* currammo = 0;
	
	//draw all ammo in the ammolist
	for(xlong i=ammolist->setfirst(); i<ammolist->getlength();i+=ammolist->setnext())
	{
		currammo = ammolist->getcurrentdata();
		currammo->comsprite(currammo->p.x,currammo->p.y-(*mark));
	}
	//*
}

void CLammomanager::pause() //! noncritical
{
	lastupdate = clsystem->getmilliseconds();
}
///*

#endif
