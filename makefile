ifeq ($(OS),Windows_NT)
    EXE  := .exe
endif

CC 		  = g++
LIB       = lib
INCLUDE   = include
SOURCES   = $(wildcard *.cpp)

all: controllershell$(EXE)

controllershell$(EXE): $(SOURCES)
	$(CC)              \
        $(DWIN)        \
		-I$(INCLUDE)   \
		-L$(LIB)       \
		-lsdl2         \
		-lsdl2main     \
		-lsdl2_ttf     \
		-linireader    \
		-std=c++11 	   \
		$(SOURCES)	   \
		-o controllershell