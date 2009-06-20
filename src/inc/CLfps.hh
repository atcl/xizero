//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFPS
#define HH_CLFPS
#pragma message "Compiling " __FILE__ " ! TODO: check if timeinterval correct"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLapi.hh"


class CLfps : public virtual CLcl
{
	private:
		xlong framespersecond;
		xlong currenttime;
		xlong elapsedtime;
		xlong temptime;
		xlong frames;
		xlong lastupdate;
		xlong interval;
		bool output;
		xlong* out;
	public:
		CLfps(xlong i,bool o);
		CLfps(xlong i,bool o,xlong* outtoo);
		~CLfps();
		void init();
		void increment();
		float get();
};

CLfps::CLfps(xlong i,bool o)
{
	interval = i*1000;
	output = o;
}

CLfps::CLfps(xlong i,bool o,xlong* outtoo)
{
	interval = i*1000;
	output = o;
	out = outtoo;
}

CLfps::~CLfps() { }

void CLfps::init()
{
	frames = 0;
	lastupdate = CLsystem::CLgetmilliseconds_();
	elapsedtime = 0;
}

void CLfps::increment()
{
	frames++;
	currenttime = CLsystem::CLgetmilliseconds_();
	elapsedtime += (currenttime - lastupdate);
	temptime = elapsedtime;

	if(temptime >= interval)
	{
		framespersecond = xlong(frames*1000/temptime);

		if(output==true)
		{
			CLsystem::CLprint_("fps: ",0);
			CLsystem::CLprint_(framespersecond);
			//cout << "fps: " << framespersecond << endl;
		}

		frames = 0;
		elapsedtime = 0;
	}

	lastupdate = currenttime;
}

float CLfps::get()
{
	return framespersecond;
}

#endif

