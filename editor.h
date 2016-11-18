#ifndef EDITOR_H
#define	EDITOR_H

#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <curses.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4

using namespace std;
static bool isOpen = false;
static bool closeProg = false;

void bringUpMenu();
char* filePrompt();
void displayError(int error);
void saveAs(char** array);
void save(int fd, char** array);
void editor(const char* filename);
void print_in_middle(WINDOW *win, int starty, int startx, int width, chtype color);
void exit(MENU my_menu, WINDOW* my_menu_win, ITEM my_items, int n_choices);

#endif	/* EDITOR_H */