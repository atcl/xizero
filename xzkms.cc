#include <xf86drmMode.h>

void flush()
{

}

int main()
{

	int fd = open("/dev/dri/card0",O_RDWR);

	drmModeRes* resources = drmModeGetResources(fd);
	drmModeConnector* connector;
	drmModeEncodeder* encoder;

	if(resources==0) { printf("drmModeGetResources failed!\n"); exit(1); }

	int i = 0;
	for(;i<resources->count_connectors;++i)
	{
		connector = drmModeGetConnector(fd,resources->connector[i]);
		if(connector==0) { continue; }
		if(connector->connection==DRM_MODE_CONNECTED && connector->count_modes>0) { break; }
		drmModeFreeConnector(connector); 
	}

	if(i==resources->count_connectors) { printf("No active connector found!\n"); } 

	for(i=0;i<resources->count_encoders;++i)
	{
		encoder = drmModeGetEncoder(fd,resources->encoders[i]);
		if(encoder==0) { continue; }
		if(encoder->encoder_id=connector->encoder_id) { break; }
		drmModeFreeEncoder(encoder);
	}

	uint32_t fb = open("/dev/zero", O_RDWR);
	void* video = mmap(0,800*600*4,PROT_READ|PROT_WRITE,MAP_SHARED,fb,0);
	
	stride = 	//todo: compute stride

	unsigned int fb_id = 0; 
	uint32_t crtc;

	drmModeAddFB(fd,width,height,24,32,stride,fb,&fb_id);
	drmModeSetCrtc(fd,encoder->crtc_id,fb_id,0,0,connector->connector_id,1,mode);

	//try draw here

	return 0;
}
