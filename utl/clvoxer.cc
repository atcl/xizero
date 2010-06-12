#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

struct nor
{
	int c;
	vox* v;
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

vox* poly2vox(vec a,vec b,vec c,vec d)
{

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

	//...

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
}
