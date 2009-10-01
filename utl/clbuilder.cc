#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

union doubleword
{
	int32_t dd;
	int16_t dw[2];
	int8_t  db[4];
};

struct y3dline
{
	doubleword a;
	doubleword b;
	doubleword c;
	doubleword d;
};

using namespace std;

void lineout(y3dline y, int32_t t)
{
	switch(t)
	{
		case 'OBJT':
			cout << y.a.db[0] << y.a.db[1] << y.a.db[2] << y.a.db[3] << ",";
			cout << y.b.db[0] << y.b.db[1] << y.b.db[2] << y.b.db[3] << ",";
			cout << y.c.dd << "," << y.d.dd << endl;
		break;

		case 'SOBJ':
			cout << y.a.db[0] << y.a.db[1] << y.a.db[2] << y.a.db[3] << ",";
			cout << y.b.db[0] << y.b.db[1] << y.b.db[2] << y.b.db[3] << ",";
			cout << y.c.dd << "," << y.d.dd << endl;
		break;

		case 'POLY':
			cout << y.a.db[0] << y.a.db[1] << y.a.db[2] << y.a.db[3] << ",";
			cout << y.b.db[0] << y.b.db[1] << y.b.db[2] << y.b.db[3] << ",";
			cout << y.c.dd << "," << y.d.dd << endl;
		break;

		case 'CONN':
			cout << y.a.db[0] << y.a.db[1] << y.a.db[2] << y.a.db[3] << ",";
			cout << y.b.dd << "," << y.c.dd << "," << y.d.dd << endl;
		break;

		case 'VECT':
			cout << y.a.db[0] << y.a.db[1] << y.a.db[2] << y.a.db[3] << ",";
			cout << y.b.dd << "," << y.c.dd << "," << y.d.dd << endl;
		break;

		case 'DOCK':
			cout << y.a.db[0] << y.a.db[1] << y.a.dw[2] << ",";
			cout << y.b.dd << "," << y.c.dd << "," << y.d.dd << endl;
		break;

		case 'ENDO':
			cout << y.a.db[0] << y.a.db[1] << y.a.db[2] << y.a.db[3] << ",";
			cout << y.b.dd << "," << y.c.dd << "," << y.d.dd << endl;
		break;
	}
}

long flipendianess(long x)
{
//huch brauch ich ja gar nicht, port to CLutils...
	doubleword l;
	l.dd = x;
	char a[4];

	a[0] = l.db[0];
	a[1] = l.db[1];
	a[2] = l.db[2];
	a[3] = l.db[3];

	l.db[0] = a[3];
	l.db[1] = a[2];
	l.db[2] = a[1];
	l.db[3] = a[0];

	return l.dd;
}

doubleword getinput()
{
	doubleword temp;
	cin >> temp.dd;

	return temp;
}

//this is cool and should be added to cstdio
bool fexists(char f[])
{
	FILE *fex = fopen(f, "r");
	if(fex) return true;
	else return false;
}
//*



