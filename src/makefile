# Makefile

CC      = g++
CCFLAGS = -I/usr/include/freetype2 
WWFLAGS = -Wno-multichar -Wno-write-strings
O2FLAGS = -O2
O3FLAGS = -O3
LDFLAGS = -lfltk -lX11 -laudiere
OUTPARA = -o 
CLEANUP = rm
DEBUG   = -g
STATIC  = -static

SOURCE1 = xizero.cc
TARGET1 = xizero

SOURCE2 = unittest.cc
TARGET2 = unittest

TARGET3 = clbuilder

TARGET4 = asciimissile

SOURCE5 = testx.cc
TARGET5 = testx


# Compile!
default:
	$(CC) $(CCFLAGS) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(DEBUG)

static:
	$(CC) $(CCFLAGS) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS) $(STATIC)

unit:
	$(CC) $(CCFLAGS) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS)

fast:
	$(CC) $(CCFLAGS) $(O3FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS)

testx:
	$(CC) $(CCFLAGS) $(O3FLAGS) $(OUTPARA) $(TARGET5) $(SOURCE5) $(LDFLAGS) $(WWFLAGS)

all:
	$(CC) $(CCFLAGS) $(O2FLAGS) $(OUTPARA) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS)
	$(CC) $(CCFLAGS) $(O2FLAGS) $(OUTPARA) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS)
	#$(CC) $(CCFLAGS) $(O2FLAGS) $(OUTPARA) $(TARGET3) $(SOURCE3) $(LDFLAGS) $(WWFLAGS)
	#$(CC) $(CCFLAGS) $(O2FLAGS) $(OUTPARA) $(TARGET4) $(SOURCE4) $(LDFLAGS) $(WWFLAGS)

clean:
	$(CLEANUP) $(TARGET1)
	$(CLEANUP) $(TARGET2)
