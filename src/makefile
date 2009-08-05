# Makefile

CC      = g++
WWFLAGS = -Wno-multichar -Wno-write-strings -Wno-pragmas
WAFLAGS = -Wall
O0FLAGS = -O0
O2FLAGS = -O2
O3FLAGS = -O3
LDFLAGS = -lfltk -laudiere
OUTPARA = -o 
CLEANUP = rm
DEBUG   = -g
LINUX   = -DLINUX
WIN32   = -DWIN32

SOURCE1 = xizero.cc
TARGET1 = xizero

SOURCE2 = unittest.cc
TARGET2 = unittest

TARGET3 = clbuilder

TARGET4 = asciimissile

STRIP   = strip -s -R .comment -R .gnu.version

#options to test:
CPUa    = -mcpu pentium-mmx
CPUb    = -mcpu pentium3
MATH    = -ffast-math -fsingle-precision-constant -fomit-frame-pointer
ATOM    = -xL
EXP     = -fprefetch-loop-arrays -funit-at-a-time -funroll-loops -floop-optimize
ALI     = -falign-functions=32 -falign-labels=32 -falign-loops=32 -falign-jumps=32 -malign-int
#

# Compile!
default:
	$(CC) $(O2FLAGS) $(MATH) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEBUG)
	$(STRIP) $(TARGET1)

static:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(STATIC) $(WWFLAGS) $(LINUX) 

unit:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEBUG)
	$(STRIP) $(TARGET2)

debug:
	$(CC) $(O0FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WAFLAGS) $(LINUX) $(DEBUG)
	
win:
	$(CC) $(O0FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WAFLAGS) $(WIN32)

all:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX)
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS) $(LINUX)
	#$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET3) $(SOURCE3) $(LDFLAGS) $(WWFLAGS) $(LINUX)
	#$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET4) $(SOURCE4) $(LDFLAGS) $(WWFLAGS) $(LINUX)

clean:
	$(CLEANUP) $(TARGET1)
