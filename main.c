#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum{
    OUT,   EMPTY,
    SECRET,CHARA,
    ENEMY, WEAPON,
    DEATHENEMY,
    PRISON,EXIT
} squareKind;

typedef struct{
    int enemy[3];
    int chara;
    squareKind map[7][7];
}cave;    

typedef struct{
    int hp;
    int weapon;
    int key;
    int killscore;
}chara;

typedef enum{
    NONE,       NOWEAPON,
    HAVEWEAPON, GUN,
    QUIT
} modeKind;

void p(squareKind map[][7]);
void quitGame();
void checkSquare();
int m[7][7];
int moveChara();
int makeEnemy();
int battleScene(int);
int stack;
int c; // moveCharaの文字取得

cave d;
chara ch = {30, 1, 0, 0};
squareKind sq;

modeKind mode;

int main(int argc, char *argv[]){
    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            if(i == 0 || i == 6 || j == 0 || j == 6){
                d.map[i][j] = OUT;
                m[i][j] = OUT;
            }else{
                d.map[i][j] = SECRET;
                m[i][j] = EMPTY;
            }
        }
    }
    srand(time(NULL));
    int r = rand()%30;

    d.chara = 15;

    m[1][5] = PRISON; m[3][4] = EXIT;  m[1][1] = ENEMY;
    m[3][2] = ENEMY;  m[5][5] = ENEMY; m[2][4] = WEAPON;

    d.enemy[0] = 30+r;
    r = rand()%30;
    d.enemy[1] = 30+r;
    d.enemy[2] = 30;

    p(d.map);
    while(1){
        if(moveChara() == -1) printf("壁があって進めない…\n");
        p(d.map);
        checkSquare();
    }

    return 0;
}

void p(squareKind map[][7]){
    for(int i=0; i<7; i++){
        for(int j=0; j<7; j++){
            if(d.chara/10 == i && d.chara%10 == j) printf("@");
            else if(i == 0 || i == 6)    printf("-");
            else if(j == 0 || j == 6)    printf("|");
            else if(map[i][j] == SECRET) printf("%c", '?');
            else if(map[i][j] == CHARA)  printf("%c", '@');
            else if(map[i][j] == PRISON) printf("%c", 'H');
            else if(map[i][j] == EXIT)   printf("%c", 'O');
            else if(map[i][j] == EMPTY)  printf("%c", ' ');
            else if(map[i][j] == ENEMY)  printf("%c", 'E');
            else if(map[i][j] == DEATHENEMY)  printf("%c", 'X');
        }
        printf("\n");
    }
}

int checkError(){
    int m = d.chara;
    return (m/10 > 5 || m/10 < 1 || m%10 < 1 || m%10 > 5 || m < 10 || m%10 == 0 || m>55);
}


int moveChara(){
    scanf("%lc%*c", &c);
    fflush(stdin);
    if(c == 'q') quitGame();
    if(c == 'r') p(d.map);
    
    sq = d.map[d.chara/10][d.chara%10];
    stack = d.chara;

    int mover;

    if(c == 's') mover = 10;
    if(c == 'w') mover = -10;
    if(c == 'a') mover = -1;
    if(c == 'd') mover = 1;
    if(c == 'e') mover = 0;

    d.chara += mover;
    if(checkError()){
        d.chara -= mover;
        return -1;
    }else{
        if (m[d.chara/10][d.chara%10] == WEAPON){
            m[stack/10][stack%10] = EMPTY;
            d.map[stack/10][stack%10] = EMPTY;
        }
        d.map[stack/10][stack%10] = m[stack/10][stack%10];
    }
        
    fflush(stdin);
    //putchar('\n');
}

void checkSquare(){
    int mover = 0;

    d.chara += mover;
    if (m[d.chara/10][d.chara%10] == WEAPON){
        ch.weapon = 2;
        printf("小さなナイフを手に入れた\n");
        m[d.chara/10][d.chara%10] = EMPTY;
        d.map[d.chara/10][d.chara%10] = EMPTY;
    }
    else if(m[d.chara/10][d.chara%10] == ENEMY){
        if(d.enemy[(d.chara%10)/2] = battleScene(d.enemy[(d.chara%10)/2]) == 0) m[d.chara/10][d.chara%10] = DEATHENEMY;
        //printf("enemyhp: %d\n",d.enemy[(d.chara%10)/2]);
    }
    d.map[stack/10][stack%10] = m[stack/10][stack%10];
}

void quitGame(){
    printf("本当に終了しますか?..y/n\n");
    scanf("%lc%*c", &c);
    if(c == 'y' || c == 'Y'){
        exit(EXIT_SUCCESS);
    }
}

int battleScene(int e){
    printf("=============================\n");
    printf("盗賊を見つけた。\n");
    if(ch.weapon == HAVEWEAPON){
        printf("武器もあるし、今ならやれそう\n");
        printf("戦う e/なにもしない q ");
        scanf("%lc%*c", &c);
        if(c == 'e'){
            e -= 15;
            if (e > 0)
            printf("かなりダメージを与えた\n");
        }
    }else if(ch.weapon == NOWEAPON){
        printf("丸腰だと勝てそうにない…けど…\n");
        printf("戦う e/なにもしない q ");
        scanf("%lc%*c", &c);
        if(c == 'e'){
            e -= 5;
            if (e > 0)
            printf("それなりに消耗している\n");
        }
    }else if(ch.weapon == GUN){
        printf("こちらに気が付いていない。しっかりと狙いを定めた。\n");
        printf("戦う e/なにもしない q ");
        scanf("%lc%*c", &c);
        if(c == 'e'){
            e = 0;
            printf("銃弾は脳天を貫き、盗賊は倒れた\n");
        }
    }

    if(e > 0){
        if(ch.hp == 1){
            ch.hp -= 1;
            printf("もう動けそうにない…ここまでみたいだ…\n");
            exit(EXIT_SUCCESS);
        }else if(ch.hp == 5){
            ch.hp -= 4;
            printf("傷が深い…視界がぼんやりしている\n");
        }else
            ch.hp -= 5;
    }

    if (e < 0){
        e = 0; //0下回った時の処理
        printf("盗賊は倒れた。手に嫌な感触が残った\n");
    }

    printf("キャラのHP: %d\n",ch.hp);
    return e;
}



void printExp();
int ran();

void printExp(){
    printf("\n%d) 武器なしで戦闘のテストをします\n", NOWEAPON);
    printf("%d) 武器を持ち戦闘のテストをします\n", HAVEWEAPON);
    printf("%d) 銃を持ち戦闘のテストをします\n", GUN);
    printf("%d) テストを終了します\n\n", QUIT);
}

