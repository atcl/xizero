//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license
#ifndef HH_CLEXPLOSION
#define HH_CLEXPLOSION
#pragma message "Compiling " __FILE__ " ! TODO: all"

//combi aus "translate along normals" + "dyadic product"
class CLexplosion
{
	protected:
		static xlong version;
		float last_time;
		float curr_time;
		xlong interval;

		CLobject* object;

	public:
		CLexplosion(CLobject* o);
		~CLexplosion();

		void start();
		xlong getversion();
};

xlong CLexplosion::version = 0x00010000;

CLexplosion::CLexplosion(CLobject* o)
{
	object = o;
}

CLexplosion::~CLexplosion() { }

void CLexplosion::start()
{

}

xlong CLexplosion::getversion()
{
	return version;
}

#endif
