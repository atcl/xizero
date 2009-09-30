# Makefile

CC      = g++
WWFLAGS = -Wno-multichar -Wno-write-strings -Wno-pragmas
WAFLAGS = -Wall
O0FLAGS = -O0
O2FLAGS = -O2
O3FLAGS = -O3
LDFLAGS = -lfltk
OUTPARA = -o 
CLEANUP = rm
DEBUG   = -g
LINUX   = -DLINUX
WIN32   = -DWIN32
DEVELOP = -DDEV
RELEASE = -DREL
XZDAT   = dat/makexizerodat.sh

SOURCE1 = src/xizero.cc
TARGET1 = xizero

SOURCE2 = src/unittest.cc
TARGET2 = unittest

TARGET3 = utl/clbuilder

TARGET4 = utl/asciimissile

STRIP   = strip -s -R .comment -R .gnu.version

GAMEDAT = xizero.dat

COPY    = cp
BINDST  = /usr/games/
DATDST  = /usr/share/xizero/

#options to test:
OPT     = -ffast-math -fomit-frame-pointer -funroll-loops -floop-optimize -funit-at-a-time
CPU     = -mmmx
ALI     = -falign-functions=32 -falign-labels=32 -falign-loops=32 -falign-jumps=32
#

# Compile!
default:
	$(CC) $(O2FLAGS) $(OPT) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEVELOP) #change to RELEASE
	$(STRIP) $(TARGET1)
	#$(XZDAT) #activate

install:
	$(CP) $(TARGET1) $(BINDST)
	$(CP) $(GAMEDAT) $(DATDST)

local:
	$(CC) $(O2FLAGS) $(OPT) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEVELOP)
	$(STRIP) $(TARGET1)
	
data:
	$(XZDAT)

unit:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEBUG)
	$(STRIP) $(TARGET2)

debug:
	$(CC) $(O0FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WAFLAGS) $(LINUX) $(DEBUG)
	
win32:
	$(CC) $(O0FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(WIN32)

clean:
	$(CLEANUP) $(TARGET1)
	$(CLEANUP) $(GAMEDAT)
