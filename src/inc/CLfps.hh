//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLFPS
#define HH_CLFPS
#pragma message "Compiling " __FILE__ " ! TODO: check if timeinterval coorect"

#include <time.h>
#include "CLapi.hh"


class CLfps
{
	private:
		static xlong version;
		float framespersecond;
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
		xlong getversion();
};

xlong CLfps::version = 0x00010000;

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
	lastupdate = clock();
	elapsedtime = 0;
}

void CLfps::increment()
{
	frames++;
	currenttime = clock();
	elapsedtime += (currenttime - lastupdate);
	temptime = (elapsedtime / CLOCKS_PER_SEC);

	if( temptime >= interval)
	{
		framespersecond = (frames/temptime);

		if(output==true) cout << "fps: " << framespersecond << endl;

		frames = 0;
		elapsedtime = 0;
	}

	lastupdate = currenttime;
}

float CLfps::get()
{
	return framespersecond;
}

xlong CLfps::getversion()
{
	return version;
}

#endif
