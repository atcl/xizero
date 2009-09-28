//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
#ifndef HH_CLAR
#define HH_CLAR
#pragma message "Compiling " __FILE__ " ! TODO: "

#include "CLtypes.hh"
#include "CLstruct.hh"

/* class name:	CLar
 * 
 * description:	This class handles ar archives.
 * 
 * author:	atcl
 * 
 * notes:	test
 * 
 * version: 0.1
 */
 
class CLar : public virtual CLcl
{
	private:
		CLfile** members;
		uxlong   filecount;
	
	public:
		CLar(const xchar* sf);
		CLar(CLfile* sf);
		~CLar();
		void loadar(CLfile* sf);
		CLfile* findbyname(const xchar* e) const;
		CLfile* findbyextension(const xchar* e) const;
		CLfile* getmember(uxlong index) const;
		uxlong  getfilecount() const;
};

CLar::CLar(CLfile* sf) { loadar(sf); }

CLar::CLar(const xchar* sf) { loadar(clsystem->getfile(sf)); }

void CLar::loadar(CLfile* sf)
{
	xchar* bf = sf->text;
	xlong cfs = sf->size;

	//ar can contain max 127 files!!!
	CLfile* tindex[128];
	xlong tsize = cfs - 8;
	//*

	//check for "magic-string"
	if( clsystem->cmpcstr(bf,"!<arch>",6) == 0 )
	{
		//init variables
		xlong bc = 8;
		xlong fc = 0;
		xchar fn[16];
		xlong fs = 0;
		xlong ts[10];
		//*

		//for each member do
		do
		{
			//read member header
			clutils->copychararray(&fn[0],&bf[bc],16);	//member filename
			bc += 48;					//no necessary information here, so skip
			//*
			
			//decode filesize of current ar member
			fs = clsystem->ato(&bf[bc]);
			bc+=12; //goto end of header
			//*

			//create xlong array for current ar member
			xlong fs2 = fs>>2;
			if(fs%4!=0) fs2++;
			fs2++;
			xlong* tb = new xlong[fs2];
			xlong* bf2 = static_cast<xlong*>(static_cast<void*>(&bf[bc]));
			//*

			//fill array
			for(uxlong i=0; i<fs2; i++)
			{
				tb[i] = bf2[i];
			}
			bc += fs;
			//*

			//make new armember
			tindex[fc] = new CLfile;
			tindex[fc]->size = fs;
			tindex[fc]->lsize = fs2;
			tindex[fc]->name = new xchar[16]; clutils->copychararray(tindex[fc]->name,&fn[0],16);
			tindex[fc]->data = tb;
			tindex[fc]->text = static_cast<xchar*>(static_cast<void*>(&tb[0]));
			//*

			//adjust global ar variables
			if(fs%2!=0) { bc++; tsize--; }
			tsize -= (fs+60); //subtract reading size from global size
			fc++; //increment filecount
			//*

		} while( tsize > 0 );
		//*

		//create arfile
		filecount = fc;
		members = new CLfile*[fc];
		//*

		//copy armembers to arfile
		for(uxlong j=0; j<fc; j++) { members[j] = tindex[j]; }
		//*
	}
	else { clsystem->exit(1,0,__func__,"no \"!<arch>\" identifier found"); }
}

CLar::~CLar() { for(int i=0; i<filecount; i++) delete members[i]; }

CLfile* CLar::findbyextension(const xchar* e) const
{
		xlong r = -1;

		for(uxlong h=0; h<filecount; h++)
		{
			if(clutils->checkextension(members[h]->name,16,e)==true)
			{
				r=h;
				break;
			}
		}
		
		if(r==-1) return 0;
		else return members[r];
}

CLfile* CLar::findbyname(const xchar* e) const
{
		xlong r = -1;
		xlong ti = 0;

		for(uxlong h=0; h<filecount; h++)
		{
			while(e[ti]!=0)
			{
				if(e[ti]==members[h]->name[ti]) { r = h; ti++; }
				else  { r = -1; break; }
			}
		}
		
		if(r==-1) return 0;
		else return members[r];
}

CLfile* CLar::getmember(uxlong index) const
{
	if(index<filecount) return members[index];
	else return 0;
}

uxlong CLar::getfilecount() const { return filecount; }

#endif
