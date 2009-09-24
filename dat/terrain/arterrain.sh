#!/bin/sh 

rm terrain.a

for file in `ls *.y3d` ; do
	ar -q terrain.a $file 
	mv terrain.a ../
done
