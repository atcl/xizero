//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include <time.h>
#include <iostream>

#include "asciimissile.hh"

#include <sys/select.h>

int kbhit(void)
{
	struct timeval tv;
	fd_set read_fd;
	
	tv.tv_sec=0;
	tv.tv_usec=0;
	FD_ZERO(&read_fd);
	FD_SET(0,&read_fd);
	
	if(select(1, &read_fd, NULL, NULL, &tv) == -1) return 0;
	
	if(FD_ISSET(0,&read_fd)) return 1;
	
	return 0;
}

int main()
{
	screen  *myscreen  = new screen(80,25);
	level   *mylevel   = new level("level.txt",myscreen->getwidth(),myscreen->getheight());
	missile *mymissile = new missile();
	game    *mygame    = new game();

	//SCREEN *s = newterm( "new", stdout, stdin );
	//set_term(s);

	//game loop
	int ch;
	bool running = true;
	float last_time = clock();
	float curr_time = 0;
	float elap_time = 0;
	float temp_time = 0;
	float velocity = CLOCKS_PER_SEC; //meaning every second one mark update
	while(running==true)
	{
		//input
		if(kbhit())
		{
			ch =  getch();
			switch(ch)
			{	
				case KEY_LEFT:
					if(!mygame->boundarydetection(mymissile->getx()-1,mymissile->gety(),80,24)) break;
					mymissile->addtox(-1);
				break;
				case KEY_RIGHT:
					if(!mygame->boundarydetection(mymissile->getx()+1,mymissile->gety(),80,24)) break;
					mymissile->addtox(1);
				break;
				case KEY_UP:
					if(!mygame->boundarydetection(mymissile->getx(),mymissile->gety()-1,80,24)) break;
					mymissile->addtoy(-1);
				break;
				case KEY_DOWN:
					if(!mygame->boundarydetection(mymissile->getx(),mymissile->gety()+6,80,24)) break;
					mymissile->addtoy(1);
				break;
				case '0':
					running = 0;
				break;
			}
		}	

		//output
		myscreen->clear();

		curr_time = clock();
		elap_time += (curr_time - last_time);
		temp_time = elap_time / CLOCKS_PER_SEC;
		if( temp_time >=  0.25 )
		{
			mylevel->addtomark(-1);
			last_time = curr_time;
			//cout << "hi" << endl;
			elap_time = 0;
		}
		last_time = curr_time;

		mylevel->display(myscreen->getwindow());
		mymissile->display(myscreen->getwindow());
		mygame->displaystats(myscreen->getwindow());
		myscreen->flush();
	 }

	//exit;
	return 0;
}


