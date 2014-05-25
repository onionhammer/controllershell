CC 		  = clang++
LIB       = /usr/local/lib/
INCLUDE   = /usr/local/include/
SOURCES   = $(wildcard *.cpp)

all: controllershell

controllershell: Entry.cpp
	$(CC) 			   \
		-I$(INCLUDE)   \
		-L$(LIB) 	   \
		-lsdl2    	   \
		-lsdl2main     \
		-lsdl2_ttf     \
		-linireader    \
		-std=c++11 	   \
		-stdlib=libc++ \
		$(SOURCES)	   \
		-o controllershell