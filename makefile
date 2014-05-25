CC 		  = g++
LIB       = /usr/local/lib/
INCLUDE   = /usr/local/include/
SOURCES   = $(wildcard *.cpp)

all: controllershell

controllershell: Entry.cpp Entry.h
	$(CC) 			   \
		-I$(INCLUDE)   \
		-L$(LIB) 	   \
		-lsdl2    	   \
		-lsdl2main     \
		-lsdl2_ttf     \
		-liniparser    \
		-std=c++11     \
		$(SOURCES)	   \
		-o controllershell