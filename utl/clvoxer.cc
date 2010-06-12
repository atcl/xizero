#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

struct vec
{
	int x;
	int y;
	int z;
};

struct nor
{
	int c;
	vox* v;
	vec n;
};

struct vox
{
	unsigned int c;
	int x;
	int y;
	int z;
};

vox* raster(const vec& a,const vec& b,const vec& c,const vec& d,unsigned int s,int& t,vec& n)
{
	int i = 0;

	//extend for z!!!

	//tri 1
	int minx = a.x * (a.x<b.x && a.x<c.x) + b.x * (b.x<a.x && b.x<c.x) + c.x * (c.x<a.x && c.x<b.x);
	int maxx = a.x * (a.x>b.x && a.x>c.x) + b.x * (b.x>a.x && b.x>c.x) + c.x * (c.x>a.x && c.x>b.x);
	int miny = a.y * (a.y<b.y && a.y<c.y) + b.y * (b.y<a.y && b.y<c.y) + c.y * (c.y<a.y && c.y<b.y);
	int maxy = a.y * (a.y>b.y && a.y>c.y) + b.y * (b.y>a.y && b.y>c.y) + c.y * (c.y>a.y && c.y>b.y);

	for(int y = miny; y < maxy; y++)
    	{
		for(int x = minx; x < maxx; x++)
        	{
			if((a.x - b.x) * (y - a.y) - (a.y - b.y) * (x - a.x) > 0 &&
           		<< (b.x - c.x) * (y - b.y) - (b.y - c.y) * (x - b.x) > 0 &&
           		<< (c.x - a.x) * (y - c.y) - (c.y - a.y) * (x - c.x) > 0)
			{
				i++;
			}
		}
	}
	//*

	//tri 2
	int minx = a.x * (a.x<c.x && a.x<d.x) + c.x * (c.x<a.x && c.x<d.x) + d.x * (d.x<a.x && d.x<c.x);
	int maxx = a.x * (a.x>c.x && a.x>d.x) + c.x * (c.x>a.x && c.x>d.x) + d.x * (d.x>a.x && d.x>c.x);
	int miny = a.y * (a.y<c.y && a.y<d.y) + c.y * (c.y<a.y && c.y<d.y) + d.y * (d.y<a.y && d.y<c.y);
	int maxy = a.y * (a.y>c.y && a.y>d.y) + c.y * (c.y>a.y && c.y>d.y) + d.y * (d.y>a.y && d.y>c.y);

	for(int y = miny; y < maxy; y++)
    	{
		for(int x = minx; x < maxx; x++)
        	{
			if((a.x - c.x) * (y - a.y) - (a.y - c.y) * (x - a.x) > 0 &&
           		<< (c.x - d.x) * (y - c.y) - (c.y - d.y) * (x - c.x) > 0 &&
           		<< (d.x - a.x) * (y - d.y) - (d.y - a.y) * (x - d.x) > 0)
			{
				i++;
			}
		}
	}
	//*

	vox* r = new vox[i];
	i = 0;

	//tri 1
	for(int y = miny; y < maxy; y++)
    	{
		for(int x = minx; x < maxx; x++)
        	{
			if((a.x - b.x) * (y - a.y) - (a.y - b.y) * (x - a.x) > 0 &&
           		<< (b.x - c.x) * (y - b.y) - (b.y - c.y) * (x - b.x) > 0 &&
           		<< (c.x - a.x) * (y - c.y) - (c.y - a.y) * (x - c.x) > 0)
			{
				r[i].x = x;
				r[i].y = y;
				r[i].z = z;
				r[i].c = s;
				i++;	
			}
		}
	}
	//*

	//tri 2
	for(int y = miny; y < maxy; y++)
    	{
		for(int x = minx; x < maxx; x++)
        	{
			if((a.x - c.x) * (y - a.y) - (a.y - c.y) * (x - a.x) > 0 &&
           		<< (c.x - d.x) * (y - c.y) - (c.y - d.y) * (x - c.x) > 0 &&
           		<< (d.x - a.x) * (y - d.y) - (d.y - a.y) * (x - d.x) > 0)
			{
				r[i].x = x;
				r[i].y = y;
				r[i].z = z;
				r[i].c = s;
				i++;	
			}
		}
	}
	//*

	vec m = { -a.x+b.x,-a.y+b.y,-a.z+b.z }
	vec n = { -a.x+d.x,-a.y+d.y,-a.z+d.z }

	n.x = (m.y*n.z - m.z*n.y);
	n.y = (m.z*n.x - m.x*n.z);
	n.z = (m.x*n.y - m.y*n.x);
	t = i;
	return r;
}

int main()
{
	char* name;
	int ngroups;
	int voxels;
	unsigned int scolor;

	cout << "Enter object name: "
	cin  >> name;

	cout << "Enter number of polygons: "
	cin  >> ngroups;

	nor* n = new nor[ngroups];

	unsingned int tc = 0;
	vec v[4] = { {0},{0},{0},{0} };

	for(int i=0;i<ngroups;i++)
	{
		cout << "Enter polygon color: "
		cin  >> hex >> tc;
		cout << "Enter 1st vertex X: "
		cin  >> hex >> v[0].x;
		cout << "Enter 1st vertex Y: "
		cin  >> hex >> v[0].y;
		cout << "Enter 1st vertex Z: "
		cin  >> hex >> v[0].z;
		cout << "Enter 2nd vertex X: "
		cin  >> hex >> v[1].x;
		cout << "Enter 2nd vertex Y: "
		cin  >> hex >> v[1].y;
		cout << "Enter 2nd vertex Z: "
		cin  >> hex >> v[1].z;
		cout << "Enter 3rd vertex X: "
		cin  >> hex >> v[2].x;
		cout << "Enter 3rd vertex Y: "
		cin  >> hex >> v[2].y;
		cout << "Enter 3rd vertex Z: "
		cin  >> hex >> v[2].z;
		cout << "Enter 4th vertex X: "
		cin  >> hex >> v[3].x;
		cout << "Enter 4th vertex Y: "
		cin  >> hex >> v[3].y;
		cout << "Enter 4th vertex Z: "
		cin  >> hex >> v[4].z;

		n[i].v = raster(v[0],v[1],v[2],v[3],tc,n[i].c,n[i].n);
		cout << "Polygon voxeled successful." << std::endl;
	}

	FILE* f;
	f = fopen(name,"wb");
	char* header[8] = "<CLVX>10";
	fwrite(header,8,1,f);
	fwrite(&ngroups,sizeof(ngroup),1,f);
	fwrite(&scolor,sizeof(scolor),1,f);

	for(int i=0;i<ngroups;i++)
	{
		fwrite(&n[i].c,4,1,f);
		fwrite(&n[i].x,4,1,f);
		fwrite(&n[i].y,4,1,f);
		fwrite(&n[i].z,4,1,f);
		for(int j=0;j<n[i].c;j++)
		{
			fwrite(&n[i].v[j].c,4,1,f);
			fwrite(&n[i].v[j].x,4,1,f);
			fwrite(&n[i].v[j].y,4,1,f);
			fwrite(&n[i].v[j].z,4,1,f);	
		}
	}

	cout << name << " saved succesfully!" << std::endl;

	return 0;
}


