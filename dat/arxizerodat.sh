#!/bin/sh 

rm -f 'xizero.dat'
ar -q 'xizero.a' 'bosses.a' 'enemies.a' 'player.a' 'screens.a' 'terrain.a' 'levels.a' 'level0.a'
mv 'xizero.a' 'xizero.dat'

