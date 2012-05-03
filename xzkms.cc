
//#include <errno.h>
//#include <math.h>
//#include <stdint.h>
//#include <unistd.h>
//#include <sys/poll.h>
//#include <sys/time.h>
//#include <sys/ioctl.h>

#include <cstdlib>
#include <cstdio>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

void flush()
{

}

int main()
{
	//int fd = drmOpen("i915", NULL);
	int fd = open("/dev/dri/card0",O_RDWR); // | O_CLOEXEC
	if(fd<=0) { printf("Could not open card0!\n"); exit(1); }

	//Get Resource access
	drmModeRes* resources = drmModeGetResources(fd);
	if(resources==0) { printf("drmModeGetResources failed!\n"); exit(1); }
	//*

	int i = 0;

	//Seek connector and connect
	drmModeConnector* connector;
	for(;i<resources->count_connectors;++i)
	{
		connector = drmModeGetConnector(fd,resources->connectors[i]);
		if(connector==0) { continue; }
		if(connector->connection==DRM_MODE_CONNECTED && connector->count_modes>0) { break; }
		drmModeFreeConnector(connector); 
	}
	if(i==resources->count_connectors) { printf("No active connector found!\n"); exit(1); } 
	//*

	//Seek encoder and choose
	drmModeEncoder* encoder;
	for(i=0;i<resources->count_encoders;++i)
	{
		encoder = drmModeGetEncoder(fd,resources->encoders[i]);
		if(encoder==0) { continue; }
		if(encoder->encoder_id=connector->encoder_id) { break; }
		drmModeFreeEncoder(encoder);
	}
	if(i==resources->count_encoders) { printf("No active encoder found!\n"); exit(1); } 
	//*

	uint32_t fb = open("/dev/zero", O_RDWR);
	void* video = mmap(0,800*600*4,PROT_READ|PROT_WRITE,MAP_SHARED,fb,0);
	
	unsigned int width = 800;
	unsigned int height = 600;
	unsigned int stride = 0;	//todo: compute stride

	unsigned int fb_id = 0; 
	uint32_t crtc;

	//seek mode
	drmModeModeInfo mode;
	for(;i<connector->count_modes;++i)
	{
		mode = connector->modes[i];
		if( (mode.hdisplay==width) && (mode.vdisplay==height) ) { break; }
	}
	if(i==connector->count_modes) { printf("Requested mode not found!\n"); exit(1); }
	//*

	drmModeAddFB(fd,width,height,24,32,stride,fb,&fb_id);
	drmModeSetCrtc(fd,encoder->crtc_id,fb_id,0,0,&connector->connector_id,1,&mode);

	//try draw here
	long* frame = static_cast<long*>(video);
	frame[10*800+10] = 0x00FFFFFF;


	//close(fd);
	//drm_close(fd);

	return 0;
}

