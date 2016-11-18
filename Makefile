CC = g++ 
DEBUG = -g -O0 -pedantic-errors 
CFLAGS = -Wall -std=c++11 -c -lncurses $(DEBUG) 
LFLAGS = -Wall -lmenu -lncurses -g -o

all: editor

editor: editor.o 
	$(CC) $(LFLAGS) editor editor.o

editor.o: editor.cpp editor.h
	$(CC) $(CFLAGS)  editor.cpp

.PHONY: 
	clean 

clean: 
	rm -rf *.o 
	rm -rf editor
