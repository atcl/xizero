#include <iostream>
#include <iomanip>
#include <vector>

struct vec
{
	int x;
	int y;
	int z;
};

bool operator==(vec a,vec b) //check for colinearity!
{
	int p = (a.x*b.x + a.y*b.y + a.z*b.z) * (a.x*b.x + a.y*b.y + a.z*b.z);
	int q = (a.x*a.x + a.y*a.y + a.z*a.z) * (b.x*b.x + b.y*b.y + b.z*b.z);
	if(p==q) { return 1; }
	return 0;
}

struct vox
{
	unsigned int c;
	int x;
	int y;
	int z;
};

struct nor
{
	std::vector<vox> v;
	vec n;
};

std::vector<vox> raster(const vec& a,const vec& b,const vec& c,const vec& d,unsigned int s,int& t,vec& n)
{
	//todo!!!
}

int main(int argc,char** argv)
{
	//evaluate command line arguments
	bool g = 0;
	if(argc>0 && argv[1]=="-g") { g = 1; }
	//*

	//vector of normal structures nor
	std::vector<nor> ngroup;
	std::vector<nor>::iterator ngroupiter;
	//*

	//enter object name
	char* name;
	cout << "Enter object name: "; cin  >> name;
	//*

	//enter polygon count
	int polys = 0;
	cout << "Enter number of polygons: "; cin  >> polys;
	//*

	//enter color for all polygon faces
	unsigned int gc = 0;
	if(g) { cout << "Enter polygon color: "; cin >> hex >> gc; }
	//*

	//enter objects shadow color
	unsigned int sc = 0;
	cout << "Enter shadow color: "; cin >> hex >> sc;
	//*

	//enter polygons
	vec v[4] = { {0},{0},{0},{0} };
	std::vector<vox> voxels;
	std::vector<nor> currn;
	nor temp;
	bool nex = 0;
	for(int i=0;i<polys;i++)
	{
		cout << "Polygon (" << i << "):" << std::endl;

		if(!g) { cout << "Enter polygon color: "; cin  >> hex >> gc; }
		cout << "Enter 1st vertex X: "; cin  >> v[0].x;
		cout << "Enter 1st vertex Y: "; cin  >> v[0].y;
		cout << "Enter 1st vertex Z: "; cin  >> v[0].z;
		cout << "Enter 2nd vertex X: "; cin  >> v[1].x;
		cout << "Enter 2nd vertex Y: "; cin  >> v[1].y;
		cout << "Enter 2nd vertex Z: "; cin  >> v[1].z;
		cout << "Enter 3rd vertex X: "; cin  >> v[2].x;
		cout << "Enter 3rd vertex Y: "; cin  >> v[2].y;
		cout << "Enter 3rd vertex Z: "; cin  >> v[2].z;
		cout << "Enter 4th vertex X: "; cin  >> v[3].x;
		cout << "Enter 4th vertex Y: "; cin  >> v[3].y;
		cout << "Enter 4th vertex Z: "; cin  >> v[4].z;

		normal = normal(v[0],v[1],v[2],v[3]);
		voxels = raster(v[0],v[1],v[2],v[3],gc,n[i].c,n[i].n);
		//check if normal group exists
		for(ngroupiter=ngroup.begin();ngroupiter!=ngroup.end();ngroupiter++) { if(*ngroupiter.n==normal) { nex = 1; currn = *ngroupiter; } }
		//push back all voxels to new or exisitng normal group
		if(nex==1) { currn.v.insert(currn.v.end(), voxels.begin(), voxels.end()); }
		else { temp.n = normal; temp.v = voxels; ngroup.pushback(temp); }
		//reset for next polygon
		nex = 0;
		voxels.clear();
		cout << "Voxeling successful." << std::endl;
	}
	//*

	//write to file
	FILE* f;
	f = fopen(name,"wb");
	char* header[8] = "<CLVX>10";
	fwrite(header,8,1,f);
	fwrite(&ngroup.size(),4,1,f);
	fwrite(&sc,4,1,f);

	std::vector<vox>::iterator voxeliter;
	for(ngroupiter=ngroup.begin();ngroupiter!=ngroup.end();ngroupiter++)
	{
		fwrite(&(*ngroupiter).v.size(),4,1,f);
		fwrite(&(*ngroupiter).x,4,1,f);
		fwrite(&(*ngroupiter).y,4,1,f);
		fwrite(&(*ngroupiter).z,4,1,f);
		for(voxeliter=(*ngroupiter).v.begin();voxeliter!=(*ngroupiter).v.end();voxeliter++)
		{
			fwrite(&(*voxeliter).c,4,1,f);
			fwrite(&(*voxeliter).x,4,1,f);
			fwrite(&(*voxeliter).y,4,1,f);
			fwrite(&(*voxeliter).z,4,1,f);
		}
	}
	cout << name << " saved succesfully!" << std::endl;
	//*

	//exit
	return 0;
	//*
}
