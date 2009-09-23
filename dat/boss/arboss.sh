#!/bin/sh 

if [ "$1" != "" ]; then
	rm -f $1'.a'
	ar -q $1'.a' $1'.y3d' $1'.ini' $1'.csv'
else
	echo -n "Enter enemy name: "
	read boss
	rm -f $boss'.a'
	ar -q $boss'.a' $boss'.y3d' $boss'.ini' $boss'.csv'
fi

