#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum{
    OUT,   EMPTY,
    SECRET,CHARA,
    ENEMY, WEAPON,
    DEATHENEMY, GDEATHENEMY,
    PRISON,EXIT,
    GUNS
} squareKind;

typedef enum{
    NOWEAPON, HAVEWEAPON,
    GUN,      FULL,
    QUIT
} modeKind;

typedef struct{
    int enemy[3][2];
    int chara;
    squareKind map[7][7];
}cave;    

typedef struct{
    int hp;
    modeKind weapon;
    int key;
    int killscore;
}chara;


void p(squareKind map[][7]);
void quitGame();
void checkSquare();
void move2Prison();
int m[7][7];
int moveChara();
int makeEnemy();
int battleScene(int);
int stack;
int c; // moveCharaの文字取得

cave d;
chara ch = {30, 0, 0, 0};
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
    int r = rand()%60;

    d.chara = 15;

    m[1][5] = PRISON; m[3][4] = EXIT;  m[1][1] = ENEMY;
    m[3][2] = ENEMY;  m[5][5] = ENEMY; m[2][4] = WEAPON;
    m[2][1] = GUNS;

    d.enemy[0][0] = 30+r;
    d.enemy[0][1] = 11;
    r = rand()%60;
    d.enemy[1][0] = 30+r;
    d.enemy[1][1] = 32;
    d.enemy[2][0] = 30;
    d.enemy[2][1] = 55;

    p(d.map);
    while(1){
        if(moveChara() == -1) printf("壁があって進めない…\n");
        p(d.map);
        checkSquare();
        printf("%d %d\n", ch.weapon, ch.hp);
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
        if (m[d.chara/10][d.chara%10] == WEAPON || m[d.chara/10][d.chara%10] == GUNS ){
            m[d.chara/10][d.chara%10] = EMPTY;
            d.map[d.chara/10][d.chara%10] = EMPTY;
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
        ch.weapon = HAVEWEAPON;
        printf("小さなナイフを手に入れた\n");
        m[d.chara/10][d.chara%10] = EMPTY;
        d.map[d.chara/10][d.chara%10] = EMPTY;
    }
    else if (m[d.chara/10][d.chara%10] == GUNS){
        if(ch.weapon == HAVEWEAPON) ch.weapon = FULL;
        else                        ch.weapon = GUN;
        printf("フリントロック式の銃だ。\n");
        m[d.chara/10][d.chara%10] = EMPTY;
        d.map[d.chara/10][d.chara%10] = EMPTY;
    }
    else if(m[d.chara/10][d.chara%10] == ENEMY){
        /*
        if(ch.weapon == GUN || ch.weapon == FULL){ 
            if(d.enemy[(d.chara%10)/2] = battleScene(d.enemy[(d.chara%10)/2]) == 0) m[d.chara/10][d.chara%10] = GDEATHENEMY;
        }
        else{
            if(d.enemy[(d.chara%10)/2] = battleScene(d.enemy[(d.chara%10)/2]) == 0) m[d.chara/10][d.chara%10] = DEATHENEMY;
        }*/
        int cc = battleScene(d.enemy[(d.chara%10)/2][0]);
        d.enemy[(d.chara%10)/2][0] = cc;
        if(cc == 0){
            m[d.chara/10][d.chara%10] = DEATHENEMY;
        printf("%d\n", cc);
        }
        //printf("enemyhp: %d\n",d.enemy[(d.chara%10)/2]);
    }
    else if(m[d.chara/10][d.chara%10] == DEATHENEMY){
        printf("血だまりに男が倒れている。もう動かないだろう\n");
    }
    else if(m[d.chara/10][d.chara%10] == GDEATHENEMY){
        printf("頭から中身がこぼれている…吐き気を催したが、こらえた。\n");
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
    int f = 0;


    // 自分の攻撃
    if(ch.weapon == HAVEWEAPON){
        printf("ナイフもあるし、今なら戦えそう\n");
        printf("戦う e/なにもしない q >");
        scanf("%lc%*c", &c);
        if(c == 'e'){
            e -= 15;
            if (e > 0)
            printf("かなりダメージを与えた\n");
            f = 1;
        }
    }else if(ch.weapon == NOWEAPON){
        printf("丸腰だと勝てそうにない…けど…\n");
        printf("戦う e/なにもしない q >");
        scanf("%lc%*c", &c);
        //printf("%d\n", d.enemy[0]);
        if(c == 'e'){
            e -= 5;
            if (e > 0){
                printf("少しだけ、傷を負わせた\n");
                move2Prison();
                printf("「痛い！離して！」...抵抗はむなしく牢屋に戻された。\n");
                f = 1;
            }
        }
    }else if(ch.weapon == GUN || ch.weapon == FULL){
        printf("こちらに気が付いていない。盗賊の頭に狙いを定めた。\n");
        printf("引き金を引く e/なにもしない q >");
        scanf("%lc%*c", &c);
        if(c == 'e'){
            e = 0;
            printf("銃弾は脳天を貫き、盗賊は倒れた\n");
            ch.weapon -= 2;
            
            f = 1;
        }
    }

    // 敵の攻撃
    if(e > 0){
        if(ch.hp == 1){
            ch.hp -= 1;
            if(d.chara == 15)   printf("牢に投げられ、頭を強く打った。\n視界がチカチカと点滅し、鼻血が噴き出す。\nうめきながら立とうとするが、脚が震えるばかりで力が入らない。\nニヤニヤ気味の悪い笑みを最後に、意識が途切れた。\n");
            else printf("もう動けそうにない…ここまでみたいだ…\n");
            printf("GAMEOVER\n");
            exit(EXIT_SUCCESS);
        }else if(ch.hp == 5){
            ch.hp -= 4;
            printf("傷が深い…視界がぼんやりしている\n");
        }else if(f == 1)
            ch.hp -= 5;
    }

    if (e <= 0){
        e = 0; //0下回った時の処理
        printf("盗賊は倒れた。手に嫌な感触が残った\n");
    }

    printf("キャラのHP: %d\n",ch.hp);
    return e;
}

void move2Prison(){
    d.chara = 15;
}


