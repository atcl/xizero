#!/bin/sh 

if [ "$1" != "" ]; then
	rm -f $1'.a'
	ar -q $1'.a' $1'.mapt' $1'.maph' $1'.mape'
	mv $1'.a'
else
	echo -n "Enter level name: "
	read level
	rm -f $level'.a'
	ar -q $level'.a' $level'.mapt' $level'.maph' $level'.mape'
	mv $level'.a'
fi
