///license
//atCROSSLEVEL studios 2009
//licensed under zlib/libpng license 
///*

///guard
#ifndef HH_CLAR
#define HH_CLAR
///*

///includes
#include "CLtypes.hh"
#include "CLstruct.hh"
#include "CLbase.hh"
///*

///header
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
///*

///definitions
class CLar : public CLbase<CLar,0>
{
	private:
		static CLsystem* clsystem;
		static CLstring* clstring; 
	protected:
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
		uxlong  getfilecount() const { return filecount; };
};

CLsystem* CLar::clsystem = CLsystem::instance();
CLstring* CLar::clstring = CLstring::instance();
///*

///implementation
CLar::CLar(CLfile* sf) { loadar(sf); } //! noncritical

CLar::CLar(const xchar* sf) { loadar(clsystem->getfile(sf)); } //! noncritical

void CLar::loadar(CLfile* sf) //! noncritical
{
	xchar* bf = sf->text;
	xlong cfs = sf->size;

	//ar can contain max 127 files!!!
	CLfile* tindex[128];
	xlong tsize = cfs - 8;
	//*

	//check for "magic-string"
	if( clstring->compare(bf,u8"!<arch>",6) != 0 )
	{
		//init variables
		xlong bc = 8;
		xlong fc = 0;
		xchar* fn;
		xlong fs = 0;
		xlong ts[10];
		//*

		//for each member do
		do
		{
			//read member header
			fn = clstring->copy(&bf[bc],16);	//member filename
			bc += 48;					//no necessary information here, so skip
			//*
			
			//decode filesize of current ar member
			fs = clstring->tolong(&bf[bc]);
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
			for(xlong i=0; i<fs2; i++) { tb[i] = bf2[i]; }
			bc += fs;
			//*

			//make new armember
			tindex[fc] = new CLfile;
			tindex[fc]->size = fs;
			tindex[fc]->lsize = fs2;
			tindex[fc]->name = clstring->copy(&fn[0],16);
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
		for(xlong j=0; j<fc; j++) { members[j] = tindex[j]; }
		//*
	}
	else { tty(__func__); say(u8"no \"!<arch>\" identifier found"); }
}

CLar::~CLar() { delete[] members; delete members; } //! noncritical

CLfile* CLar::findbyextension(const xchar* e) const //! noncritical
{
		xlong r = -1;
		xlong l = 0;
		xlong m = clstring->length(e);
		
		for(uxlong h=0; h<filecount; h++)
		{
			while(members[h]->name[l]!='/') { l++; }
			l-=m;
			if(clstring->compare(&(members[h]->name[l]),e,m)!=0) { r = h; }
		}
		
		if(r==-1) return 0;
		else return members[r];
}

CLfile* CLar::findbyname(const xchar* e) const //! noncritical
{
		xlong r = -1;

		for(uxlong h=0; h<filecount; h++) { if(clstring->compare(members[h]->name,e)!=0) { r = h; } }
		
		if(r==-1) return 0;
		else return members[r];
}

CLfile* CLar::getmember(uxlong index) const //! noncrtical
{
	if(index>=filecount) return 0;
	return members[index];
}
///*

#endif