int main()
{
	vector<y3dline> proto;
	int32_t globalpolycount = 0;
	int32_t globaldockcount = 0;
	y3dline temp;
	y3dline test[4];
	int32_t sobjcount;
	int32_t polycount = 0;
	int32_t dockcount;
	short docktype;
	char  tempc1;
	char  tempc2;

	//header:
	temp.a.dd = '<CLY';
	temp.b.dd = '3DB>';
	temp.c.dd = 0;
	temp.d.dd = 0;
	
	proto.push_back(temp);

	//OBJT
	temp.a.dd = 'OBJT';
	cout << "Enter name of object [4 characters]: ";
	cin  >> temp.b.db;
	cout << "Enter number of subobjects: ";
	cin >> temp.c.dd;
	sobjcount = temp.c.dd;
	temp.d.dd = 0; //after input complete polycount	
	proto.push_back(temp);
	lineout(temp,'OBJT');

	cout << endl;

	//SHADOWCOLOR
	uxlong scolor = 0;
	cout << "Enter color of Shadows [in hex, 4 byte ie: 00ff0000 for red]: ";
	cin >> hex >> scolor;

	//SOBJ's
	for(int i=0; i<sobjcount; i++)
	{
		temp.a.dd = 'SOBJ';
		cout << "Enter name of Subobject [4 characters]: ";
		cin >> temp.b.db;
		cout << "Enter number of polygons: ";
		cin >> temp.c.dd;
		polycount = temp.c.dd;
		cout << "Enter number of dockings: ";
		cin >> temp.d.dd;
		dockcount = temp.d.dd;
		proto.push_back(temp);
		lineout(temp,'SOBJ');

		//COOR
		temp.a.dd = 'CONN';
		cout << "Enter reference X value: ";
		cin >> temp.b.dd;
		cout << "Enter reference Y value: ";
		cin >> temp.c.dd;
		cout << "Enter reference Z value: ";
		cin >> temp.d.dd;
		proto.push_back(temp);
		lineout(temp,'CONN');

		//POLY's
		for(int j=0; j<polycount; j++)
		{
			globalpolycount++;

			temp.a.dd = 'POLY';
			cout << "Enter name of Polygon [4 characters]: ";
			cin >> temp.b.db;
			cout << "Enter color of Polygon [in hex, 4 byte ie: 00ff0000 for red]: ";
			cin >> hex >> temp.c.dd;
			tempc1 = temp.c.db[0];
			//tempc2 = temp.c.db[1];
			temp.c.db[0] = temp.c.db[2];
			//temp.c.db[1] = temp.c.db[3];
			temp.c.db[2] = tempc1;
			temp.d.dd = 0;
			proto.push_back(temp);
			lineout(temp,'POLY');

			cout << endl;

			//VECT's
			for(int h=0; h<4; h++)
			{
				test[h].a.dd = 'VECT';
				cout << "Enter X" << (h+1) << " value: ";
				cin >> dec >> test[h].b.dd;
				cout << "Enter Y" << (h+1) << " value: ";
				cin >> dec >> test[h].c.dd;
				cout << "Enter Z" << (h+1) << " value: ";
				cin >> dec >> test[h].d.dd;
				lineout(temp,'VECT');
			}

			//visibility check

			//convexity check

			proto.push_back(test[0]);
			proto.push_back(test[1]);
			proto.push_back(test[2]);
			proto.push_back(test[3]);

			cout << endl;
		}

		//DOCK's
		for(int k=0; k<dockcount; k++)
		{
			globaldockcount++;

			cout << "Enter Docking type [0-65536]: ";
			cin.width(5);
			cin >> docktype;
			temp.a.dw[0] = 'DK';
			temp.a.dw[1] = docktype;
			cout << "Enter docking X value: ";
			cin >> temp.b.dd;
			cout << "Enter docking Y value: ";
			cin >> temp.c.dd;
			cout << "Enter docking Z value: ";
			cin >> temp.d.dd;
			proto.push_back(temp);
			lineout(temp,'DOCK');
		}

		cout << endl;
	}

	proto[0].c.dd = globalpolycount;
	proto[0].d.dd = scolor;
	proto[1].d.dd = globaldockcount;
	
	//ENDO
	temp.a.dd = 'ENDO';
	temp.b.dd = 0;
	temp.c.dd = 0;
	temp.d.dd = 0x1A000000;
	proto.push_back(temp);
	lineout(temp,'ENDO');

	//output to file:
	bool dec_loop = false;

	while(!dec_loop)
	{
		char filename[12];
		cout << "Enter filename to save [8.3]: ";
		cin >> filename;
		cout << "saving to: " << filename << endl;
	
		if( filename[9]!='.' && (filename[10]!='y' || filename[10]!='Y') && filename[11]!='3' && (filename!='d' || filename[10]!='D') )
		{
			filename[9]  = '.';
			filename[10] = 'y';
			filename[11] = '3';
			filename[12] = 'd';
		}
	
		if(!fexists(filename))
		{
			FILE *fp;
			fp=fopen(filename, "wb");
			for(int z=0; z<proto.size() ;z++)
			{
				fwrite(&proto[z].a.dd, sizeof(proto[z].a.dd), 1, fp);
				fwrite(&proto[z].b.dd, sizeof(proto[z].b.dd), 1, fp);
				fwrite(&proto[z].c.dd, sizeof(proto[z].c.dd), 1, fp);
				fwrite(&proto[z].d.dd, sizeof(proto[z].b.dd), 1, fp);
			}
			fclose(fp);
			dec_loop==true;
		}
		else
		{
			char yesno;
			cout << "overwrite?" << endl;
			cin >> yesno;
			if(yesno=='y' || yesno=='Y')
			{
				FILE *fp;
				fp=fopen(filename, "wb");
				for(int z=0; z<proto.size() ;z++)
				{
					fwrite(&proto[z].a.dd, sizeof(proto[z].a.dd), 1, fp);
					fwrite(&proto[z].b.dd, sizeof(proto[z].b.dd), 1, fp);
					fwrite(&proto[z].c.dd, sizeof(proto[z].c.dd), 1, fp);
					fwrite(&proto[z].d.dd, sizeof(proto[z].b.dd), 1, fp);
				}
				fclose(fp);
				dec_loop==true;
			}
		}
	}

	cout << "done! exiting..." << endl;
	
	return 0;
}

