//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLGAMEPAD
#define HH_CLGAMEPAD
//#pragma message "Compiling " __FILE__

#ifdef WIN32
	#include <mmsystem.h>
#else //ifdef LINUX
	#include <fcntl.h>
	#include <sys/ioctl.h>
	#include <linux/joystick.h>
#endif

	#include "CLtypes.hh"


namespace CLgamepad
{
	int gamepad_device = 0;
	
	#ifdef WIN32
		struct JOYINFO gp;
	#else //ifdef LINUX
		struct js_event gp;	
	#endif

	xlong axis[2];
	bool  button[10];
	bool  tbutton[10];
	
	bool init();
	void mask();
	void handle();
	void exit();
};

#ifdef WIN32

bool CLgamepad::init()
{
	axis[0] = axis[1] = 0;
	button[0] = button[1] = button[2] = button[3] = button[4] = button[5] = button[6] = button[7] = button[8] = button[9] = 0;
	tbutton[0] = tbutton[1] = tbutton[2] = tbutton[3] = tbutton[4] = tbutton[5] = tbutton[6] = tbutton[7] = tbutton[8] = tbutton[9] = 0;
	
	if(joyGetNumDevs() == -1)
	{
		CLsystem::print("No Gamepad driver found");
		return 0;
	}
	
	if(joygetPos(0,&gp)==JOYERR_UNPLUGGED)
	{
		CLsystem::print("No Gamepad found");
		return 0;
	}
	
	gamepad_device = 0;
		
	return 1;
}

void CLgamepad::mask()
{
	
}

void CLgamepad::handle()
{
	joyGetPos(JOYSTICKID1,&gp);
	axis[0] = gp.wXpos;
	axis[1] = gp.wYpos;
	button[0] = (gp.wButtons & JOY_BUTTON1) > 0;
	button[1] = (gp.wButtons & JOY_BUTTON2) > 0;
	button[2] = (gp.wButtons & JOY_BUTTON3) > 0;
	button[3] = (gp.wButtons & JOY_BUTTON4) > 0;
	button[4] = (gp.wButtons & JOY_BUTTON5) > 0;
	button[5] = (gp.wButtons & JOY_BUTTON6) > 0;
	button[6] = (gp.wButtons & JOY_BUTTON7) > 0;
	button[7] = (gp.wButtons & JOY_BUTTON8) > 0;
	button[8] = (gp.wButtons & JOY_BUTTON9) > 0;
	button[9] = (gp.wButtons & JOY_BUTTON10) > 0;
}

void CLgamepad::exit() { }

#else //ifdef LINUX

bool CLgamepad::init()
{
	axis[0] = axis[1] = 0;
	button[0] = button[1] = button[2] = button[3] = button[4] = button[5] = button[6] = button[7] = button[8] = button[9] = 0;
	tbutton[0] = tbutton[1] = tbutton[2] = tbutton[3] = tbutton[4] = tbutton[5] = tbutton[6] = tbutton[7] = tbutton[8] = tbutton[9] = 0;
	
	if( (gamepad_device = open("/dev/input/js0",O_RDONLY)) == -1)
	{
		CLsystem::print("No Gamepad found");
		return 0;
	}
	
	fcntl(gamepad_device,F_SETFL,O_NONBLOCK);
	
	return 1;
}

void CLgamepad::mask()
{
	
}

void CLgamepad::handle()
{
	read(gamepad_device,&gp,sizeof(struct js_event));
	
	switch (gp.type & ~JS_EVENT_INIT)
    {
		case JS_EVENT_AXIS:
			if(gp.number>1) break;
			axis[gp.number] = gp.value;
			
			break;
		case JS_EVENT_BUTTON:
			if(gp.number>9) break;
			button[gp.number] = gp.value;
			tbutton[gp.number] = gp.value;
			break;
    }
}

void CLgamepad::exit()
{
	close(gamepad_device);
}

#endif

#endif

