#!/bin/sh 

rm -f ../bosses.a

for file in `ls *.a` ; do
	ar -q bosses.a $file 
	mv bosses.a ../
done 
