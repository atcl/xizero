#!/bin/sh 

echo 'packing fonts'
ar -q 'fonts.a' 'dat/fonts/CL7segment.fnt' 'dat/fonts/CLlinetype.fnt' 'dat/fonts/CLmonotype.fnt' 'dat/fonts/CLteletype.fnt' >> /dev/null 2>&1

echo 'packing intros'
ar -q 'intro.a' 'dat/intro/atcl.ani' 'dat/intro/atcl.y3d' 'dat/intro/xi.ani' 'dat/intro/xi.y3d' 'dat/intro/zero.ani' 'dat/intro/zero.y3d' >> /dev/null 2>&1

echo 'packing screens'
ar -q 'screens.a' 'dat/screens/intro000.ras' 'dat/screens/gamewon.ras' 'dat/screens/gameover.ras' >> /dev/null 2>&1

echo 'packing player'
ar -q 'player.a' 'dat/player/player0.y3d' 'dat/player/player1.y3d' 'dat/player/player.ini' >> /dev/null 2>&1

echo 'packing level maps'
for file in `ls dat/maps/*.mapt` ; do
	ar -q ${file%'.mapt'}'.a' ${file%'.mapt'}'.mapt' ${file%'.mapt'}'.maph' ${file%'.mapt'}'.mape' >> /dev/null 2>&1
done

echo 'packing levels'
for file in `ls dat/levels/*.lvl` ; do
	ar -q 'levels.a' $file >> /dev/null 2>&1
done 

echo 'packing terrain'
for file in `ls dat/terrain/*.y3d` ; do
	ar -q 'terrain.a' $file >> /dev/null 2>&1
done

echo 'prepacking bosses'
for file in `ls dat/boss/*.y3d` ; do
	ar -q ${file%'.y3d'}'.a' ${file%'.y3d'}'.y3d' ${file%'.y3d'}'.ini' ${file%'.y3d'}'.csv' >> /dev/null 2>&1
done

echo 'packing bosses'
for file in `ls dat/boss/*.a` ; do
	ar -q 'bosses.a' $file >> /dev/null 2>&1
	rm $file
done 

echo 'prepacking enemies'
for file in `ls dat/enemy/*.y3d` ; do
	ar -q ${file%'.y3d'}'.a' ${file%'.y3d'}'.y3d' ${file%'.y3d'}'.ini' ${file%'.y3d'}'.csv' >> /dev/null 2>&1
done

echo 'packing enemies'
for file in `ls dat/enemy/*.a` ; do
	ar -q 'enemies.a' $file >> /dev/null 2>&1
	rm $file
done 

echo 'pack all of above into one super-ar'
rm -f 'xizero.dat'
ar -q 'xizero.a' 'bosses.a' 'enemies.a' 'player.a' 'screens.a' 'terrain.a' 'levels.a' 'intro.a' 'fonts.a' >> /dev/null 2>&1

for file in `ls dat/maps/*.a` ; do
	ar -q 'xizero.a' $file >> /dev/null 2>&1
	rm $file
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

