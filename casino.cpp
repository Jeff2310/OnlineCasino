#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string.h>

//TODO:尝试把文字改成Unicode符号
const char suit[4][5]={"梅花","红桃","方片","黑桃"};
//TODO:把T改为10
const char number[14]={'0','A','2','3','4','5','6','7','8','9','T','J','Q','K'};
enum choice{YES = 0, NO = 1, QUIT = 2, UNDEFINED = 3};

struct Card{
    int score;
    char num;
    const char *suit;
    Card():score(0),num(number[0]),suit(NULL){}
}card[52];

//TODO:自定义洗多少副牌
void initCard(){
    for(int i=0; i<4; i++){
        for(int j=1; j<=13; j++){
            Card *tmp = &card[i*13+j-1];
            tmp->suit=suit[i];
            if(j==1)
                tmp->score = 11;
            else if(j<=10)
                tmp->score= j;
            else
                tmp->score= 10;
            tmp->num = number[j];
        }
    }
}

void shuffle(Card *c, int n){
    Card tmp;
    for(int i=0; i<n; i++){
        int index = rand()%(n-i)+i; //TODO:改成c++版本随机数
        if(i!=index) {
            tmp = card[i];
            card[i] = card[index];
            card[index] = tmp;
        }
    }
}

//TODO: 改为需要检查输入重新询问
choice getChoice() {
    printf("(y/n/q?)");
    char str[100];
    scanf("%s", str);
    if (str[1]){
        printf("请您检查输入\n");
        return UNDEFINED;
    }
    switch(str[0]) {
        case 'y':
            printf("OK\n");
            return YES;
        case 'n':
            printf("Not Bad\n");
            return NO;
        case 'q':
            printf("Bye\n");
            return QUIT;
        default:
            printf("请您检查输入\n");
            return UNDEFINED;
    }
}

void displayHand(Card *hand, int n,int score){
    if(!n) return;
    putchar('#');
    for(int i=0; i<n; i++){
        printf("【%s%c】 ",hand[i].suit,hand[i].num);
    }
    printf("\b#\n共计%d点\n",score);
}

void drawCard(Card *hand, int *size, int *score, int *index){
    hand[(*size)++]=card[*index];
    printf("您获得一张【%s%c】\n", card[*index].suit, card[*index].num);
    *score+=card[*index].score;
    (*index)++;
    displayHand(hand,*size,*score);
}

int main() {
    Card hand[2][21];
    int handSize[2],score[2];
    //flag 0=初始, 1=退出, 2=爆牌, 3=等待拼点, 4=拼点
    int flag, index, turn;
    srand(time(NULL));
    initCard();
    while(true){
        //TODO: 加点骚话
        printf("游戏开始\n");
        printf("玩家 %d 为庄主\n", rand()%2+1);
        shuffle(card,52);
        //displayHand(card,52,0); //debug
        memset(hand, 0, sizeof(hand));
        memset(handSize, 0, sizeof(handSize));
        memset(score, 0, sizeof(score));
        flag =  index = turn = 0;
        while(true){
            printf("\n@TURN %d：",++turn);
            for(int i=0; i<2; i++){
                printf("\n玩家 %d 的回合： \n", i+1);
                displayHand(hand[i],handSize[i],score[i]);
                //抽牌
                if(turn == 1){
                    drawCard(hand[i], &handSize[i], &score[i], &index);
                    continue;
                }
                printf("是否抽一张牌？\n");
                choice c = getChoice();
                if(c==YES){
                    drawCard(hand[i], &handSize[i], &score[i], &index);
                    if(score[i]>21){
                        printf("玩家 %d 爆牌，出局\n", i+1);
                        flag = 2;
                        break;
                    }
                }else if(c==NO){
                    if(i==0) flag = 3;
                    else
                        if(flag == 3) flag = 4;
                }else{
                    flag = 1;
                    break;
                }
            }
            if(flag&&flag<3) break;
            //拼点
            if(flag==4){
                if(score[0]==score[1]){
                    printf("push！\n");
                    break;
                }else{
                    printf("玩家 %d 获得胜利\n", 1+(score[0]>score[1]?0:1));
                    break;
                }
            }
        }
        if(flag==1) break;
    }
    return 0;
}