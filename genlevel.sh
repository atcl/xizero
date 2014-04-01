#!/bin/sh

rm -f $1
touch $1

for (( c=1; c<=$2; c++ ))
do
	for (( d=1; d<=$3; d++ ))
	do
		echo -n '.' >> $1
	done
	echo >> $1
done
