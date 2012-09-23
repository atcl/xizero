///<header>
// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZskms.hh
// Super KMS library
#pragma once
//#pragma message "Compiling " __FILE__ "..." " TODO: mouse"
///</header>

///<include>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#include "XZbasic.hh"
#include "XZbuffer.hh"
#include "XZsystem.hh"
#include "XZmath.hh"
///<include>

///<declare>
#define BPP 32
#define FPS 4000

#define ESCAPE	27
#define ENTER	10
#define SPACE	32
#define UP	65
#define PGUP	53
#define LEFT	67
#define RIGHT	68
#define DOWN	66
#define PGDOWN	54

#define CLOSE		//CTRL+ESC

struct tile;
///</declare>

///<define>
namespace screen
{
	buffer frame(XRES*YRES,1);	//Video Memory Front Buffer
	buffer back(XRES*YRES,1);	//Video Memory Double Buffer
	buffer depth(XRES*YRES);	//System Memory Z-Buffer
	buffer accum(XRES*YRES);	//Accumulation/Triple Buffer

	namespace
	{
		tile* cs = 0;			//cursor image
		uint tk = 0;			//turbo key
		uint kk = 0;			//keyboard key
		uint mx = XRES/2;		//mouse horizontal position
		uint my = YRES/2;		//mouse vertical position
		bool mb = 0;			//mouse button pressed

		uint last = 0;
		char* nu = 0;

		termios nc;			//new terminal config
		termios oc;			//old terminal config

		uint fd;			//drm device handle
		uint id[3];			//framebuffer ids
		bool cc = 0;			//current framebuffer
		char* ev = new char[1024];	//event buffer

		drmModeRes* resources;		//resource array
		drmModeConnector* connector;	//connector array
		drmModeEncoder* encoder;	//encoder array
		drmModeModeInfo mode;		//video mode in use
		drmModeCrtcPtr crtc;
		drm_mode_create_dumb dumf{ YRES,XRES,BPP,0,0,0,0 };
		drm_mode_create_dumb dumb{ YRES,XRES,BPP,0,0,0,0 };

		drmModeModeInfo m800x600{ 40000,800,840,968,1056,0,600,601,605,628,0,60,0,0,"atcl:   800x600@60" }; //clock,hdisplay,hsync_start,hsync_end,htotal,hskew,vdisplay,vsync_start,vsync_end,vtotal,vsync,vrefresh((1000*clock)/(htotal*vtotal)),flags,type,name 
	}

	uint kbhit();
	void init(tile* c);
	void set(uint c=1,bool f=0);
	void vwait();
	void _flush()		{ frame.copy(back); drmModeDirtyFB(fd,id[cc],0,0); }
	void flush()		{ frame.swap(back); drmModePageFlip(fd,encoder->crtc_id,id[cc=!cc],DRM_MODE_PAGE_FLIP_EVENT,0); vwait(); }
	bool event();
	void close();
	void error(bool c,const char* m) { if(c) { system::say(m,1); screen::close(); system::bye(1); } }

	inline uint time()	{ return (1000*clock())/CLOCKS_PER_SEC; }
	void wait(uint k)	{ while(k!=kk) { event(); } }
	void sleep(sint t)	{ const sint e = clock() + (t * CLOCKS_PER_SEC)/1000; while(clock()< e) { ; } }
	uint fps(bool o=1)	{ static uint f=0; uint t=time(); f+=o; if(t>=last&&o==1) { last=t+FPS; t=f>>2; f=0; return t; } return -1; } 

	inline bool run()	{ flush(); return event(); }
	inline uint key()	{ const uint r=kk; kk=0; return r; }
	inline uint turbo()	{ return tk; }
	inline uint mousex()	{ return mx; }
	inline uint mousey()	{ return my; }
	inline uint mouseb()	{ return mb; }
	inline tile* cursor()	{ return cs; }
	//inline void smouse(uint x=XRES>>1,uint y=YRES>>1)    { mx=x; my=y; }
}
///</define>

///<code>
void screen::vwait()
{
	sint l=read(fd,ev,1024); 
	sint r=0;
	sint i=0;
	while(i<l && r==0 && l>=sizeof(drm_event))
	{
		drm_event* e=(drm_event*)(&ev[i]);
		r=(e->type==DRM_EVENT_FLIP_COMPLETE);
		i+=e->length+sizeof(drm_event);
	}
}

uint screen::kbhit()
{
	int c = 0;
	ioctl(0,FIONREAD,&c);
	timeval tv{ 0,0 };
	select(1,0,0,0,&tv);
	return c;
}

void screen::init(tile* c)
{
	cs = c;
	nu = new char[256];	//64 ok too?
	tcgetattr(STDIN_FILENO,&oc);
	nc = oc;
	nc.c_lflag &= ~(ICANON|ECHO); //ISIG
	//nc.c_cc[VMIN] = 4;
	//nc.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO,TCSANOW,&nc);
	last = time()+4000;
}

