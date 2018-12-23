#ifndef OTHELLO
#define OTHELLO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#define BOARD_SIZE 8

#define CPU_MIN 0
#define CPU_MAX 1
#define CPU_CALCULATE_TIME 3

#define OUTBOARD -1
#define BLANK 0
#define WHITE 1
#define BLACK 2

#define RIGHT 0
#define RIGHTDOWN 1
#define DOWN 2
#define LEFTDOWN 3
#define LEFT 4
#define LEFTUP 5
#define UP 6
#define RIGHTUP 7

#define VS_PLAYER 0
#define VS_CPU 1

typedef struct pos {
	int x;
	int y;
} tPos;

typedef struct board {
	int turn;
	int blacknum;
	int whitenum;
	int board[BOARD_SIZE][BOARD_SIZE];
	struct board *bp, *fp;
} tBoard;

typedef struct placeList {
	tPos pos;
	struct placeList *next;
} tPlaceList;

//move 1 pos into direction
int move(tPos* pos, int direction);

//getboard OUTBOARD BLACK WHITE BLANK
int getBoard(tBoard*, tPos); 

//just place 'turn' into board[y][x]
int jplace(tBoard*, int x, int y , int turn); 

//place 'turn' into board[y][x] and turn sandwitched stone
int place(tBoard*, int x, int y, int turn);

//return 1 if it can return stone 0 if it can't
int canplace(tBoard*, int, int, int);

//set the placelist with the position that can turn the stone and return the size
int getlist(tBoard*, tPlaceList*, int); 
int getlistnum(tPlaceList*);
int islisted(tPos, tPlaceList*);

//free list
void freelist(tPlaceList*);

void insertBoard(tBoard*, tBoard*);
void freeBoard(tBoard*);

tPos othello_cpu(tBoard*, tPlaceList*, int);
tPos cpu_random(tPlaceList*);
tPos cpu_minimax(tBoard*, tPlaceList*);

int reverse(int);

#endif 
