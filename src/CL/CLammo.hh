///license
//atCROSSLEVEL studios 2009,2010
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLAMMO
#define HH_CLAMMO
///*

///includes
#include "CLtypes.hh"
#include "CLbase.hh"
#include "CLbuffer.hh"
#include "CLwindow.hh"
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
 * author:		atcl
 * 
 * notes:		...
 * 
 * version:		0.2
 */
///*

///declarations
template<int I> class CLentity; 

typedef  void (CLcomspr::*comsprite)(xlong x,xlong y);
///*

///definitions
struct CLammo
{
	bool      f;
	comsprite m;
	CLfvector p;
	CLfvector s;
};

typedef CLlist<CLammo> CLammolist;

class CLammomanager : public CLbase<CLammomanager,0>
{
	private:
		CLscreen&  clscreen;
		CLwindow&  clwindow;
		CLgame&    clgame;
		CLmath&    clmath;
		CLcomspr&  clcomspr;
	protected:
		CLammolist* list;
		CLammo**    type;
		xlong       types;
		xlong       last;
		const xlong& mark;
	public:
		CLammomanager(xlong ammotypecount,xlong* ammotypelist,const xlong& markref);
		~CLammomanager();
		void fire(xlong ammotype,const CLfvector& startposition,const CLfvector direction);
		void update();
		template<int I>void collision(CLentity<I>* e);
		void display() const;
		void pause();
};
///*

///implementation
CLammomanager::CLammomanager(xlong ammotypecount,xlong* ammotypelist,const xlong& markref) : mark(markref), //! noncritical
 clscreen(CLscreen::instance()), clwindow(CLwindow::instance()), clgame(CLgame::instance()), clmath(CLmath::instance()), clcomspr(CLcomspr::instance())
{	
	//set up attributes
	types = ammotypecount;
	list = new CLammolist();
	type = new CLammo*[types];
	//*
	
	//fill ammotypes
	for(xlong i=0;i<types;i++)
	{
			type[i] = new CLammo;
			type[i]->p = CLfvector();
			type[i]->s = CLfvector();
			switch(ammotypelist[i])
			{
				case 0:  type[i]->m = &CLcomspr::drawplasma; break;
				case 1:  type[i]->m = &CLcomspr::drawantimatter; break;
				
				default: type[i]->m = &CLcomspr::drawplasma; break;
			}
	}
	//*
}

CLammomanager::~CLammomanager() //! noncritical
{
	delete list;
	delete[] type;
}

void CLammomanager::fire(xlong at,const CLfvector& startposition,const CLfvector direction) //! critical
{
	//append ammolist if ammotype exists
	if(at<types)
	{
		CLammo* curr = new CLammo();
		curr->m = type[at]->m;
		curr->p = startposition;
		curr->s = direction;
		list->append(curr,"at" + xchar(at+30) );
	}
	//*
}

void CLammomanager::update() //! critical
{
	xlong time = clwindow.getmilliseconds();
	bool listfix=0;
	CLammo* curr = 0;
	
	//update all ammolist members
	for(xlong i=list->setfirst(); i<list->getlength(); i+=list->setnext())
	{
		//place the ammolist index correctly after deleting ammo from the list
		if(listfix) { i+=list->setprev(); listfix=0; }
		curr = list->getcurrentdata();
		//*
		
		//update current ammo position
		float inter = time-last;		//? call here clwindow->getmilliseconds()
		curr->p.x += inter*curr->s.x;
		curr->p.y -= inter*curr->s.y;
		curr->p.z += inter*curr->s.z;
		//*
		
		//check if current ammo left screen
		if(clgame.boundary(curr->p,mark)!=0)
		{
			list->delcurrent(0);
			listfix = list->isfirst();
		}
		//*
	}
	//*
	
	//save time
	last = time;
	//*
}

template<int I>
void CLammomanager::collision(CLentity<I>* e) //! critical
{
	xlong r = 0;
	CLammo* curr = 0;
	
	xlong l=list->getlength();
	//test all ammolist members for collisions
	for(xlong i=list->setfirst(); i<l; i+=list->setnext())
	{
		curr = list->getcurrentdata();
		
		//test the current ammo for collision with given opposite entity
		if(e->isvisible() && clgame.collision2d(*(e->getposition()),*(e->getboundingbox()),curr->p,clmath.delta(i))==0)
		{
			r++;
			if(list->isfirst()) { i--; }
			list->delcurrent(0);
		}
		//*
	}
	//*
	
	//if hit opposing entity let it know
	if(r!=0) { e->hit(r); }
	//*
}

void CLammomanager::display() const //! critical
{
	CLammo* curr = 0;
	
	//draw all ammo in the ammolist
	for(xlong i=list->setfirst(); i<list->getlength();i+=list->setnext())
	{
		curr = list->getcurrentdata();
		(clcomspr.*curr->m)(curr->p.x,curr->p.y-mark);
	}
	//*
}

void CLammomanager::pause() { last = clwindow.getmilliseconds(); } //! noncritical
///*

#endif