bool screen::event()
{
	const int r = kbhit();
	guard(r==0,(mb=0)==0); 

	const int s = 3-(r==1)-(r==2);
	packed t;
	read(0,&t,s);
	tk = kk = math::set(t.b[2],t.b[0],t.b[1]==91);
	read(0,&nu,math::min(r-s,256));

	mb = kk==SPACE;
	mx = math::lim(0,mx+((kk==LEFT)<<3)-((kk==RIGHT)<<3),XRES);
	my = math::lim(0,my+((kk==DOWN)<<3)-((kk==UP)<<3),YRES);

	return 1; //kk!=CLOSE;
}

void screen::set(uint c,bool f)
{
	//open default dri device
	fd = open("/dev/dri/card0",O_RDWR | O_CLOEXEC);
	error(fd<=0,"Error: Couldn't open /dev/dri/card0");
	//*

	//acquire drm resources
	resources = drmModeGetResources(fd);
	error(resources==0,"Error: drmModeGetResources failed\nMay be you are trying to start xizero from within X.\nIn this case switch to ttyX terminal.\nFor example with STRG+ALT+1 for tty1");
	//*

	//acquire original mode and framebuffer id
	crtc = drmModeGetCrtc(fd,resources->crtcs[0]);
	id[2] = crtc->buffer_id;
	//*

	int i;

	//acquire drm connector
	for(i=0;i<resources->count_connectors;++i)
	{
		connector = drmModeGetConnector(fd,resources->connectors[i]);
		if(connector==0 || c--!=0) { continue; }
		if(connector->connection==DRM_MODE_CONNECTED && connector->count_modes>0) { break; }
		drmModeFreeConnector(connector);
	}
	error(i==resources->count_connectors,"Error: No active connector found!"); 
	//*

	//acquire drm encoder
	for(i=0;i<resources->count_encoders;++i)
	{
		encoder = drmModeGetEncoder(fd,resources->encoders[i]);
		if(encoder==0) { continue; }
		if(encoder->encoder_id==connector->encoder_id) { break; }
		drmModeFreeEncoder(encoder);
	}
	error(i==resources->count_encoders,"Error: No active encoder found!");
	//*

	//check for requested mode
	for(i=0;i<connector->count_modes;++i)
	{
		mode = connector->modes[i];
		if( (mode.hdisplay==XRES) && (mode.vdisplay==YRES) ) { break; }
	}
	error(f==0 && i==connector->count_modes,"Error: Requested mode not found!");
	//*

	//force mode
	if(f==1 && i==connector->count_modes) { mode = m800x600; } //test for saved forcable modes
	//*

	//set up framebuffer
	i = drmIoctl(fd,DRM_IOCTL_MODE_CREATE_DUMB,&dumf);
	error(i==1,"Error: Could not create framebuffer object!");

	drm_mode_map_dumb dm{ dumf.handle,0,0 };
	i = drmIoctl(fd,DRM_IOCTL_MODE_MAP_DUMB,&dm);
	error(i==1,"Error: Could not map framebuffer object!");

	frame.pointer(mmap(0,dumf.size,PROT_READ | PROT_WRITE, MAP_SHARED,fd,dm.offset));
	error(frame.pointer()==MAP_FAILED,"Error: Could not map framebuffer memory!");

	i = drmModeAddFB(fd,XRES,YRES,BPP,BPP,dumf.pitch,dumf.handle,&id[0]);
	error(i==1,"Error: Could not add framebuffer!");
	//*

	//set up backbuffer
	i = drmIoctl(fd,DRM_IOCTL_MODE_CREATE_DUMB,&dumb);
	error(i==1,"Error: Could not create backbuffer object!");

	drm_mode_map_dumb dn{ dumb.handle,0,0 };
	i = drmIoctl(fd,DRM_IOCTL_MODE_MAP_DUMB,&dn);
	error(i==1,"Error: Could not map backbuffer object!");

	back.pointer(mmap(0,dumb.size,PROT_READ | PROT_WRITE, MAP_SHARED,fd,dn.offset));
	error(back.pointer()==MAP_FAILED,"Error: Could not map backbuffer memory!");

	i = drmModeAddFB(fd,XRES,YRES,BPP,BPP,dumb.pitch,dumb.handle,&id[1]);
	error(i==1,"Error: Could not add backbuffer!");
	//*

	i = drmModeSetCrtc(fd,encoder->crtc_id,id[cc],0,0,&connector->connector_id,1,&mode);
	error(i==1,"Error: Could not set mode!");

	sleep(1500);
}

void screen::close()
{
	//delete accum;
	//delete depth;
	//delete back;
	//delete frame;
	drmModeSetCrtc(fd,encoder->crtc_id,id[2],0,0,&connector->connector_id,1,&(crtc->mode)); 
	drmModeRmFB(fd,id[1]);
	drmModeRmFB(fd,id[0]);
	munmap(frame.pointer(),dumb.size);
	drm_mode_map_dumb db{ dumb.handle,0,0 };
	drmIoctl(fd,DRM_IOCTL_MODE_DESTROY_DUMB,&db);
	munmap(frame.pointer(),dumf.size);
	drm_mode_map_dumb df{ dumf.handle,0,0 };
	drmIoctl(fd,DRM_IOCTL_MODE_DESTROY_DUMB,&df);
	drmModeFreeEncoder(encoder);
	drmModeFreeConnector(connector);
	drmModeFreeResources(resources);
	::close(fd);
	tcsetattr(STDIN_FILENO,TCSANOW,&oc);
	delete nu;
	delete ev;
}
///</code>

