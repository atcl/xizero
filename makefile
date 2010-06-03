#!/bin/make

# Compile Flags
CC      = g++
WWFLAGS = -Wno-multichar -Wno-write-strings -Wno-pragmas
WAFLAGS = -Wall -Wextra -Winline -Wlogical-op -Wc++0x-compat -Wparentheses
O2FLAGS = -O2
OPT     = -fsingle-precision-constant -ffast-math -fomit-frame-pointer -funroll-loops -floop-optimize -funit-at-a-time
LDFLAGS = -lglut -lalut 
OUTPARA = -o 
DEBUG   = -g
LINUX   = -DLINUX
DEVELOP = -DDEV
RELEASE = -DREL

# Data Assembler
XZDAT   = dat/makexizerodat.sh

# Sources
SOURCE1 = src/xizero.cc
SOURCE2 = src/unittest.cc

# Targets
TARGET1 = xizero
TARGET2 = unittest
TARGET3 = utl/clbuilder
TARGET4 = utl/asciimissile
GAMEDAT = xizero.dat

# Other
STRIP   = strip -s -R .comment -R .gnu.version
CPPCH   = cppcheck -a -v -s
GRIND   = valgrind -v --tool=memcheck --leak-check=full --track-origins=yes -v appname 2>&1 | tee valgrind.txt

# File Operations
RM      = rm
MK      = mkdir -p
CP      = cp

# Targetfolders
BINDST  = /usr/games
DATDST  = /usr/share/xizero
DOCDST  = /usr/doc/xizero   #remove for deb?

# Options to test:
INLINE  = --param inline-unit-growth=200 --param large-function-growth=300 -pipe
MMX     = -mmmx
SSE     = -mssse3 -mfpmath=sse
ATOM0   = -march=core2 -mtune=generic 
ATOM1   = -march=atom -mtune=atom #GCC >=4.5 only
ALI     = -falign-functions=32 -falign-labels=32 -falign-loops=32 -falign-jumps=32
#

# Compile:
default:
	$(CC) $(O2FLAGS) $(OPT) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEVELOP) $(DEBUG) #change to RELEASE
	#$(STRIP) $(TARGET1)
	$(XZDAT)
	
none:
	$(CC) $(O2FLAGS) $(OPT) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WAFLAGS) $(WWFLAGS) $(LINUX) $(DEVELOP)
	$(CPPCH) $(SOURCE1)
	$(GRIND)

install:
	$(MK) $(BINDST)
	$(CP) $(TARGET1) $(BINDST)
	$(MK) $(DATDST)
	$(CP) $(GAMEDAT) $(DATDST)

local:
	$(CC) $(O2FLAGS) $(OPT) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEVELOP) -Wall -Wextra -Weffc++
	$(STRIP) $(TARGET1)
	
data:
	$(XZDAT)

unit:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEBUG)
	$(STRIP) $(TARGET2)

clean:
	$(RM) $(TARGET1)
	$(RM) $(GAMEDAT)
