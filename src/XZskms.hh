// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZskms.hh
// Super KMS library

///guard
#ifndef HH_XZSKMS
#define HH_XZSKMS
//#pragma message "Compiling " __FILE__ "..." " TODO: ."
///*

///includes
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include "XZbasic.hh"
#include "XZbuffer.hh"
#include "XZsystem.hh"
#include "XZmath.hh"
///*

///declarations
#define BPP 32
///*

///definitions
namespace skms
{
	buffer front(XRES*YRES,0);	//Video Memory Front Buffer
	buffer back(XRES*YRES);		//System Memory Double Buffer
	buffer depth(XRES*YRES);	//Z-Buffer
	buffer accum(XRES*YRES);	//Accumulation/Triple Buffer

	namespace
	{
		sint keys[2] = { 0,0 };
		sint mouse[4] = { 0,0,0,0 };	
		void* mcursor = 0;

		uint ed;			//input event device handle
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
	void init(void* c=0) 	{ mcursor = c; ed = open("/dev/input/event0",O_RDONLY); }
	void set(uint c,bool f=0);
	void flush()		{ /*copy back.ptr*/ drmModeDirtyFB(fd,id,0,0); }
	void event();
	bool run()		{ flush(); event(); return 0; }
	void close();

	void wait(sint k)	{ while(k!=keys[1]) { event(); } }
	void sleep(sint t)	{ const uint e = clock() + (t * CLOCKS_PER_SEC)/1000; while(clock()< e) { ; } }
	uint time()		{ return (CLOCKS_PER_SEC*1000)*clock(); }
	uint fps(bool o=1)	{ static sint f=0; static sint l=time()+4000; sint t=time(); f+=o; if(t>=l&&o==1) { l=t+4000; t=f>>2; f=0; return t; } return -1; } 

	inline sint  key()	{ const sint k = keys[1]; keys[1] = 0; return k; }
	inline sint  turbo()	{ return keys[0]; }
	inline sint  mousex()	{ return mouse[2]; }
	inline sint  mousey()	{ return mouse[3]; }
	inline sint  mouseb()	{ const sint b = mouse[0]; mouse[0] = 0; return b; }
	inline void* cursor()	{ return mcursor; }
}
///*

///implementation
void skms::event()
{
	struct input_event event;
	read(fd,&event,sizeof(struct input_event));
	keys[1] = keys[0] = math::set(event.code,event.type==EV_KEY&&event.value==1);
	keys[1] = keys[0] = math::set(event.code,event.type==EV_KEY&&event.value==0);
	mouse[0] = math::set(event.code==BTN_LEFT,event.type==EV_KEY&&event.value==1);
	mouse[1] = math::set(event.code==BTN_RIGHT,event.type==EV_KEY&&event.value==1);
	mouse[2] = math::set(event.code,event.type==EV_REL&&event.value==REL_X); 
	mouse[3] = math::set(event.code,event.type==EV_REL&&event.value==REL_Y);
}

void skms::set(uint c,bool f)
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

void skms::close()
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
	::close(ed);
}
///*

#endif

