//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLGAMEPAD
#define HH_CLGAMEPAD
#pragma message "Compiling " __FILE__

#ifdef WIN32
	#include <mmsystem.h>
#else //ifdef LINUX
	#include <fcntl.h>
	#include <sys/ioctl.h>
	#include <linux/joystick.h>
#endif

#include "CLtypes.hh"
#include "CLstruct.hh"

class CLgamepad : public virtual CLcl, public CLsingle<CLgamepad>
{
	friend class CLsingle<CLgamepad>;
	
	private:
		xlong device;
		CLgamepad();
		~CLgamepad() { };
		
		#ifdef WIN32
			struct JOYINFO gp;
		#else //ifdef LINUX
			struct js_event gp;	
		#endif

		CLgamepadstate pad;

	public:
		void mask();
		void handle();
		void exit();
		bool isavail();
		CLgamepadstate* getstate();
};

#ifdef WIN32

CLgamepad::CLgamepad()
{
	//check if gamepad driver is installed
	if(joyGetNumDevs() == -1)
	{
		clsystem->print("No Gamepad driver found");
		device = -1;
		return;
	}
	//*
	
	//check if gamepad is connected
	if(joygetPos(0,&gp)==JOYERR_UNPLUGGED)
	{
		clsystem->print("No Gamepad found");
		device = -1;
		return;
	}
	//*
	
	device = 0;
}

void CLgamepad::mask()
{
	
}

void CLgamepad::handle()
{
	//read gamepad state
	joyGetPos(JOYSTICKID1,&gp);
	//*
	
	//copy axis state
	pad.axis[0] = gp.wXpos;
	pad.axis[1] = gp.wYpos;
	//*
	
	//copy button state
	pad.button[0] = (gp.wButtons & JOY_BUTTON1) > 0;
	pad.button[1] = (gp.wButtons & JOY_BUTTON2) > 0;
	pad.button[2] = (gp.wButtons & JOY_BUTTON3) > 0;
	pad.button[3] = (gp.wButtons & JOY_BUTTON4) > 0;
	pad.button[4] = (gp.wButtons & JOY_BUTTON5) > 0;
	pad.button[5] = (gp.wButtons & JOY_BUTTON6) > 0;
	pad.button[6] = (gp.wButtons & JOY_BUTTON7) > 0;
	pad.button[7] = (gp.wButtons & JOY_BUTTON8) > 0;
	pad.button[8] = (gp.wButtons & JOY_BUTTON9) > 0;
	pad.button[9] = (gp.wButtons & JOY_BUTTON10) > 0;
	//*
}

void CLgamepad::exit() { }

#else //ifdef LINUX

CLgamepad::CLgamepad()
{	
	//check if gamepad device exists and so a gamepad is connected
	if( (device = open("/dev/input/js0",O_RDONLY)) == -1)
	{
		clsystem->print("No Gamepad found");
		return;
	}
	//*
	
	fcntl(device,F_SETFL,O_NONBLOCK);
}

void CLgamepad::mask()
{
	
}

void CLgamepad::handle()
{
	//read gamepad state
	read(device,&gp,sizeof(struct js_event));
	//*
	
	switch (gp.type & ~JS_EVENT_INIT)
    {
		//copy axis state
		case JS_EVENT_AXIS:
			if(gp.number>1) break;
			pad.axis[gp.number] = gp.value;
		break;
		//*
		
		//copy button state
		case JS_EVENT_BUTTON:
			if(gp.number>9) break;
			pad.button[gp.number] = gp.value;
			pad.tbutton[gp.number] = gp.value;
		break;
		//*
    }
}

void CLgamepad::exit() { close(device); }

#endif

bool CLgamepad::isavail() { return (device!=-1); }

CLgamepadstate* CLgamepad::getstate() { return &pad; }

#endif

