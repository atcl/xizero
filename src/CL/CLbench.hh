///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
///*

///guard
#ifndef HH_CLBENCH
#define HH_CLBENCH
///*

///includes
#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLsystem.hh"
///*

///header
/* class name:	CLbench
 * 
 * description:	This class calculates fps, memory and cpu usage.
 * 
 * author:	atcl
 * 
 * notes:	memory and cpu usage need implementing.
 * 
 * version: 0.1
 */
///*

///definitions
class CLbench : public CLbase<CLbench,1>
{
	friend class CLbase<CLbench,1>;
	
	private:
		static CLsystem* clsystem;
	protected:
		xlong framespersecond;
		xlong currenttime;
		xlong elapsedtime;
		xlong temptime;
		xlong frames;
		xlong lastupdate;
		xlong interval;
		xchar flags;
		CLbench() { };
		~CLbench() { };
	public:
		void init(xlong i,xchar flags);
		void inc();
		float getfps() const { return framespersecond; };
		void print() const;
};

CLsystem* CLbench::clsystem = CLsystem::instance();
///*

///imlementation
void CLbench::init(xlong i,xchar f) //! noncritical
{
	interval = i*1000;
	flags = f;	
	frames = 0;
	lastupdate = clsystem->getmilliseconds();
	elapsedtime = 0;
}

void CLbench::inc() //! critical
{
	frames++;
	currenttime = clsystem->getmilliseconds();
	elapsedtime += (currenttime - lastupdate);

	if(elapsedtime >= interval)
	{
		framespersecond = xlong(frames*1000/elapsedtime);
		print();
		frames = 0;
		elapsedtime = 0;
	}

	lastupdate = currenttime;
}

void CLbench::print() const //! noncritical
{
		if(flags & COUT_FPS)
		{
			tty(u8"fps: ");
			say(framespersecond);
		}
		
		//~ if(flags & COUT_RAM)
		//~ {
			//~ CLsystem::print("ram: ",0);
			//~ CLsystem::print( float(usedmem/1048576),0);
			//~ CLsystem::print("MB");
		//~ }
		//~ 
		//~ if(flags & COUT_VRAM)
		//~ {
			//~ CLsystem::print("vram: ",0);
			//~ CLsystem::print( float(usedvmem/1048576),0);
			//~ CLsystem::print("MB");
		//~ }
}
///*

#endif

