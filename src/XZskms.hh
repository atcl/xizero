// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZskms.hh
// Super KMS library

///guard
#ifndef HH_XZSKMS
#define HH_XZSKMS
//#pragma message "Compiling " __FILE__ "..." " TODO: mouse"
///*

///includes
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <termios.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include "XZbasic.hh"
#include "XZbuffer.hh"
#include "XZsystem.hh"
#include "XZmath.hh"
///*

///declarations
#define BPP 32

#define ESCAPE	1
#define ENTER	28
#define LCTRL	29
#define SPACE	57
#define RCTRL	97
#define UP	103
#define PGUP	104
#define LEFT	105
#define RIGHT	106
#define DOWN	108
#define PGDOWN	109

struct tile;
///*

///definitions
namespace screen
{
	buffer front(XRES*YRES,0);	//Video Memory Front Buffer
	buffer back(XRES*YRES);		//System Memory Double Buffer
	buffer depth(XRES*YRES);	//Z-Buffer
	buffer accum(XRES*YRES);	//Accumulation/Triple Buffer

	namespace
	{
		tile* cs = 0;
		uint kk = 0;
		uint mx = XRES/2;
		uint my = YRES/2;
		bool mb = 0;

		uint last = 0;

		uint ed;			//input event device handle
		termios nc;			//new terminal config
		termios oc;			//old terminal config

		uint fd;			//drm device handle
		uint handle;			//handle to framebuffer
		uint size;			//size of framebuffer
		uint pitch;			//stride
		uint id;			//framebuffer id
		uint oid;			//old framebuffer id

		drmModeRes* resources;		//resource array
		drmModeConnector* connector;	//connector array
		drmModeEncoder* encoder;	//encoder array
		drmModeModeInfo mode;		//video mode in use
		drmModeCrtcPtr crtc;

		drmModeModeInfo m800x600 = { 40000,800,840,968,1056,0,600,601,605,628,0,60/*(40000*1000)/(1056*628)*/,0,0,0 }; //clock,hdisplay,hsync_start,hsync_end,htotal,hskew,vdisplay,vsync_start,vsync_end,vtotal,vsync,vrefresh((1000*clock)/(htotal*vtotal)),flags,type,name 
	}

	void error(bool c,const char* m) { if(c) { system::say(m,1); system::bye(1); } }
	void init(tile* c);
	void set(uint c,bool f=0);
	void flush()		{ front.copy(back,XRES*YRES); drmModeDirtyFB(fd,id,0,0); }
	void event();
	bool run()		{ flush(); event(); return 0; }
	void close();

	inline uint time()	{ return (CLOCKS_PER_SEC*1000)*clock(); }
	void wait(sint k)	{ while(k!=kk) { event(); } }
	void sleep(sint t)	{ const uint e = clock() + (t * CLOCKS_PER_SEC)/1000; while(clock()< e) { ; } }
	uint fps(bool o=1)	{ static uint f=0; uint t=time(); f+=o; if(t>=last&&o==1) { last=t+4000; t=f>>2; f=0; return t; } return -1; } 

	inline uint key()	{ return kk; }
	inline uint mousex()	{ return mx; }
	inline uint mousey()	{ return my; }
	inline uint mouseb()	{ return mb; }
	inline tile* cursor()	{ return cs; }
	//inline void mousec()    { mx = XRES/2; my=YRES/2; }
}
///*

///implementation
void screen::init(tile* c)
{
	cs = c;
	ed = fileno(stdin);
	tcgetattr(ed,&oc);
	nc = oc;
	nc.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(ed,TCSANOW,&nc);
	last = time()+4000;
}

void screen::event()
{
	kk = getchar();
	mb=(kk==SPACE);
	mx=math::lim(0,mx+(kk==RIGHT)-(kk==LEFT),XRES);
	my=math::lim(0,my+(kk==RIGHT)-(kk==LEFT),YRES);
}

void screen::set(uint c,bool f)
{
	//open default dri device
	fd = open("/dev/dri/card0",O_RDWR | O_CLOEXEC);
	error(fd<=0,"Couldn't open /dev/dri/card0");
	//*

	//drmSetMaster(fd);

	//acquire drm resources
	resources = drmModeGetResources(fd);
	error(resources==0,"drmModeGetResources failed");
	//*

	//acquire original mode and framebuffer id
	crtc = drmModeGetCrtc(fd,resources->crtcs[0]);
	oid = crtc->buffer_id;
	//*

	int i;

	//acquire drm connector //todo: choose connector
	for(i=0;i<resources->count_connectors;++i)
	{
		connector = drmModeGetConnector(fd,resources->connectors[i]);
		if(connector==0 || c--!=0) { continue; }
		if(connector->connection==DRM_MODE_CONNECTED && connector->count_modes>0) { break; }
		drmModeFreeConnector(connector);
	}
	error(i==resources->count_connectors,"No active connector found!"); 
	//*

	//acquire drm encoder
	for(i=0;i<resources->count_encoders;++i)
	{
		encoder = drmModeGetEncoder(fd,resources->encoders[i]);
		if(encoder==0) { continue; }
		if(encoder->encoder_id==connector->encoder_id) { break; }
		drmModeFreeEncoder(encoder);
	}
	error(i==resources->count_encoders,"No active encoder found!");
	//*

	//check for requested mode
	for(i=0;i<connector->count_modes;++i)
	{
		mode = connector->modes[i];
		if( (mode.hdisplay==XRES) && (mode.vdisplay==YRES) ) { break; }
	}
	error(f==0 && i==connector->count_modes,"Requested mode not found!");
	//*

	//force mode
	if(f==1 && i==connector->count_modes) { mode = m800x600; } //test for saved forcable modes
	//*

	//setup framebuffer
	struct drm_mode_create_dumb dc = { YRES,XRES,BPP,0,0,0,0 };
	i = drmIoctl(fd,DRM_IOCTL_MODE_CREATE_DUMB,&dc);
	error(i==1,"Could not create buffer object!");

	size = dc.size;
	pitch = dc.pitch;
	handle = dc.handle;

	struct drm_mode_map_dumb dm = { handle,0,0 };
	i = drmIoctl(fd,DRM_IOCTL_MODE_MAP_DUMB,&dm);
	error(i==1,"Could not map buffer object!");

	void* ptr = mmap(0,size,PROT_READ | PROT_WRITE, MAP_SHARED,fd,dm.offset);
	error(ptr==MAP_FAILED,"Could not mirror buffer object!");
	front.pointer(ptr);
	//*

	i = drmModeAddFB(fd,XRES,YRES,BPP,BPP,pitch,handle,&id);
	error(i==1,"Could not add framebuffer!");

	i = drmModeSetCrtc(fd,encoder->crtc_id,id,0,0,&connector->connector_id,1,&mode);
	error(i==1,"Could not set mode!");
}

void screen::close()
{
	//back.~buffer();
	drmModeSetCrtc(fd,encoder->crtc_id,oid,0,0,&connector->connector_id,1,&(crtc->mode)); 
	drmModeRmFB(fd,id);
	munmap(front.pointer(),size);
	struct drm_mode_map_dumb dd = { handle };
	drmIoctl(fd,DRM_IOCTL_MODE_DESTROY_DUMB,&dd);
	drmModeFreeEncoder(encoder);
	drmModeFreeConnector(connector);
	drmModeFreeResources(resources);
	//drmDropMaster(fd);
	::close(fd);
	tcsetattr(ed,TCSANOW,&oc);
}
///*

#endif

