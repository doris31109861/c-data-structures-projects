/*
 * dino.c — 終端機版 Chrome 小恐龍遊戲（Windows：使用 <windows.h> 與 <conio.h>）
 *
 * 遊戲迴圈每格更新畫面陣列：3 層樓可上下跳、障礙物出現在第二或第三層、
 * 分數越高速度越快、記錄最高分，並有「蟲洞」技能可瞬間穿越。
 */
//function 1 : 3 floor
//function 2 : initgame startgame gameover
//function 3 : store the hightest score
//function 4 : obstcles in second or third floor
//function 5 : score up speed up
//function 6 : skill_wormhole
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#define CANVAS 50

void display(char floor[],int lowerlimit,int upperlimit);
void shiftleft(char floor[] , int upperlimit);
void insertBarrier(char *floor,int n);
void initgame();
void startgame();
void gameover();
void wormhole();
int isdead(int curfloor , int playerpos ,char floor1[]);
int i,score,playerPos,currentFloor,runtime,diff;
int highscore= 0;
char floor1[CANVAS],floor2[CANVAS],floor3[CANVAS]; 

int main(){
	while(1){
		initgame();
    	startgame();
	}
    return 0;
}

void display(char floor[],int lowerlimit,int upperlimit){
	int j = 0;
    for(j=lowerlimit;j<upperlimit;j++){
        printf("%c",floor[j]);
    }
}
void shiftleft(char floor[] , int upperlimit){
	int i;
	for(i = 0 ; i <upperlimit-1 ; i++){
		floor[i] = floor[i+1];
	}
	if(floor == floor1){
		floor[upperlimit-1] = '_';
	}else{
		floor[upperlimit-1] = ' ';
	}
	
}
void insertBarrier(char *floor,int n){
	int r = rand()%n;
	floor[CANVAS-1-r]='X';
}
int isdead(int curfloor , int playerpos ,char floor1[]){
	if(floor1[playerpos] == 'X'){
		if(curfloor == 1){
			return 0;	
		}else{
			return 1;	
		}
	}
	if(floor2[playerpos] == 'X'){
		if(curfloor == 2){
			return 0;	
		}else{
			return 1;	
		}
	}
	if(floor3[playerpos] == 'X'){
		if(curfloor == 3){
			return 0;	
		}else{
			return 1;	
		}
	}
	return 1;
}
void initgame(){
    score = 0;
    playerPos=6;//dino
    currentFloor=1;//初始的樓層
    runtime=0;  
    diff = 150;
	for(i=0;i<CANVAS;i++){
        floor1[i]='_';
        floor2[i]=' ';
        floor3[i]=' ';
    }
    floor1[CANVAS-1] = 'X';
    while( kbhit() != 1 ){
    	printf("\n\n");
    	printf("\t               __\n");
		printf("\t              / _\)\n");
		printf("\t     _.----._/ /\n");
		printf("\t    /         /    _\n");
		printf("\t __/ (  | (  |    | \\ .  _  _\n");
		printf("\t/__.-'|_|--|_|    |_/ | | ||_|\n");
    	printf("\n\n\tpress any botton to start the dino\n");
    	Sleep(diff); // restart
    	system("cls"); // clear screen
	}
}
void startgame(){
	while(isdead(currentFloor , playerPos ,floor1)){
    	score++;
    	if(score%50 == 1 && diff > 30){
    		diff = diff - 30;
		}
    	if(kbhit()){
    		if(getch() == ' '){
    			currentFloor ++;
    			runtime=4;
			}
		}
		if(runtime>0){
			runtime--;
		}
		if(runtime==0){
			currentFloor = 1;
		}
		if(currentFloor == 1){
			printf("\n\n\n\t");
			display(floor3,0,CANVAS); // sky
			printf("\n\t");
		    display(floor2,0,CANVAS); // sky
		    printf("\n\t");
		    display(floor1,0,playerPos);
		    printf("B");
		    display(floor1,playerPos+1,CANVAS); // floor
		}else if(currentFloor == 2){
			printf("\n\n\n\t");
			display(floor3,0,CANVAS); // sky
			printf("\n\t");
			display(floor2,0,playerPos); // sky
			printf("B");
			display(floor2,playerPos+1,CANVAS); // floor
			printf("\n\t");
			display(floor1,0,CANVAS);
		}else{
			printf("\n\n\n\t");
			display(floor3,0,playerPos); // sky
			printf("B");
			display(floor3,playerPos+1,CANVAS); // floor
			printf("\n\t");
			display(floor2,0,CANVAS); // sky
			printf("\n\t");
			display(floor1,0,CANVAS);
		}
		printf("\n\n\tSCORE: %d",score);
		printf("\n");
		shiftleft(floor1,CANVAS);
		shiftleft(floor2,CANVAS);
		shiftleft(floor3,CANVAS);
		Sleep(diff); // restart
		system("cls"); // clear screen
		if(score%10 == 0){
			insertBarrier(floor1,10);
		}
		if(score%20 == 0){
			insertBarrier(floor2,20);
		}
		if(score%25 == 0){
			insertBarrier(floor3,30);
		}
		if(score > highscore){
			highscore = score;
		}
	}
	gameover();
	return ; // 此函式回傳 void，不能 return 0
}
void gameover(){
	system("cls"); // clear screen
	printf("\n\n\n");
	printf("\t  #####    ####    ### ###  #####     ####    ##  ##   #####    #####\n");
	printf("\t ##       ##  ##   #######  ##       ##  ##   ##  ##   ##       ##  ##\n");	
	printf("\t ## ###   ######   ## # ##  ####     ##  ##   ##  ##   ####     #####\n");
	printf("\t ##  ##   ##  ##   ## # ##  ##       ##  ##    ####    ##       ## ##\n");
	printf("\t  ####    ##  ##   ##   ##  #####     ####      ##     #####    ##  ##\n");
	printf("\n\t\tYour last score is: %d\t\tYour highest score is: %d\n\n",score,highscore);
	printf("\t\t\t\t\t\t\t   Press 'R' to restart");
	while(getch()!= 'r'){	}
	system("cls"); // clear screen
	Sleep(1000); // Windows API：毫秒為單位（原本的 sleep 不在 windows.h 中）
}

