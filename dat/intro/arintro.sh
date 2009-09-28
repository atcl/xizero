#!/bin/sh 

rm ../intro.a
ar -q 'intro.a' 'atcl.ani' 'atcl.y3d' 'xi.ani' 'xi.y3d' 'zero.ani' 'zero.y3d'
mv intro.a ../ 
