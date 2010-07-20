#include <cstdio>
#include <iostream>
#include <iomanip>
#include <vector>

struct vec
{
	int x;
	int y;
	int z;
};

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

bool operator==(vec a,vec b) //check for colinearity!
{
	if( (a.x*b.x + a.y*b.y + a.z*b.z) == 0) { return 1; }
	return 0;
}

int min(int a,int b,int c)
{
	if(a<=b && a<=c) { return a; }
	if(b<=a && b<=c) { return b; }
	if(c<=a && c<=b) { return c; }
}

int max(int a,int b,int c)
{
	if(a>=b && a>=c) { return a; }
	if(b>=a && b>=c) { return b; }
	if(c>=a && c>=b) { return c; }
}

vec normal(vec a,vec b)
{
	vec r;

	return r;
}

std::vector<vox> raster(const vec& a,const vec& b,const vec& c,unsigned int s)
{
	//bounding cube
	int minx = min(a.x,b.x,c.x);
	int maxx = max(a.x,b.x,c.x); 
	int miny = min(a.y,b.y,c.y); 
	int maxy = max(a.y,b.y,c.y); 
	int minz = min(a.z,b.z,c.z); 
	int maxz = max(a.z,b.z,c.z);
	//*

	//return vector
	std::vector<vox> r;
	vox temp;
	//

	//raster
	for(int z=minz;z<maxz;z++)
	{
		for(int y=miny;y<maxy;y++)
		{
			for(int x=minx;x<maxz;x++)
			{
				//3d half space function
				if((a.x - b.x) * (y - a.y) - (a.y - b.y) * (x - a.x) > 0 &&
            			   (b.x - c.x) * (y - b.y) - (b.y - c.y) * (x - b.x) > 0 &&
            			   (c.x - a.x) * (y - c.y) - (c.y - a.y) * (x - c.x) > 0 &&

				   (a.y - b.y) * (z - a.z) - (a.z - b.z) * (y - a.y) > 0 &&
            			   (b.y - c.y) * (z - b.z) - (b.z - c.z) * (y - b.y) > 0 &&
            			   (c.y - a.y) * (z - c.z) - (c.z - a.z) * (y - c.y) > 0)
				//*

				temp.x = x;
				temp.y = y;
				temp.z = z;
				temp.c = s;
				r.push_back(temp);
			}
		}
	}
	//*

	return r;
}

int main(int argc,char** argv)
{
	//evaluate command line arguments
	char* input;
	if(argc<2) { std::cout << "no input file" << std::endl; return 0; }
	input = argv[1];
	//*

	//vector of normal structures nor
	std::vector<nor> ngroup;
	std::vector<nor>::iterator ngroupiter;
	//*

	char magic[4];
	char name[4];
	int polys;
	unsigned int scolor;
	unsigned int vcolor; 
	
	FILE* f = fopen(input,"r");
	fscanf(f,"%s,%s,%i,%i\n",&magic,&name,&polys,&scolor);

	//read polys polygons
	vec v[4] = { {0},{0},{0},{0} };
	vec n;
	std::vector<vox> voxels;
	nor temp;
	nor curr;
	bool nex = 0;
	for(int i=0;i<polys;i++)
	{
		//polygon input
		fscanf(f,"%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i\n",&vcolor,&v[0].x,&v[0].y,&v[0].z,&v[1].x,&v[1].y,&v[1].z,&v[2].x,&v[2].y,&v[2].z,&v[3].x,&v[3].y,&v[3].z); 
		//*

		//vertex processing
		temp.n = normal(v[0],v[1]);
		temp.v = raster(v[0],v[1],v[2],vcolor);
		//add 2nd tri
		//*

		//check if normal group exists
		for(ngroupiter=ngroup.begin();ngroupiter!=ngroup.end();ngroupiter++) { if(ngroupiter->n==n) { nex = 1; curr = *ngroupiter; } }
		
		//push back all voxels to new or exisitng normal group
		if(nex==1) { curr.v.insert(curr.v.end(), temp.v.begin(), temp.v.end()); }
		else { ngroup.push_back(temp); }
		
		//reset for next polygon
		nex = 0;
		voxels.clear();
	}
	//*

	fclose(f);


	//write to file
	f = fopen(name,"w");
	fprintf(f,"CLVX,%s,%i,%i\n",name,ngroup.size(),scolor);

	std::vector<vox>::iterator voxeliter;
	for(ngroupiter=ngroup.begin();ngroupiter!=ngroup.end();ngroupiter++)
	{
		fprintf(f,"%i,%i,%i,%i\n",ngroupiter->v.size(),ngroupiter->n.x,ngroupiter->n.y,ngroupiter->n.z);
		for(voxeliter=ngroupiter->v.begin();voxeliter!=ngroupiter->v.end();voxeliter++)
		{
			fprintf(f,"%i,%i,%i,%i\n",voxeliter->c,voxeliter->x,voxeliter->y,voxeliter->z);
		}
	}
	fclose(f);
	std::cout << name << "succesful!" << std::endl;
	//*

	//exit
	return 0;
	//*
}
