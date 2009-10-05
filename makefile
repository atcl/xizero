# Makefile

# Compile Flags
CC      = g++
WWFLAGS = -Wno-multichar -Wno-write-strings -Wno-pragmas
WAFLAGS = -Wall
O0FLAGS = -O0
O2FLAGS = -O2
OPT     = -ffast-math -fomit-frame-pointer -funroll-loops -floop-optimize -funit-at-a-time
LDFLAGS = -lfltk
OUTPARA = -o 
DEBUG   = -g
LINUX   = -DLINUX
WIN32   = -DWIN32
DEVELOP = -DDEV
RELEASE = -DREL
X86     = -DX86

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

# Strip
STRIP   = strip -s -R .comment -R .gnu.version

# File Operations
RM      = rm
MK      = mkdir -p
CP      = cp

# Targetfolders
BINDST  = /usr/games
DATDST  = /usr/share/xizero
DOCDST  = /usr/doc/xizero   #remove for deb?

# Options to test:
CPU     = -mmmx
ALI     = -falign-functions=32 -falign-labels=32 -falign-loops=32 -falign-jumps=32
#

# Compile:
default:
	$(CC) $(O2FLAGS) $(OPT) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEVELOP) $(X86) #change to RELEASE
	$(STRIP) $(TARGET1)
	#$(XZDAT) #activate

install:
	$(MK) $(BINDST)
	$(CP) $(TARGET1) $(BINDST)
	$(MK) $(DATDST)
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
	$(RM) $(TARGET1)
	$(RM) $(GAMEDAT)
