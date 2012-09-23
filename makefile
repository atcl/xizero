SRC = xizero.cc
BIN = /tmp/test
INC = -I/usr/include/libdrm
LIB = -ldrm
DEF = -DTITLE=\"XIZERO\" -DVERSION=\"0.1\" -DALWAYS -DXRES=800 -DYRES=600 #-DSSE
OPT = -march=atom -mtune=atom -m32 -std=c++11 -fno-exceptions -fno-rtti -O2 -fomit-frame-pointer -pipe -falign-loops -flto
WRN = -pedantic -g -Wall -Wextra -Winline -Wlogical-op -Wc++11-compat -Wparentheses -Weffc++ -Wmissing-declarations -Wredundant-decls -Wshadow -Wstrict-aliasing 

.PHONY: default

default:
	scan-build -o /tmp   gcc $(SRC) -o $(BIN) $(INC) $(LIB) $(DEF) $(OPT) $(WRN)
	scan-build -o /tmp clang $(SRC) -o $(BIN) $(INC) $(LIB) $(DEF) $(OPT) $(WRN)
	cppcheck -a -v -s $(SRC)
	ldd $(BIN)
	
