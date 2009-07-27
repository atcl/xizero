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
STATIC  = -static
OS      = -DLINUX

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
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(OS) $(DEBUG)

static:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(OS) $(STATIC)

unit:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS) $(OS) $(DEBUG)

fast:
	$(CC) $(O3FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(OS)

test:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET5) $(SOURCE5) $(LDFLAGS) $(WWFLAGS) $(OS) $(DEBUG)

debug:
	$(CC) $(O0FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WAFLAGS) $(OS) $(DEBUG)

all:
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(OS)
	$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS) $(OS)
	#$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET3) $(SOURCE3) $(LDFLAGS) $(WWFLAGS) $(OS)
	#$(CC) $(O2FLAGS) $(OUTPARA) $(TARGET4) $(SOURCE4) $(LDFLAGS) $(WWFLAGS) $(OS)

clean:
	$(CLEANUP) $(TARGET1)
