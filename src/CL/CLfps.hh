//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLBENCH
#define HH_CLBENCH
//#pragma message "Compiling " __FILE__ " ! TODO: check if timeinterval correct"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLsystem.hh"

//rename file to CLbench.hh
class CLbench : public virtual CLcl
{
	private:
		xlong framespersecond;
		xlong currenttime;
		xlong elapsedtime;
		xlong temptime;
		xlong frames;
		xlong lastupdate;
		
		xlong interval;
		xchar flags;
		
	public:
		CLbench(xlong i,xchar flags);
		~CLbench();
		void init();
		void inc();
		float getfps();
		void print();
};


CLbench::CLbench(xlong i,xchar f)
{
	interval = i*1000;
	flags = f;	
}

CLbench::~CLbench() { }

void CLbench::init()
{
	frames = 0;
	lastupdate = CLsystem::getmilliseconds();
	elapsedtime = 0;
}

void CLbench::inc()
{
	frames++;
	currenttime = CLsystem::getmilliseconds();
	elapsedtime += (currenttime - lastupdate);
	temptime = elapsedtime;
//nicht durch temptime teilen, da altes zu neuem und druch 2
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
			CLsystem::print("fps: ",0);
			CLsystem::print(framespersecond);
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

