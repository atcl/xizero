SRC = xizero.cc
BIN = /tmp/test
EXE = xizero
DEF = -DTITLE=\"XIZERO\" -DVERSION=\"0.1\" -DALWAYS -DXRES=640 -DYRES=480
COP = -march=native -mtune=native -m32 -std=c++0x -O2 -pipe -fomit-frame-pointer -falign-loops -fno-exceptions -fno-rtti
LOP = -flto
WRN = -pedantic -g -Wall -Wextra -Winline -Wlogical-op -Wc++11-compat -Wparentheses -Weffc++ -Wmissing-declarations -Wredundant-decls -Wshadow -Wstrict-aliasing 
DBG = -Wno-multichar -Wno-write-strings -Wno-pragmas -Wno-attributes -Wstrict-aliasing

.PHONY: default all test

default all:
	$(CC) $(SRC) -c -o $(EXE).o $(DEF) $(COP) $(REL)
	$(CC) $(EXE).o -o $(EXE) $(LOP)

test:
	scan-build -o /tmp   gcc $(SRC) -o $(BIN) $(DEF) $(COP) $(LOP) $(WRN) $(DBG)
	scan-build -o /tmp clang++ $(SRC) -o $(BIN) $(DEF) $(COP) $(LOP) $(WRN) $(DBG)
	cppcheck --enable=all --enable=style --enable=performance --std=c++11 -v $(SRC)
	ldd $(BIN)

check:
	touch $(EXE).perf
	echo -n $$( date +"%d.%m.%Y | %H%M | " ) >> $(EXE).perf
	/usr/bin/time -f " %C | %U | %M" -a -o $(EXE).perf ./$(EXE) > /dev/null

