#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <random>
#include <unistd.h>

const char suit[4][5]={"÷��","����","��Ƭ","����"};
//TODO:��T��Ϊ10
const char *number[14]={"0","A","2","3","4","5","6","7","8","9","10","J","Q","K" };
enum choice{YES = 0, NO = 1, QUIT = 2};

struct Card{
    int score;
    const char *num;
    const char *suit;
    Card():score(0),num(NULL),suit(NULL){}
}card[52];

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
        int index = (*e)()%(n-i)+i; //TODO:�ĳ�c++�汾�����
        if(i!=index) {
            tmp = arr[i];
            arr[i] = arr[index];
            arr[index] = tmp;
        }
    }
}

choice getChoice() {
    char str[100];
    while(true) {
        printf("(y/n/q?)");
        scanf("%s", str);
        if (str[1]) {
            printf("�����������\n");
            continue;
        }
        switch (str[0]) {
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
                printf("�����������\n");
                continue;
        }
    }
}


//չʾ����
void displayHand(Card *hand, int n,int score){
    if(!n) return;
    putchar('#');
    for(int i=0; i<n; i++){
        printf("��%s%s�� ",hand[i].suit,hand[i].num);
    }
    //�����������һ���ո�ɾ��
    printf("\b#\n����%d��\n",score);
}

void drawCard(Card *hand, int *size, int *score, int *index){
    hand[(*size)++]=card[*index];
    printf("�����һ�š�%s%s��\n", card[*index].suit, card[*index].num);
    *score+=card[*index].score;
    (*index)++;
    displayHand(hand,*size,*score);
    sleep(1);
}

int main() {
    Card hand[2][21];
    int handSize[2],score[2];
    //flag 0=��ʼ, 1=�˳�, 2=����, 3=�ȴ�ƴ��, 4=ƴ��
    int flag, index, turn;
    std::default_random_engine e((unsigned)time(NULL));
    printf("�����׼����϶ĳ����������Ըкɹ����߷��ơ�\n21����Ϸ���մ̼�\n");
    initCard();
    while(true){
        printf("��Ϸ��ʼ\n");
        printf("��� %d Ϊׯ��\n", e()%2+1);
        shuffle(card,52, &e);
        //displayHand(card,52,0); //debug
        memset(hand, 0, sizeof(hand));
        memset(handSize, 0, sizeof(handSize));
        memset(score, 0, sizeof(score));
        flag =  index = turn = 0;
        while(true){
            printf("\n@TURN %d��",++turn);
            for(int i=0; i<2; i++){
                printf("\n��� %d �Ļغϣ� \n", i+1);
                displayHand(hand[i],handSize[i],score[i]);
                //����
                if(turn == 1){
                    drawCard(hand[i], &handSize[i], &score[i], &index);
                    continue;
                }
                printf("�Ƿ��һ���ƣ�\n");
                choice c = getChoice();
                if(c==YES){
                    drawCard(hand[i], &handSize[i], &score[i], &index);
                    if(score[i]>21){
                        printf("��� %d ���ƣ����֣� ��� %d ��ʤ\n", i+1, 2-i);
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
            //ƴ��
            if(flag==4){
                if(score[0]==score[1]){
                    printf("push��\n");
                    break;
                }else{
                    printf("��� %d ���ʤ��\n", 1+(score[0]>score[1]?0:1));
                    break;
                }
            }
        }
        printf("��Ϸ����\n");
        sleep(1);
        if(flag==1) break;
        printf("���¿�ʼ��Ϸ\n============\n");
    }
    return 0;
}