#!/bin/sh 

for file in `ls *.y3d` ; do
	ar -q terrain.a $file 
done
