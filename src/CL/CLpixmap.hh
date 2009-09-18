//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLPIXMAP
#define HH_CLPIXMAP
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLsystem.hh"

uxlong CLhatoi(uxchar c)
{
	switch(c)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'A': return 10;
		case 'B': return 11;
		case 'C': return 12;
		case 'D': return 13;
		case 'E': return 14;
		case 'F': return 15;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		default:  return 0;
	}
}

uxchar* xpm2ico(xchar* xpm[])
{
	#define ICO_HEADER_LENGTH 6
	#define ICO_DIRECTORY_LENGTH 16
	
	uxlong xpm_ptr = 0;
	
	//read width,height,colors and chars per pixel
	uxlong width = CLsystem::instance()->ato(&xpm[0][xpm_ptr]);
	xpm_ptr++; while( (xpm[0][xpm_ptr]) !=' ') xpm_ptr++;
	uxlong height = CLsystem::instance()->ato(&xpm[0][xpm_ptr]);
	xpm_ptr++; while( (xpm[0][xpm_ptr]) !=' ') xpm_ptr++;
	uxlong colors = CLsystem::instance()->ato(&xpm[0][xpm_ptr]);
	xpm_ptr++; while( (xpm[0][xpm_ptr]) !=' ') xpm_ptr++;
	uxlong charpp = CLsystem::instance()->ato(&xpm[0][xpm_ptr]); //this will work only for 1 char per pixel!!!
	if(charpp!=1) return 0;
	uxlong bytesize = (width*height)<<2;
	//*
	
	//allocate ico buffer
	uxchar* ico = new uxchar[ICO_HEADER_LENGTH+ICO_DIRECTORY_LENGTH+bytesize];
	//*
	
	//write ico header
	uxshort* ico_header = reinterpret_cast<uxshort*>(&ico[0]);
	ico_header[0] = 0; //has to 0
	ico_header[1] = 1; //ico type
	ico_header[2] = 1; //1 icon in file
	//*
	
	//write ico directory
	uxchar* ico_directory1 = reinterpret_cast<uxchar*>(&ico[6]);
	ico_directory1[0] = width;    //icon width
	ico_directory1[1] = height;   //icon height
	ico_directory1[2] = 0;        //more than 256 colors
	ico_directory1[3] = 0;        //has to be 0
	uxshort* ico_directory2 = reinterpret_cast<uxshort*>(&ico[10]);
	ico_directory2[0] = 0;        //should be zero
	ico_directory2[1] = 32;       //bits per pixel
	uxlong* ico_directory3 = reinterpret_cast<uxlong*>(&ico[14]);
	ico_directory3[0] = bytesize; //size of pure icon in bytes
	ico_directory3[1] = 0;        //offset of icon data after header
	//*
	
	//read color table and set image buffers colors
	colors++;
	uxchar ctable[256];
	uxchar cindex = 0;
	uxlong currcol = 0; 
	for(uxlong i=1; i<colors; i++)
	{
		xpm_ptr = 0;
		cindex = xpm[i][0];
		xpm_ptr++; while( (xpm[i][xpm_ptr]) !=' ') xpm_ptr++;
		xpm_ptr++;
		if(xpm[i][xpm_ptr]!='c' && xpm[i][xpm_ptr]!='C') return 0;
		xpm_ptr++; while( (xpm[i][xpm_ptr]) !=' ') xpm_ptr++;
		xpm_ptr++;
		if(xpm[i][xpm_ptr]!='#') return 0;
		xpm_ptr++;
		currcol  = CLhatoi(xpm[i][xpm_ptr])<<20; xpm_ptr++; 
		currcol += CLhatoi(xpm[i][xpm_ptr])<<16; xpm_ptr++;
		currcol += CLhatoi(xpm[i][xpm_ptr])<<12; xpm_ptr++;
		currcol += CLhatoi(xpm[i][xpm_ptr])<<8;  xpm_ptr++;
		currcol += CLhatoi(xpm[i][xpm_ptr])<<4;  xpm_ptr++;
		currcol += CLhatoi(xpm[i][xpm_ptr]);
		ctable[cindex] = currcol;
	}
	//*

	//fill ico data
	uxlong ico_ptr = 0; 
	height += colors;
	uxlong* ico_data = reinterpret_cast<uxlong*>(&ico[22]);
	for(uxlong j=colors; j<height; j++)
	{
		for(uxlong k=0; k<width; k++)
		{
			ico_data[ico_ptr] = ctable[xpm[j][k]];
			ico_ptr++;
		}
	}
	//*
	
	return ico;
}

#endif
