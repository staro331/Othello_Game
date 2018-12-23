#include "othello.h"
#include "bitboard.h"

int eval(BitBoard, BitBoard, int);
int evalresult(BitBoard, BitBoard, int);
int maxlevel(int, BitBoard, BitBoard, int, int);
int minlevel(int, BitBoard, BitBoard, int, int);
int getLineScore(BitBoard, BitBoard, int);

BitBoard board2bitboard(tBoard* pboard, int turn);

#define INF 60000

int linescore[8] = {
	0,		//000
	10, 	//001
	-50,	//010
	-50,	//011
	100,	//100
	100,	//101
	100,	//110
	100,	//111
};

tPos cpu_minimax(tBoard* pboard, tPlaceList* placelist)
{
	putchar('\n');
	
	int limit = 6;
	if ((pboard->blacknum + pboard->whitenum) > (64 - 6)) {
		limit = 64 - (pboard->blacknum + pboard->whitenum);
	}


	tPlaceList* p;
	tPlaceList* tmp = NULL;
	int score, scoremax = -INF;
	BitBoard white = board2bitboard(pboard, WHITE);
	BitBoard black = board2bitboard(pboard, BLACK);
	BitBoard m, rev;
	for (p = placelist->next; p != NULL; p = p->next) {
		m = cr2bitboard(p->pos.x, p->pos.y);
		rev = getRevPat(white, black, pboard->turn, m);
		if (pboard->turn == WHITE) 
			score=minlevel(limit-1, white ^ (m | rev), black ^ rev, WHITE, BLACK);
		else if (pboard->turn == BLACK) 
			score=minlevel(limit-1, white ^ rev, black ^ (m | rev), BLACK, WHITE);
		
		/*
		if (pboard->turn == WHITE) {
			printf("white: %016llx ", white ^ (m | rev));
			printf("black: %016llx ", black ^ rev);
		} else {
			printf("white: %016llx ", white ^ rev);
			printf("black: %016llx ", black ^ (m | rev));
		}
		
		printf("white: %016llx ", white);
		printf("black: %016llx ", black);
		*/

		//printf("%c%c score: %d\n", 'a' + p->pos.x, '1' + p->pos.y, score);
		

		if (score > scoremax) {
			scoremax = score;
			tmp = p;
		}
	}
	
	fflush(stdout);
	//sleep(5);
	
	assert(tmp != NULL);
	return tmp->pos;
}

BitBoard board2bitboard(tBoard* pboard, int turn)
{
	int i, j;
	BitBoard b = 0;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (pboard->board[i][j] == turn)
				b |= cr2bitboard(j, i);
		}
	}
	return b;
}

int maxlevel(int limit, BitBoard white, BitBoard black, int myturn, int turn) 
{
	int flag = 0;
	int score, scoremax = -INF;
	BitBoard m, rev;
	//printf("white: %016llx, black: %016llx, turn: %s\n",
	//		white, black, (turn == WHITE) ? "white" : "black");
	if (limit == 0)
		return eval(white, black, myturn);
	
	for (m = 0x8000000000000000; m != 0; m = m >> 1) {
		if ((m & (white | black)) != 0) continue;
		if ((rev=getRevPat(white, black, turn, m)) == 0)
			continue;
		flag = 1;
		
		if (turn == WHITE) 
			score=minlevel(limit-1, white ^ (m | rev), black ^ rev, myturn, BLACK);
		else if (turn == BLACK) 
			score=minlevel(limit-1, white ^ rev, black ^ (m | rev), myturn, WHITE);

		if (score > scoremax) 
			scoremax = score;
	}
	if (flag == 0) {
		for (m = 0x8000000000000000; m != 0; m = m >> 1) {
			if ((m & (white | black)) != 0) continue;
			if ((rev=getRevPat(white, black, reverse(turn),m))==0)
				continue;
			flag = 1;
			break;
		}
		if (flag == 1)	
			return minlevel(limit-1, white, black, myturn, reverse(turn));
		else if (flag == 0)
			return evalresult(white, black, myturn);
	}
	return scoremax;
}

