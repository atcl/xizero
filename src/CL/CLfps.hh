//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBENCH
#define HH_CLBENCH
#pragma message "Compiling " __FILE__ " ! TODO: check if timeinterval correct"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLsystem.hh"

//rename file to CLbench.hh
class CLbench : public virtual CLcl, public CLsingle<CLbench>
{
	friend class CLsingle<CLbench>;
	
	private:
		CLsystem* system;
		xlong framespersecond;
		xlong currenttime;
		xlong elapsedtime;
		xlong temptime;
		xlong frames;
		xlong lastupdate;
		xlong interval;
		xchar flags;
		CLbench();
		~CLbench();
	public:
		void init(xlong i,xchar flags);
		void inc();
		float getfps();
		void print();
};

CLbench::CLbench() { system = CLsystem::instance(); }

CLbench::~CLbench() { }

void CLbench::init(xlong i,xchar f)
{
	interval = i*1000;
	flags = f;	
	frames = 0;
	lastupdate = system->getmilliseconds();
	elapsedtime = 0;
}

void CLbench::inc()
{
	frames++;
	currenttime = system->getmilliseconds();
	elapsedtime += (currenttime - lastupdate);
	temptime = elapsedtime;
//nicht durch temptime teilen, da altes zu neuem und durch 2
	if(temptime >= interval)
	{
		framespersecond = xlong(frames*1000/temptime);
		print();
		frames = 0;
		elapsedtime = 0;
	}

	lastupdate = currenttime;
}

float CLbench::getfps()
{
	return framespersecond;
}

void CLbench::print()
{
		if(flags & COUT_FPS)
		{
			system->print("fps: ",0);
			system->print(framespersecond);
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

