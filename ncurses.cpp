#include <ncurses/ncurses.h>
#include <clocale>
#include <afxres.h>
#include <cstring>
#include "gui.h"

Win_border *blank = new Win_border(' ',' ',' ',' ',' ',' ',' ',' ');
Win_border *simple = new Win_border('|', '|', '-', '-', '+', '+', '+', '+');

void init(){
    setlocale(LC_ALL,"");
    initscr();
    start_color();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    curs_set(false);
}

void wprint(Window *win,const char * str, float delay, bool centered){
    int y, x;
    getyx(win->win, y, x);
    if(centered){
        x = (win->getWidth()-strlen(str))/2;
    }else{
        x=1;
    }
    refresh();
    mvwprintw(win->win, y, x, str);
    wmove(win->win, y+1, x);
    win->create_box(true);
    refresh();
    Sleep(delay);
}

