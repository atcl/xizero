#!/bin/sh 

rm -f ../levels.a

for file in `ls *.lvl` ; do
	ar -q levels.a $file 
	mv levels.a ../
done  
