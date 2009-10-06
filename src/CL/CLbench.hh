//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBENCH
#define HH_CLBENCH
#pragma message "Compiling " __FILE__ " ! TODO: check if timeinterval correct"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLsystem.hh"

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

class CLbench : public virtual CLcl, public CLsingle<CLbench>
{
	friend class CLsingle<CLbench>;
	
	private:
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
		inline float getfps();
		void print() const;
};

void CLbench::init(xlong i,xchar f)
{
	interval = i*1000;
	flags = f;	
	frames = 0;
	lastupdate = clsystem->getmilliseconds();
	elapsedtime = 0;
}

void CLbench::inc()
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

float CLbench::getfps() { return framespersecond; }

void CLbench::print() const
{
		if(flags & COUT_FPS)
		{
			clsystem->print("fps: ",0);
			clsystem->print(framespersecond);
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

#endif

