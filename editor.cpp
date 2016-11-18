#include "editor.h"

//prints text to the middle
void print_in_middle(WINDOW *win, int starty, int startx, int width, chtype color) {
    int length, x, y;
    float temp;

    if (win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = strlen("Menu");
    temp = (width - length) / 2;
    x = startx + (int) temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", "Menu");
    wattroff(win, color);
    refresh();
}



//calls the menu
void bringUpMenu(char**array = NULL, int fd = 0) {

    const char * choices[] = {
        "Open",
        "Save",
        "Save As",
        "Exit",
    };

    ITEM **my_items;
    int c;
    MENU *my_menu;
    WINDOW *my_menu_win;
    int n_choices, i;

    /* Initialize curses */

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    init_pair(1, COLOR_RED, COLOR_BLACK);

    /* Create items */
    n_choices = ARRAY_SIZE(choices);
    my_items = (ITEM **) calloc(n_choices, sizeof (ITEM *));

    for (i = 0; i < n_choices; ++i) {
        my_items[i] = new_item(choices[i], "");
    }

    /* Create menu */
    my_menu = new_menu((ITEM **) my_items);

    /* Create the window to be associated with the menu */
    my_menu_win = newwin(10, 40, 4, 4);
    keypad(my_menu_win, TRUE);

    /* Set main window and sub window */
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

    /* Set menu mark to the string " * " */
    set_menu_mark(my_menu, " * ");

    /* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);

    print_in_middle(my_menu_win, 1, 0, 40, COLOR_PAIR(1));

    mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
    mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
    mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
    refresh();
    /* Post the menu */
    post_menu(my_menu);
    wrefresh(my_menu_win);

    int response = 1;
    bool done = false;
    bool exitCalled = false;


    while (((c = wgetch(my_menu_win)) != KEY_F(1)) && !done && !exitCalled) {
        switch (c) {
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                ++response;

                if (response > 4) {
                    response = 4;
                }

                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                --response;

                if (response < 1) {
                    response = 1;
                }

                break;
            case '\n':

                done = true;
                //calls the corresponding functions 
                if (response == 1) {
                    if (!isOpen) {
                        const char * filename = filePrompt();
                        editor(filename);
                    } else {
                         WINDOW*overWindow = newwin(500, 500, 0, 0);
                        wprintw(overWindow, "There is an unsaved open file, save? (Y/N)");
                        wrefresh(overWindow);
                        char * response = new char[1];
                        wgetstr(overWindow, response);
                        if (response[0] == 'Y' || response[0] == 'y') {
                            delwin(overWindow);
                            save(fd, array);
                        } else {
                            delwin(overWindow);
                            const char * filename = filePrompt();
                            editor(filename);
                        }
                    }
                } else if (response == 2) //save
                {
                    if (array != NULL) {
                        save(fd, array);
                    } else {
                        printw("There is no file open, open file? (Y/N)");
                        char * response = new char[1];
                        getstr(response);
                        if (response[0] == 'Y' || response[0] == 'y') {
                            const char * filename = filePrompt();
                            editor(filename);
                        } else {
                            werase(my_menu_win);
                            wclear(my_menu_win);
                            wrefresh(my_menu_win);
                            unpost_menu(my_menu);
                            free_menu(my_menu);

                            for (int i = 0; i < n_choices; ++i) {
                                free_item(my_items[i]);
                            }
                        }

                    }
                } else if (response == 3) {
                    if (array != NULL) {
                        saveAs(array);
                    } else {
                        printw("There is no file open, open file? (Y/N)");
                        char * response = new char[1];
                        getstr(response);

                        if (response[0] == 'Y' || response[0] == 'y') {
                            const char * filename = filePrompt();
                            editor(filename);
                        } else {
                            werase(my_menu_win);
                            wclear(my_menu_win);
                            wrefresh(my_menu_win);
                            unpost_menu(my_menu);
                            free_menu(my_menu);

                            for (int i = 0; i < n_choices; ++i) {
                                free_item(my_items[i]);
                            }
                            endwin();
                        }
                    }

                } else if (response == 4) {
                    exitCalled = true;
                    if (!isOpen) {
                        werase(my_menu_win);
                        wclear(my_menu_win);
                        wrefresh(my_menu_win);
                        unpost_menu(my_menu);
                        free_menu(my_menu);

                        for (int i = 0; i < n_choices; ++i) {
                            free_item(my_items[i]);
                        }
                        endwin();
                    } else {
                        WINDOW*overWindow = newwin(500, 500, 0, 0);
                        wprintw(overWindow, "There is an unsaved open file, save? (Y/N)");
                        wrefresh(overWindow);
                        char * response = new char[1];
                        wgetstr(overWindow, response);
                        if (response[0] == 'Y' || response[0] == 'y') {
                            delwin(overWindow);
                            save(fd, array);
                        } else {
                            werase(my_menu_win);
                            wclear(my_menu_win);
                            wrefresh(my_menu_win);
                            unpost_menu(my_menu);
                            free_menu(my_menu);
                            for (int i = 0; i < n_choices; ++i) {
                                free_item(my_items[i]);
                            }
                            delwin(overWindow);
                            endwin();
                            break;
                        }
                    }

                }
                break;


        }
        wrefresh(my_menu_win);
    }

    if (exitCalled == false) {
        unpost_menu(my_menu);
        free_menu(my_menu);

        for (i = 0; i < n_choices; ++i) {
            free_item(my_items[i]);
        }
        /*endwin();*/
    }
}


//displays a window with a prompt to the user to enter a filename
char* filePrompt() {
    char * filename = new char[256];
    initscr();
    WINDOW *fileWin = newwin(500, 500, 0, 0);
    wprintw(fileWin, "Please enter a filename: ");
    wrefresh(fileWin);
    wgetstr(fileWin, filename);
    delwin(fileWin);
    endwin();
    return filename;
}

//window to display error messages
void displayError(int error) {
    initscr();
    WINDOW *errWin = newwin(500, 500, 0, 0);
    wprintw(errWin, strerror(error));
    wrefresh(errWin);
    wgetch(errWin);
    delwin(errWin);
    endwin();
}

//creates a new filedescriptor and
//saves the file accordingly
void saveAs(char **array) {
    isOpen = false;
    int fd;
    char * name = new char[256];

    WINDOW* file = newwin(500, 500, 0, 0);

    wprintw(file, "Save File As: ");
    wrefresh(file);

    wgetstr(file, name);
    delwin(file);
    endwin();


    fd = open(name, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd == -1) {
        char * response = new char[1];

        displayError(errno);
        WINDOW*overWindow;
        overWindow = newwin(500, 500, 0, 0);
        wprintw(overWindow, "Overwrite Existing File? (Y/N) ");
        wgetstr(overWindow, response);
        if (response[0] == 'Y' || response[0] == 'y') {

            delwin(overWindow);
	    fd = open(name, O_WRONLY | O_TRUNC);
            save(fd, array);

        } else {

            wclear(overWindow);
            wprintw(overWindow, "Did not save");
            wrefresh(overWindow);
            delwin(overWindow);

        }

    } else {
        save(fd, array);
    }

}

//saves the characters using the preexisting
//filedescriptor
void save(int fd, char**dArray) {
    isOpen = false;
    for (int i = 0; i < 1000; i++) {
        int n = write(fd, dArray[i], ARRAY_SIZE(dArray[i]));
        if (n == -1) {
            displayError(errno);
        }
    }
}

//main function
void editor(const char* filename) {
    //Opening a file for reading and writing	
    int fd = open(filename, O_RDWR | O_CREAT, 0666);

    if (fd == -1) {
        displayError(errno);
    } else {

        isOpen = true;
        //initializing the Window
        initscr();
        raw();
        refresh();

        //Initializing the WINDOW variables, and setting their locations
        WINDOW* menuToolBar = newwin(1, COLS, 0, 0);
        WINDOW* titleToolBar = newwin(1, 18, 0, (COLS / 2) - 9);
        WINDOW* currentFileOpen = newwin(1, COLS, LINES - 1, 0);
        WINDOW* displayArea = newwin(LINES - 2, COLS, 1, 0);

        //Enabling accepting buttons inputs from Keypad
        keypad(displayArea, TRUE);
        //Defining the values of the menuToolBar, titleToolBar, and the file  currently
        //open in order to closer mimic what 
        wprintw(menuToolBar, "F1: Menu");
        wprintw(titleToolBar, "CSCI 1730 Editor!");
        wprintw(currentFileOpen, filename);

        wmove(displayArea, 0, 0);

        wrefresh(menuToolBar);
        wrefresh(titleToolBar);
        wrefresh(currentFileOpen);
        wrefresh(displayArea);

        //2D Array that contains the values inputed
        char** textContainer;
        textContainer = new char *[1000];

        for (int i = 0; i < 1000; i++) {
            textContainer[i] = new char[COLS + 1];
        }
        for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < COLS + 1; j++) {
                textContainer[i][j] = '\0';
            }
        }

        //initializing the values for the cursor
        int cursRow = 0, cursCol = 0;
        int n, numLines = 0, curVal = 0;
        char buffer[256];

        initscr();
        wprintw(displayArea, "");
        refresh();
        def_prog_mode();
        endwin();

        //Adding characters in the file to a buffer and adding
        //the buffer to the ncurses window 
        while ((n = read(fd, buffer, 256)) > 0) {
            buffer[n] = '\0';
            reset_prog_mode();
            wrefresh(displayArea);
            wprintw(displayArea, buffer);

            for (int j = n; j > 0; j--) {
                int count = 0;
                while (count < COLS) {
                    textContainer[numLines][count] = buffer[curVal];
                    count++;
                    curVal++;
                }
                numLines++;
            }
            curVal = 0;
            wrefresh(displayArea);
            def_prog_mode();
            endwin();
        }
	int cd = close(fd);
        if(cd == -1)
        {
             displayError(errno);
        }
        fd = open(filename, O_WRONLY | O_TRUNC);
        if(fd == -1)
        {
             displayError(errno);
        }
        reset_prog_mode();
        wrefresh(displayArea);
        idlok(displayArea, TRUE);
        scrollok(displayArea, TRUE);
        wsetscrreg(displayArea, 0, LINES - 2);

        //Infinite loop that will only terminate when F1 
        //is called and exit is clicked inside that menu
        //does things for special characters, otherwise 
        //the character is added to the array and displayed 
        //to the screen
        int offset = 0;
        while (true) {

            int c = wgetch(displayArea);
            isOpen = true;
            if (c == KEY_UP) {
                if (cursRow > 0) {
                    cursRow--;
                } else {
                    if (offset > 0)
                        offset--;
                }
            } else if (c == KEY_DOWN) {
                if (cursRow < LINES - 2) {
                    cursRow++;
                } else {
                    if (offset < numLines)
                        offset++;
                }
            } else if (c == KEY_LEFT) {
                if (cursCol > 0) {
                    cursCol--;
                }
            } else if (c == KEY_RIGHT) {
                if (cursCol < COLS) {
                    cursCol++;
                }
            } else if (c == KEY_BACKSPACE) {
                if (cursCol > 0) {
                    for (int i = cursCol - 1; i < COLS; i++) {
                        textContainer[cursRow + offset][i] = textContainer[cursRow][i + 1];
                    }
                    cursCol--;
                } else {
                    if (cursRow + offset != 0) {
                        cursRow--;
                        cursCol = COLS - 1;
                    }
                }
            } else if (c == KEY_DL) {
                textContainer[cursRow+offset][cursCol] = ' ';
                for (int i = cursCol; i < COLS; i++) {
                    textContainer[cursRow + offset][i] = textContainer[cursRow][i + 1];
                }
            } else if (c == KEY_F(1)) {
                //Bring up the main menu
                bringUpMenu(textContainer, fd);

                if (closeProg) {
                    int cr = close(fd);
                    if (cr == -1)
                        displayError(errno);
                    break;
                }

            } else if (c == KEY_ENTER) {
                if (textContainer[cursRow + offset][cursCol] == '\0') {
                    cursRow++;
                    cursCol = 0;
                } else {
                    int tempVal = textContainer[cursRow + offset][cursCol];
                    textContainer[cursRow][cursCol] = '\n';
                    int tempVal2 = textContainer[cursRow + offset][cursCol + 1];
                    textContainer[cursRow][cursCol + 1] = '\0';

                    for (int i = numLines + 1; numLines > (cursRow + 1 + offset); i--) {
                        textContainer[i] = textContainer[i - 1];
                    }

                    textContainer[cursRow + 1 + offset][0] = tempVal;
                    textContainer[cursRow + 1 + offset][1] = tempVal2;
                    int v = cursCol + 1;

                    for (int i = 2; i < COLS; i++) {
                        if (v < COLS) {
                            textContainer[cursRow + 1 + offset][i] = textContainer[cursRow + offset][v];
                            v++;
                        } else
                            textContainer[cursRow + 1 + offset][i] = '\0';
                    }
                    cursRow = cursRow + 1;
                    numLines++;
                    cursCol = 0;
                }
            } else if (c == KEY_STAB) {
                textContainer[cursRow + offset][cursCol] = '\t';

            } else {
                int tempVal = textContainer[cursRow + offset][cursCol];
                textContainer[cursRow + offset][cursCol] = c;
                if (textContainer[cursRow + offset][COLS - 1] == '\0') {
                    for (int i = COLS - 1; i > cursCol + 1; i--) {
                        textContainer[cursRow + offset][i] = textContainer[cursRow + offset][i - 1];
                    }
                    textContainer[cursRow + offset][cursCol + 1] = tempVal;
                }
                //else consider line wrapping
                if (cursCol < COLS) {
                    cursCol++;
                }
            }
            wclear(displayArea);
            for (int i = offset; i < LINES - 2; i++) {
                wprintw(displayArea, textContainer[i]);
            }
            wrefresh(displayArea);
            wrefresh(menuToolBar);
            wrefresh(titleToolBar);
            wrefresh(currentFileOpen);
            wmove(displayArea, cursRow, cursCol);
        }
        //closing all windows and deallocating the array
        if (closeProg) {
            werase(menuToolBar);
            wclear(menuToolBar);
            delwin(menuToolBar);
            werase(titleToolBar);
            wclear(titleToolBar);
            delwin(titleToolBar);
            werase(currentFileOpen);
            wclear(currentFileOpen);
            delwin(currentFileOpen);
            werase(displayArea);
            wclear(displayArea);
            delwin(displayArea);
            endwin();
            for (int i = 0; i < 1000; i++) {
                delete[] textContainer[i];
            }
            delete[] textContainer;

        }
    }
}

//running the editor
int main(int argc, char *argv[]) {
    if (argc == 2) {
        const char * filename = argv[1];
        editor(filename);
    }
    if (argc == 1) {
        char ** array = NULL;
        bringUpMenu(array, 0);
    }
    return 0;
}

