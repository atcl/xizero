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
STATIC  = -L/usr/lib/libfltk.a -lXft -lfontconfig -lXinerama -lm -lX11 -lfltk -static
LINUX   = -DLINUX
WIN32   = -DWIN32

SOURCE1 = xizero.cc
TARGET1 = xizero

SOURCE2 = unittest.cc
TARGET2 = unittest

TARGET3 = clbuilder

TARGET4 = asciimissile

SOURCE5 = testx.cc
TARGET5 = testx

#options to test:
CPUa    = -mcpu pentium-mmx
CPUb    = -mcpu pentium3
MATH    = -ffast-math
ATOM    = -xL
#


# Compile!
default:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEBUG)

static:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(STATIC) $(WWFLAGS) $(LINUX) 

unit:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEBUG)

fast:
	$(CC) $(O3FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX)

test:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET5) $(SOURCE5) $(LDFLAGS) $(WWFLAGS) $(LINUX) $(DEBUG)

debug:
	$(CC) $(O0FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WAFLAGS) $(LINUX) $(DEBUG)
	
win:
	$(CC) $(O0FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WAFLAGS) $(WIN32) $(STATIC)

all:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(LINUX)
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS) $(LINUX)
	#$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET3) $(SOURCE3) $(LDFLAGS) $(WWFLAGS) $(LINUX)
	#$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET4) $(SOURCE4) $(LDFLAGS) $(WWFLAGS) $(LINUX)

clean:
	$(CLEANUP) $(TARGET1)
