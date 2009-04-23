//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLENTITY
#define HH_CLENTITY
#pragma message "Compiling " __FILE__ " ! TODO: ..."


class CLentity
{
	private:
		static xlong version;
		CLobject* model;

		xlong xcoord;
		xlong ycoord;
		xlong width;
		xlong height;
		xlong depth;
		xlong xdelay;
		xlong ydelay;
		xlong xcount;
		xlong ycount;

		xlong ammotype;
		xlong ammodirection;
		xlong firerate;
		
		xlong health;
		xlong shield;
		xlong armor;

		xlong points;
		
	public:
		CLentity();
		~CLentity();
		void draw();
		void fire();
		void hurt();
		void collision();
		xlong getversion();
};

xlong CLentity::version = 0x00010000;

CLentity::CLentity()
{

}

CLentity::~CLentity() { }



xlong CLentity::getversion()
{
	return version;
} 

#endif
