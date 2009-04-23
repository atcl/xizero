//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license

#include <ncurses.h>
#include <fstream>

using namespace std;

class missile
{
	private:
		long x;
		long y;
		long health;
	public:
		missile();
		~missile();

		void addtox(long xx);
		void addtoy(long yy);
		void addtohealth(long h);
		long getx();
		long gety();
		long gethealth();
		void display(WINDOW *win);
};

missile::missile()
{
	x = 10;
	y = 10;
}

missile::~missile()
{

}

void missile::addtox(long xx)
{
	x += xx;
}

void missile::addtoy(long yy)
{
	y += yy;
}

void missile::addtohealth(long h)
{
	health += h;
}

long missile::getx()
{
	return x;
}

long missile::gety()
{
	return y;
}

long missile::gethealth()
{
	return health;
}

void missile::display(WINDOW* win)
{
	wattron(win,A_BOLD);
	wattron(win,COLOR_PAIR(1));
	wmove(win,y,x);   waddch(win,'A'); wattroff(win,COLOR_PAIR(1)); wattron(win,COLOR_PAIR(2));
	wmove(win,y+1,x); waddch(win,'H');
	wmove(win,y+2,x); waddch(win,'H'); wattroff(win,COLOR_PAIR(2)); wattron(win,COLOR_PAIR(1));
	wmove(win,y+3,x); waddch(win,'M'); wattroff(win,COLOR_PAIR(1)); wattron(win,COLOR_PAIR(4));
	wmove(win,y+4,x); waddch(win,'^');
	wmove(win,y+5,x); waddch(win,'^');
	wattroff(win,COLOR_PAIR(4));
	wattroff(win,A_BOLD);
}

class level
{
	private:
		char** levelmap;
		long levelwidth;
		long levelheight;
		long levellength;
		long mark;
		long screenwidth;
		long screenheight;
	public:
		level(const char* leveltxt,long sw,long sh);
		~level();
		bool addtomark(long m);
		void display(WINDOW* win);
};

level::level(const char* leveltxt,long sw,long sh)
{
	levelwidth = 80;
	levelheight = 25;
	levellength = 75;

	screenwidth = sw;
	screenheight = sh;

	FILE* levelfile;
	levelfile = fopen(leveltxt,"r");
	if(levelfile==NULL)
	{
		//exit(1);
	}
	else
	{
		levelmap = new char*[levellength];
		for(int i=0; i<levellength; i++)
		{
			levelmap[i] = new char[levelwidth];
			fgets(levelmap[i],levelwidth+2,levelfile);
		}
		fclose(levelfile);
	}
	mark = (levellength - levelheight);
}

level::~level()
{

}

bool level::addtomark(long m)
{
	if( (mark + m) > 0)
	{
		mark += m;
		return 0;
	}
	else
	{
		return 1;
	}
}

void level::display(WINDOW* win)
{
	for(int i=0; i<screenheight-1; i++)
	{
		for(int j=0; j<screenwidth; j++)
		{
			wmove(win,i,j);
			if(levelmap[mark + i][j] != '.') waddch(win,levelmap[mark + i][j]);
		}
	}
}

class screen
{
	private:
		WINDOW* win;
		long width;
		long height;
	public:
		screen(long w,long h);
		~screen();
		long getwidth();
		long getheight();
		WINDOW* getwindow();
		void clear();
		void flush();
};

screen::screen(long w,long h)
{
	width = w;
	height = h;

	initscr();
	win = newwin(h,w,0,0);

	nodelay(win, TRUE);
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);

	start_color();
	init_pair(1, COLOR_WHITE,  COLOR_BLACK);
	init_pair(2, COLOR_RED,    COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_GREEN,  COLOR_BLACK);
	init_pair(6, COLOR_BLUE,   COLOR_BLACK);
	init_pair(8, COLOR_WHITE,   COLOR_RED);
	//init_pair(7, COLOR_GREY,   COLOR_BLACK);
	//init_pair(3, COLOR_ORANGE, COLOR_BLACK);

	clear();
	wclear(win);
}

screen::~screen()
{

}

long screen::getwidth()
{
	return width;
}

long screen::getheight()
{
	return height;
}

WINDOW* screen::getwindow()
{
	return win;
}

void screen::clear()
{
	wclear(win);
}

void screen::flush()
{
	wrefresh(win);
}

class game
{
	private:

	public:
		game();
		~game();
		void displayintro(WINDOW* win);
		void displaystats(WINDOW* win);
		bool boundarydetection(long x,long y,long w,long h);
		bool collisiondetection(long px,long py);

};

game::game()
{

}

game::~game()
{

}

void game::displayintro(WINDOW* win)
{

}

void game::displaystats(WINDOW* win)
{
	wattron(win,A_BOLD);
	wattron(win,COLOR_PAIR(8));
	wmove(win,24,0);
	waddstr(win,"HEALTH: ");
	waddstr(win,"100%"); //temp placeholder
	waddstr(win,"               ");
	waddstr(win,"PROGRESS: ");
	waddstr(win,">>>>>>>>>>>>>>>>>>>+"); //temp placeholder
	waddstr(win,"               ");
	waddstr(win,"FPS: ");
	waddstr(win,"100"); //temp placeholder	
	wattroff(win,COLOR_PAIR(8));
	wattroff(win,A_BOLD);
}

bool game::boundarydetection(long x,long y,long w,long h)
{
	if(x >= w || x < 0) return 0;
	if(y >= h || y < 0) return 0;
	return 1;
}

bool game::collisiondetection(long px,long py)
{
	return 0;
}

