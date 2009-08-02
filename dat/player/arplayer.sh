#!/bin/sh 

if [ "$1" != "" ]; then
	rm -f $1'.a'
	ar -q $1'.a' $1'0.y3d' $1'1.y3d' $1'.ini'
else
	echo -n "Enter player name: "
	read player
	rm -f $player'.a'
	ar -q $player'.a' $player'0.y3d' $player'1.y3d' $player'.ini'
fi

