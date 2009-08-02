#!/bin/sh 

if [ "$1" != "" ]; then
	rm -f $1'.a'
	ar -q $1'.a' $1'.y3d' $1'.ini' $1'.csv'
else
	echo -n "Enter enemy name: "
	read enemy
	rm -f $enemy'.a'
	ar -q $enemy'.a' $enemy'.y3d' $enemy'.ini' $enemy'.csv'
fi

