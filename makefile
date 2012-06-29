SRC = xizero.cc
BIN = -o /tmp/test
INC = -I/usr/include/libdrm
LIB = -ldrm
DEF = -DTITLE=\"XIZERO\" -DVERSION=\"0.1\" -DALWAYS -DSSE -DXRES=800 -DYRES=600
OPT = -march=atom -mtune=atom -m32 -std=c++0x -fno-exceptions -fno-rtti -O2 -fomit-frame-pointer -pipe -falign-loops -flto
WRN = -Wall -Wextra -Winline -Wlogical-op -Wc++0x-compat -Wparentheses -Weffc++ -pedantic

.PHONY: default

default:
	gcc $(SRC) $(BIN) $(INC) $(LIB) $(DEF) $(OPT) $(WRN)
	clang $(SRC) $(BIN) $(INC) $(LIB) $(DEF) $(OPT) $(WRN)
	cppcheck -a -v -s $(SRC)
	ldd $(BIN)
	
