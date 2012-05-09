// atCROSSLEVEL 2010,2011,2012
// released under zlib/libpng license
// XZkms.cc
// kms testing ground

#include <time.h>

#include <sys/mman.h>

#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

#include "src/XZbasic.hh"
#include "src/XZsystem.hh"
#include "src/XZmath.hh"

namespace kms
{
	namespace
	{
		unsigned int ed;		//input event device handle
		sint  mouse[4] = { 0,0,0,0 };	
		sint  keys[2] = { 0,0 };

		unsigned int fd;		//drm device handle
		unsigned int width;		//screen width in pixels
		unsigned int height;		//screen height in pixels
		unsigned int bpp;		//bits per pixels
		unsigned int handle;		//handle to framebuffer
		unsigned int size;		//size of framebuffer
		unsigned int pitch;		//stride
		unsigned int id;		//framebuffer id
		unsigned int oid;		//old framebuffer id

		void* ptr;			//pointer to memory mirror of framebuffer

		drmModeRes* resources;		//resource array
		drmModeConnector* connector;	//connector array
		drmModeEncoder* encoder;	//encoder array
		drmModeModeInfo mode;		//video mode in use
		drmModeCrtcPtr crtc;
	}

	void init();
	void error(bool c,const char* m);
	void* setmode(int w,int h,int c,bool f);
	void flush();
	void sleep(int s);
	void restore();
}

void kms::init()
{
	ed = open("/dev/input/event1",O_RDONLY);

	while(true)
	{
		struct input_event event;
		int num_bytes = read(fd, &event, sizeof(struct input_event));

		keys[1] = keys[0] = math::set(event.code,event.type==EV_KEY&&event.value==1);
		mouse[0] = math::set(event.code==BTN_LEFT,event.type==EV_KEY&&event.value==1);
		mouse[1] = math::set(event.code==BTN_RIGHT,event.type==EV_KEY&&event.value==1);
		mouse[2] = math::set(event.code,event.type==EV_ABS&&event.value==ABS_X); 
		mouse[3] = math::set(event.code,event.type==EV_ABS&&event.value==ABS_Y);
		system::say((char*)&keys[0],1);
		if(keys[0]=='q') { break; }
	}

	close(ed);
}

void kms::sleep(int s)
{
	clock_t endwait;
	endwait = clock () + s * CLOCKS_PER_SEC ;
	while(clock() < endwait) { ; }
}

void kms::flush()
{
	struct input_event event;
	read(fd, &event, sizeof(struct input_event));
	keys[1] = keys[0] = math::set(event.code,event.value==1);


	drmModeDirtyFB(fd,id,0,0);
}

void kms::error(bool c,const char* m)
{
	if(c) { system::say(m,1); system::bye(1); }
}

void* kms::setmode(int w,int h,int c,bool f)
{
	width = w;
	height = h;

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
		if(connector==0) { continue; }
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
		if( (mode.hdisplay==width) && (mode.vdisplay==height) ) { break; }
	}
	error(i==connector->count_modes,"Requested mode not found!");
	//*

	//force mode
	//...
	//*

	//setup framebuffer
	struct drm_mode_create_dumb dc = { height,width,32,0,0,0,0 };
	i = drmIoctl(fd,DRM_IOCTL_MODE_CREATE_DUMB,&dc);
	error(i==1,"Could not create buffer object!");

	size = dc.size;
	pitch = dc.pitch;
	handle = dc.handle;

	struct drm_mode_map_dumb dm = { handle,0,0 };
	i = drmIoctl(fd,DRM_IOCTL_MODE_MAP_DUMB,&dm);
	error(i==1,"Could not map buffer object!");

	ptr = mmap(0,size,PROT_READ | PROT_WRITE, MAP_SHARED,fd,dm.offset);
	error(ptr==MAP_FAILED,"Could not mirror buffer object!");
	//*

	i = drmModeAddFB(fd,width,height,32,32,pitch,handle,&id);
	error(i==1,"Could not add framebuffer!");

	i = drmModeSetCrtc(fd,encoder->crtc_id,id,0,0,&connector->connector_id,1,&mode);
	error(i==1,"Could not set mode!");

	return ptr;
}

void kms::restore()
{
	drmModeSetCrtc(fd,encoder->crtc_id,oid,0,0,&connector->connector_id,1,&(crtc->mode)); 
	drmModeRmFB(fd,id);
	munmap(ptr,size);
	struct drm_mode_map_dumb dd = { handle };
	drmIoctl(fd,DRM_IOCTL_MODE_DESTROY_DUMB,&dd);
	drmModeFreeEncoder(encoder);
	drmModeFreeConnector(connector);
	drmModeFreeResources(resources);
	//drmDropMaster(fd);
	close(fd);
}

int main()
{
	kms::init();

	long* frame = static_cast<long*>(kms::setmode(1024,600,1,0));

	for(int i=0;i<600;++i) { for(int j=0;j<1024;++j) { frame[i*1024+j] = i*j; } }

	kms::flush();

	kms::sleep(6);

	kms::restore();

	return 0;
}

