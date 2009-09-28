#!/bin/sh 

rm -f ../enemies.a

for file in `ls *.a` ; do
	ar -q enemies.a $file 
	mv enemies.a ../
done 
