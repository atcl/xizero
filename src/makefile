# Makefile

CC      = g++
CCFLAGS = -I/usr/include/freetype2 
WWFLAGS = -Wno-multichar -Wno-write-strings
O2FLAGS = -O2
O3FLAGS = -O3
LDFLAGS = -lfltk -lX11
SOURCE1 = xizero.cc
TARGET1 = -o xizero
SOURCE2 = unittest.cc
TARGET2 = -o unittest
TARGET3 = clbuilder
TARGET4 = asciimissile

# Compile!
default:
	$(CC) $(CCFLAGS) $(O2FLAGS) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS)

fast:
	$(CC) $(CCFLAGS) $(O3FLAGS) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS)

all:
	$(CC) $(CCFLAGS) $(O2FLAGS) $(TARGET1) $(SOURCE1) $(LDFLAGS) $(WWFLAGS)
	$(CC) $(CCFLAGS) $(O2FLAGS) $(TARGET2) $(SOURCE2) $(LDFLAGS) $(WWFLAGS)
	#$(CC) $(CCFLAGS) $(O2FLAGS) $(TARGET3) $(SOURCE3) $(LDFLAGS) $(WWFLAGS)
	#$(CC) $(CCFLAGS) $(O2FLAGS) $(TARGET4) $(SOURCE4) $(LDFLAGS) $(WWFLAGS)

clean:
	rm xizero
	rm unittest