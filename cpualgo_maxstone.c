#include "othello.h"
#include "bitboard.h"

static int eval(BitBoard, BitBoard, int);
static int evalresult(BitBoard, BitBoard, int);
static int maxlevel(int, BitBoard, BitBoard, int myturn, int turn, int alpha, int beta);
static int minlevel(int, BitBoard, BitBoard, int myturn, int turn, int alpha, int beta);

static BitBoard board2bitboard(tBoard* pboard, int turn);

#define INF 60000
#define VICTORY 50000
#define DEFEAT -50000

tPos cpu_maxstone(tBoard* pboard, tPlaceList* placelist, int limit)
{
	putchar('\n');
	
	int limit_new = limit;
	if ((pboard->blacknum + pboard->whitenum) > (64 - limit_new)) {
		limit_new = 64 - (pboard->blacknum + pboard->whitenum);
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
			score=minlevel(limit_new-1, white ^ (m | rev), black ^ rev, WHITE, BLACK, scoremax, INF);
		else if (pboard->turn == BLACK) 
			score=minlevel(limit_new-1, white ^ rev, black ^ (m | rev), BLACK, WHITE, scoremax, INF);
		
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

static BitBoard board2bitboard(tBoard* pboard, int turn)
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

static int maxlevel(int limit, BitBoard white, BitBoard black, int myturn, int turn, int alpha, int beta) 
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
			score=minlevel(limit-1, white ^ (m | rev), black ^ rev, myturn, BLACK, alpha, beta);
		else if (turn == BLACK) 
			score=minlevel(limit-1, white ^ rev, black ^ (m | rev), myturn, WHITE, alpha, beta);

		if (score >= beta)
			return score;

		if (score > scoremax) {
			scoremax = score;
			alpha = (alpha >= scoremax) ? alpha : scoremax;
		}
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
			return minlevel(limit, white, black, myturn, reverse(turn), alpha, beta);
		else if (flag == 0)
			return evalresult(white, black, myturn);
	}
	return scoremax;
}

static int minlevel(int limit, BitBoard white, BitBoard black, int myturn, int turn, int alpha, int beta)
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
			score=maxlevel(limit-1, white ^ (m | rev), black ^ rev, myturn, BLACK, alpha, beta);
		else if (turn == BLACK)
			score=maxlevel(limit-1, white ^ rev, black ^ (m | rev), myturn, WHITE, alpha, beta);

		if (score <= alpha)
			return score;
		
		if (score < scoremin) {
			scoremin = score;
			beta = (beta <= scoremin) ? beta : scoremin; 
		}
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
			return maxlevel(limit, white, black, myturn, reverse(turn), alpha, beta);
		else if (flag == 0)
			return evalresult(white, black, myturn);
	}

	return scoremin;
}

static int evalresult(BitBoard white, BitBoard black, int turn)
{
	if (turn == WHITE) 
		return getnum(white) - getnum(black);
	else
		return getnum(black) - getnum(white);
}

static int eval(BitBoard white, BitBoard black, int turn)
{
	if (getnumall(white, black) != 64) {
		fprintf(stderr, "error %d\n", getnumall(white, black));
		exit(1);
	}
	evalresult(white, black, turn);
}

