//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFPS
#define HH_CLFPS
#warning "Compiling " __FILE__ " ! TODO: check if timeinterval correct"

#include "CLtypes.hh"
#include "CLcl.hh"
#include "CLapi.hh"


class CLfps : public virtual CLcl
{
	private:
		xlong framespersecond;
		float currenttime;
		float elapsedtime;
		float temptime;
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
	interval = i;
	output = o;
}

CLfps::CLfps(xlong i,bool o,xlong* outtoo)
{
	interval = i;
	output = o;
	out = outtoo;
}

CLfps::~CLfps() { }

void CLfps::init()
{
	frames = 0;
	lastupdate = CLgetmilliseconds_();
	elapsedtime = 0;
}

void CLfps::increment()
{
	frames++;
	currenttime = CLgetmilliseconds_();
	elapsedtime += (currenttime - lastupdate);
	temptime = (elapsedtime / 1000);

	if(temptime >= interval)
	{
		framespersecond = xlong(frames/temptime);

		if(output==true)
		{
			CLttyout_("fps: ");
			CLprint_(framespersecond);
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

