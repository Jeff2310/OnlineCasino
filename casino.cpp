//#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <random>
#include <unistd.h>

#include <sstream>
#include "gui.h"

const char suit[4][8]={"Club","Heart","Diamond","Ace"};
const char *number[14]={"0","A","2","3","4","5","6","7","8","9","10","J","Q","K" };
enum choice{YES = 0, NO = 1, QUIT = 2};

struct Card{
    int score;
    const char *num;
    const char *suit;
    Card():score(0),num(NULL),suit(NULL){}
}card[52];

char buffer[200];

void initCard(){
    for (int i = 0; i < 4; i++) {
        for (int j = 1; j <= 13; j++) {
            Card *tmp = &card[+i * 13 + j - 1];
            tmp->suit = suit[i];
            if (j == 1)
                tmp->score = 11;
            else if (j <= 10)
                tmp->score = j;
            else
                tmp->score = 10;
            tmp->num = number[j];
        }
    }
}

void shuffle(Card *arr, int n, std::default_random_engine *e){
    Card tmp;
    for(int i=0; i<n; i++){
        int index = (*e)()%(n-i)+i;
        if(i!=index) {
            tmp = arr[i];
            arr[i] = arr[index];
            arr[index] = tmp;
        }
    }
}

choice getChoice(Window *win) {
    int ch;
    while(true) {
        wprint(win,"y/n/q?", 0, true);//
        ch = wgetch(win->win);
        switch (ch) {
            case 'y':
                wprint(win,"OK",0, true);
                return YES;
            case 'n':
                wprint(win, "Not Bad", 0, true);
                return NO;
            case 'q':
                wprint(win, "Bye", 0, true);
                return QUIT;
            default:
                wprint(win, "Please check your input", 0, true);
                continue;
        }
    }
}

//展示手牌
void displayHand(Window *win, Card *hand, int n,int score){
    if(!n) return;
    std::stringstream ss;
    wprint(win, "Your current hand is", 0, true);
    ss<<"#";
    for(int i=0; i<n; i++){
        ss<<"[" <<hand[i].suit<<' '<<hand[i].num<<"] ";
    }
    ss<<"#";
    wprint(win, ss.str().c_str(), 0, true);
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "Sum: %d", score);
    wprint(win, buffer, 500, true);
}

void drawCard(Window *win, Card *hand, int *size, int *score, int *index){
    hand[(*size)++]=card[*index];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer,"You have picked [%s %s]", card[*index].suit, card[*index].num);
    wprint(win, buffer, 1000, true);
    *score+=card[*index].score;
    (*index)++;
    displayHand(win,hand,*size,*score);
}

int main() {

    init();
    Window *status = new Window(3, COLS, LINES-3, 0, *simple);
    Window *p[2];
    p[0] = new Window(LINES-3, COLS/2-1, 0, 0, *simple);
    p[1] = new Window(LINES-3, COLS/2-1, 0, COLS/2+1, *simple);

    p[0]->create_box(true);
    p[1]->create_box(true);

    status->create_box(true);

    Card hand[2][21];
    int handSize[2],score[2];
    //flag 0=初始, 1=退出, 2=爆牌, 3=等待拼点, 4=拼点
    int flag, index, turn;
    std::default_random_engine e((unsigned)time(NULL));
    initCard();//初始化数组

    memset(buffer, 0, sizeof(buffer));
    wclear(status->win);
    sprintf(buffer,"澳门首家线上赌场上线啦！性感荷官在线发牌。");
    wmove(status->win, 1, 1);
    wprint(status, buffer, 2000, true);

    while(true){
        memset(buffer, 0, sizeof(buffer));
        wclear(status->win);
        sprintf(buffer, "Game started，the dealer is player %d ", e()%2+1);
        wmove(status->win, 1, 1);
        wprint(status, buffer, 2000, true);

        shuffle(card,52, &e);

        memset(hand, 0, sizeof(hand));
        memset(handSize, 0, sizeof(handSize));
        memset(score, 0, sizeof(score));
        flag =  index = turn = 0;
        while(true){
            memset(buffer, 0, sizeof(buffer));
            wmove(status->win, 1, 1);
            sprintf(buffer, "TURN %d", ++turn);
            wprint(status, buffer, 1000, true);
            p[0]->create_box(false);
            p[0]->create_box(false);

            for(int i=0; i<2; i++){
                wclear(p[i]->win);
                wmove(p[i]->win,1,1);

                memset(buffer, 0, sizeof(buffer));
                wclear(status->win);
                sprintf(buffer, "Player %d 's move", i+1);
                wmove(status->win, 1, 1);
                wprint(status, buffer, 0, true);

                displayHand(p[i], hand[i],handSize[i],score[i]);
                //抽牌
                if(turn == 1){
                    drawCard(p[i], hand[i], &handSize[i], &score[i], &index);
                    continue;
                }
                wprint(p[i],"Draw a card",0, true);
                choice c = getChoice(p[i]);
                if(c==YES){
                    drawCard(p[i], hand[i], &handSize[i], &score[i], &index);
                    if(score[i]>21){
                        memset(buffer, 0, sizeof(buffer));
                        sprintf(buffer, "Player %d busted, winner is player %d ", i+1, 2-i);
                        wclear(p[i]->win);
                        wmove(status->win, 1, 1);
                        wprint(status, buffer, 2000, true);
                        flag = 2;
                        break;
                    }
                }else if(c==NO){
                    if(i==0) flag = 3;
                    else
                        if(flag == 3) flag = 4;
                    sleep(1);
                }else{
                    flag = 1;
                    break;
                }
            }
            if(flag&&flag<3) break;
            //拼点
            if(flag==4){
                if(score[0]==score[1]){
                    wclear(status->win);
                    wmove(status->win, 1, 1);
                    wprint(status, "push", 1000, true);
                    break;
                }else{
                    memset(buffer, 0, sizeof(buffer));
                    sprintf(buffer, "Player %d won\n", 1+(score[0]>score[1]?0:1));
                    wclear(status->win);
                    wmove(status->win, 1, 1);
                    wprint(status, buffer, 1000, true);
                    break;
                }
            }
        }
        wclear(status->win);
        wmove(status->win, 1, 1);
        wprint(status, "Game Over", 1000, true);
        if(flag==1) break;
        wclear(status->win);
        wmove(status->win, 1, 1);
        wprint(status, "Restarting", 1000, true);
    }
    getch();
    endwin();
    return 0;
}