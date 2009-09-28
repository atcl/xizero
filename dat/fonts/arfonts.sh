#!/bin/sh 

rm ../fonts.a
ar -q 'fonts.a' 'CL7segment.fnt' 'CLlinetype.fnt' 'CLmonotype.fnt' 'CLteletype.fnt'
mv fonts.a ../ 