int minlevel(int limit, BitBoard white, BitBoard black, int myturn, int turn)
{
	int flag = 0;
	int score, scoremin = INF;
	BitBoard m, rev;
	//printf("white: %016llx, black: %016llx, turn: %s\n",
	//		white, black, (turn == WHITE) ? "white" : "black");

	if (limit == 0)
		return eval(white, black, myturn);

	for (m = 0x8000000000000000; m != 0; m = m >> 1) {
		if ((m & (white | black)) != 0) continue;
		if ((rev = getRevPat(white, black, turn, m)) == 0)
			continue;
		flag = 1;

		if (turn == WHITE)
			score=maxlevel(limit-1, white ^ (m | rev), black ^ rev, myturn, BLACK);
		else if (turn == BLACK)
			score=maxlevel(limit-1, white ^ rev, black ^ (m | rev), myturn, WHITE);
		
		if (score < scoremin)
			scoremin = score;
	}
	if (flag == 0) {
		for (m = 0x8000000000000000; m != 0; m = m >> 1) {
			if ((m & (white | black)) != 0) continue;
			if ((rev=getRevPat(white, black, reverse(turn),m))==0)
				continue;
			flag = 1;
			break;
		}
		if (flag == 1)	
			return maxlevel(limit-1, white, black, myturn, reverse(turn));
		else if (flag == 0)
			return evalresult(white, black, myturn);
	}

	return scoremin;
}

int evalresult(BitBoard white, BitBoard black, int turn)
{
	if (getnum(turn) > getnum(reverse(turn)))
		return INF;
	else
		return -INF;
}

int eval(BitBoard white, BitBoard black, int turn)
{
	int placeScore = getplacenum(white, black, turn) - getplacenum(white, black, reverse(turn));
	int lineScore = getLineScore(white, black, turn) - getLineScore(white, black, reverse(turn));
	return 100 * placeScore + 3 * lineScore;
}

int getLineScore(BitBoard white, BitBoard black, int turn)
{
	int score = 0;
	BitBoard b = (turn == WHITE) ? white : black;
	score += linescore[(b & 0xe000000000000000) >> 61];	
	score += linescore[((b & 0x0000200000000000) >> 45) | 
					   ((b & 0x0040000000000000) >> 53) |
					   ((b & 0x8000000000000000) >> 61)];
	score += linescore[((b & 0x0000800000000000) >> 47) |
				   	   ((b & 0x0080000000000000) >> 54) |
					   ((b & 0x8000000000000000) >> 61)];

	score += linescore[(b & 0x00000000000000e0) >> 5];
	score += linescore[((b & 0x0000000000200000) >> 21) |
					   ((b & 0x0000000000004000) >> 13) |
					   ((b & 0x0000000000000080) >> 5)];
	score += linescore[((b & 0x0000000000800000) >> 23) |
					   ((b & 0x0000000000008000) >> 14) |
					   ((b & 0x0000000000000080) >> 5)];

	score += linescore[((b & 0x0400000000000000) >> 58) |
					   ((b & 0x0200000000000000) >> 56) |
					   ((b & 0x0100000000000000) >> 54)];
	score += linescore[((b & 0x0000040000000000) >> 42) |
				  	   ((b & 0x0002000000000000) >> 48) |
					   ((b & 0x0100000000000000) >> 54)];
	score += linescore[((b & 0x0000010000000000) >> 40) |
					   ((b & 0x0001000000000000) >> 47) |
					   ((b & 0x0100000000000000) >> 54)];

	score += linescore[((b & 0x0000000000000004) >> 2) |
					   (b & 0x0000000000000002) |
					   ((b & 0x0000000000000001) << 2)];
	score += linescore[((b & 0x0000000000040000) >> 18) |
					   ((b & 0x0000000000000200) >> 8) |
					   ((b & 0x0000000000000001) << 2)];
	score += linescore[((b & 0x0000000000010000) >> 16) |
					   ((b & 0x0000000000000100) >> 7) |
					   ((b & 0x0000000000000001) << 2)];
	return score;
}
