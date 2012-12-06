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
	scan-build -o /tmp   gcc $(SRC) -o $(BIN) $(INC) $(LIB) $(DEF) $(OPT) $(WRN) $(DBG)
	scan-build -o /tmp clang $(SRC) -o $(BIN) $(INC) $(LIB) $(DEF) $(OPT) $(WRN) $(DBG)
	cppcheck --enable=all --enable=style --enable=performance --std=c++11 -v $(SRC)
	ldd $(BIN)

check:
	touch $(EXE).perf
	echo -n $$( date +"%d.%m.%Y | %H%M | " ) >> $(EXE).perf
	/usr/bin/time -f " %C | %U | %M" -a -o $(EXE).perf ./$(EXE) > /dev/null


