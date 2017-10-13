#ifndef ONLINECASINO_GUI_H
#define ONLINECASINO_GUI_H

#include <ncurses/curses.h>

class Win_border{
private:
    chtype ls, rs, ts, bs, tl, tr, bl, br;
public:
    Win_border(chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br){
        this->ls = ls;
        this->rs = rs;
        this->ts = ts;
        this->bs = bs;
        this->tl = tl;
        this->tr = tr;
        this->bl = bl;
        this->br = br;
    }
    void use(WINDOW *win){
        wborder(win,ls, rs, ts, bs, tl, tr, bl ,br);
        //wrefresh(win);
        refresh();
    }
};

extern Win_border *simple;
extern Win_border *blank;

class Window{
private:
    int height, width, startx, starty;
    Win_border bd;
public:
    WINDOW *win;
    Window():height(0),width(0),startx(0),starty(0),bd(*blank){
        win = NULL;
    }
    Window(int h, int w, int y, int x, Win_border b):height(h),width(w),startx(x),starty(y),bd(b){
        win = newwin(h,w,y,x);
    }
    ~Window(){
        create_box(false);
        wrefresh(win);
        delwin(win);
    }
    void create_box(bool flag)
    {
        int i, j;
        int x, y, w, h;

        x = startx;
        y = starty;
        w = width;
        h = height;

        if(flag == TRUE)
        {
            bd.use(win);
        }
        else
            for(j = y; j <= y + h; ++j)
                for(i = x; i <= x + w; ++i)
                    mvaddch(j, i, ' ');
        wrefresh(win);
    }
    
    int getWidth(){
        return width;
    }
};

extern Window *status;
extern Window *p1;
extern Window *p2;

extern void init();
extern void wprint(Window *win,const char * str, float delay, bool centered);

#endif //ONLINECASINO_GUI_H

