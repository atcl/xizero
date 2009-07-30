#!/bin/sh 

if [ "$1" != "" ]; then
	ar -q $1'.a' $1'.mapt' $1'.maph' $1'.mape'
else
	echo -n "Enter level name: "
	read level
	ar -q $level'.a' $level'.mapt' $level'.maph' $level'.mape'
fi
