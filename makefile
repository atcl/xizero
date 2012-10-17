SRC = xizero.cc
BIN = /tmp/test
EXE = xizero
DEF = -DTITLE=\"XIZERO\" -DVERSION=\"0.1\" -DALWAYS -DXRES=800 -DYRES=600
OPT = -march=atom -mtune=atom -m32 -std=c++11 -fno-exceptions -fno-rtti -O2 -fomit-frame-pointer -pipe -falign-loops -flto #-fwhole-program
WRN = -pedantic -g -Wall -Wextra -Winline -Wlogical-op -Wc++11-compat -Wparentheses -Weffc++ -Wmissing-declarations -Wredundant-decls -Wshadow -Wstrict-aliasing 
DBG = -Wno-multichar -Wno-write-strings -Wno-pragmas -Wno-attributes -Wstrict-aliasing
SSE = -DSSE

.PHONY: default all test

default all:
	$(CXX) $(SRC) -o $(EXE) $(INC) $(LIB) $(DEF) $(SSE) $(OPT) $(REL)

test:
	scan-build -o /tmp   gcc $(SRC) -o $(BIN) $(INC) $(LIB) $(DEF) $(OPT) $(DBG)
	scan-build -o /tmp clang $(SRC) -o $(BIN) $(INC) $(LIB) $(DEF) $(OPT) $(DBG)
	cppcheck -a -v -s $(SRC)
	ldd $(BIN)

