#!/bin/sh 

echo 'packing fonts'
ar -q 'fonts.a' 'fonts/CL7segment.fnt' 'fonts/CLlinetype.fnt' 'fonts/CLmonotype.fnt' 'fonts/CLteletype.fnt'

echo 'packing intros'
ar -q 'intro.a' 'intro/atcl.ani' 'intro/atcl.y3d' 'intro/xi.ani' 'intro/xi.y3d' 'intro/zero.ani' 'intro/zero.y3d'

echo 'packing screens'
ar -q 'screens.a' 'screens/introtext.tga' 'screens/loadscreen.tga' 'screens/gamewon.tga' 'screens/gameover.tga'

echo 'packing player'
ar -q 'player.a' 'player/player0.y3d' 'player/player1.y3d' 'player/player.ini'

echo 'packing levels (lvl)'
for file in `ls levels/*.lvl` ; do
	ar -q 'levels.a' $file 
done 

echo 'packing terrain'
for file in `ls terrain/*.y3d` ; do
	ar -q 'terrain.a' $file 
done

echo 'packing bosses'
for file in `ls boss/*.a` ; do
	ar -q 'bosses.a' $file 
done 

echo 'packing enemies'
for file in `ls enemy/*.a` ; do
	ar -q 'enemies.a' $file 
done 

echo 'pack all of above into one super ar'
rm -f 'xizero.dat'
ar -q 'xizero.a' 'bosses.a' 'enemies.a' 'player.a' 'screens.a' 'terrain.a' 'levels.a' 'intro.a' 'fonts.a'

echo 'packing level maps into superar'
for file in `ls level/*.a` ; do
	ar -q 'xizero.a' $file
done 
mv 'xizero.a' 'xizero.dat'

echo 'cleaning up'
rm -f 'fonts.a'
rm -f 'intro.a'
rm -f 'screens.a'
rm -f 'levels.a'
rm -f 'terrain.a'
rm -f 'enemies.a'
rm -f 'player.a'
rm -f 'bosses.a'

