#!/bin/make

# Compile Flags
NX = xizero
NU = unittest

CC = g++
O2 = -O2
OP = -fsingle-precision-constant -ffast-math -fomit-frame-pointer -funroll-loops -floop-optimize -funit-at-a-time
WW = -Wno-multichar -Wno-write-strings -Wno-pragmas
WA = -Wall -Wextra -Winline -Wlogical-op -Wc++0x-compat -Wparentheses -Weffc++
OU = -o
SX = src/xizero.cc
SU = src/unittest.cc
LD = -lglut -lalut #-lXpm
DF = -DXRES=800 -DYRES=600 -DZRES=400 -DTITLE='"atCROSSLEVELs XiZero"' -DVERSION='"0.2"'
DB = -g

# Other
ST = strip -s -R .comment -R .gnu.version
CH = cppcheck -a -v -s
GR = valgrind -v --tool=memcheck --leak-check=full --track-origins=yes -v ./xizero 2>&1 | tee valgrind.txt

# File Operations
RM = rm -f
MK = mkdir -p
CP = cp

# Exclusive
XD = dat/makexizerodat.sh
GD = xizero.dat
DD = doc/man/xizero.1
BD = bin/xizero.desktop
ID = bin/xizero.xpm

BI = /usr/games
DA = /usr/share/xizero
DO = /usr/man/man1   #remove for deb?
DS = /usr/share/applications

# Options to test:
IN = --param inline-unit-growth=200 --param large-function-growth=300 -pipe
MX = -mmmx
SE = -mssse3 -mfpmath=sse
A0 = -march=core2 -mtune=generic 
A1 = -march=atom -mtune=atom #GCC >=4.5 only
AL = -falign-functions=32 -falign-labels=32 -falign-loops=32 -falign-jumps=32
#

# Compile:
default:
	$(CC) $(O2) $(OP) $(OU) $(NX) $(SX) $(LD) $(WW) $(DF) $(DB)
	$(ST) $(NX)
	$(XD)
	
unit:
	$(CC) $(O2) $(OP) $(OU) $(NU) $(SU) $(LD) $(WW) $(DF) $(DB)
	$(ST) $(NU)
	
none:
	$(CC) $(O2) $(OP) $(OX) $(SX) $(LD) $(WA) $(WW) $(DF) $(DB)
	#$(CH) $(SX)
	#$(GR)

install:
	$(MK) $(BI)
	$(CP) $(NX) $(BI)
	$(MK) $(DA)
	$(CP) $(GD) $(DA)
	$(CP) $(DD) $(DO)
	$(CP) $(BD) $(DS)
	$(CP) $(ID) $(DA)

data:
	$(XD)

clean:
	$(RM) $(NX)
	$(RM) $(NU)
	$(RM) $(GD)
