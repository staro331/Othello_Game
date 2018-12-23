#include "othello.h"

typedef struct direction {
	int flag0;
	int flag1;
} tDirection;

int getBoard(tBoard* pboard, tPos pos)
{
	if (pos.x < 0 || pos.x > BOARD_SIZE - 1 || pos.y < 0 || pos.y > BOARD_SIZE -1)
		return OUTBOARD;
	return pboard->board[pos.y][pos.x];
}

int move(tPos* pos, int direction)
{
	switch (direction) {
	case RIGHT:
		pos->x += 1;
		break;
	case RIGHTDOWN:
		pos->x += 1; pos->y += 1;
		break;
	case DOWN:
		pos->y += 1;
		break;
	case LEFTDOWN:
		pos->x -= 1; pos->y += 1;
		break;
	case LEFT:
		pos->x -= 1;
		break;
	case LEFTUP:
		pos->x -= 1; pos->y -= 1;
		break;
	case UP:
		pos->y -= 1;
		break;
	case RIGHTUP:
		pos->x += 1; pos->y -= 1;
		break;
	}
	return 0;
}

int jplace(tBoard* pboard, int x, int y, int turn)
{
	pboard->board[y][x] = turn;
	pboard->whitenum = 0;
	pboard->blacknum = 0;
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			switch (pboard->board[i][j]) {
			case WHITE:
				pboard->whitenum++;
				break;
			case BLACK:
				pboard->blacknum++;
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

int place(tBoard* pboard, int x, int y, int turn)
{
	int i;
	tPos pos;
	tDirection direction[8];	
	jplace(pboard, x, y, turn);
	for (i = 0; i < 8; i++) {
		direction[i].flag0 = direction[i].flag1 = 0;
		pos.x = x;
		pos.y = y;
		while (1) {
			move(&pos, i);
			int piece = getBoard(pboard, pos);
			if (piece == OUTBOARD) break;
			if (piece == BLANK) break;
			if (piece != turn) {
				direction[i].flag0 = 1;
				continue;
			}
			if (piece == turn) {
				direction[i].flag1 = (direction[i].flag0) ? 1 : 0;
				break;
			}
		}

		if (!direction[i].flag1) continue;
		pos.x = x;
		pos.y = y;
		while (1) {
			move(&pos, i);
			int piece = getBoard(pboard, pos);
			if (piece == OUTBOARD) break;
			if (piece == BLANK) break;
			if (piece == turn) break;
			if (piece != turn) {
				jplace(pboard, pos.x, pos.y, turn);
				continue;
			}
		}
	}
}

int canplace(tBoard* pboard, int x, int y, int turn)
{
	int i;
	int flag0 = 0;
	int flag1 = 0;
	tPos pos;
	pos.x = x; pos.y = y;
	if (getBoard(pboard, pos) != BLANK) return 0;

	for (i = 0; i < 8; i++) {
		flag0 = 0;
		pos.x = x;
		pos.y = y;
		while (1) {
			move(&pos, i);
			int piece = getBoard(pboard, pos);
			if (piece == OUTBOARD) break;
			if (piece == BLANK) break;
			if (piece != turn) {
				flag0 = 1;
				continue;
			}
			if (piece == turn) {
				flag1 = flag0 ? 1 : 0;
				break;
			}
		}
		if (flag1) break;
	}
	return flag1;
}

int getlist(tBoard* pboard, tPlaceList* placelist, int turn)
{
	int count = 0;
	int i, j;
	freelist(placelist);
	tPlaceList *bp = placelist;
	tPlaceList *p;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (canplace(pboard, i, j, turn)) {
				if ((p = (tPlaceList*)malloc(sizeof(tPlaceList))) == NULL) {
					perror("malloc");
					exit(1);
				}
				p->pos.x = i;
				p->pos.y = j;
				p->next = NULL;
				bp->next = p;
				bp = p;				
				count++;
			}
		}
	}
	return count;
}

int getlistnum(tPlaceList* placelist)
{
	int num = 0;
	tPlaceList* p;
	for (p = placelist->next; p != NULL; p = p->next)
		num++;

	return num;
}

int islisted(tPos pos, tPlaceList* placelist)
{
	tPlaceList *p;
	for (p = placelist->next; p != NULL; p = p->next) {
		if (p->pos.x == pos.x && p->pos.y == pos.y)
			return 1;
	}
	return 0;
}

void freelist(tPlaceList* placelist)
{
	while (placelist->next != NULL) {
		tPlaceList* tmp = placelist->next;
		placelist->next = tmp->next;
		free(tmp);
	}
}

void insertBoard(tBoard* board, tBoard* board_next)
{
	board_next->bp = board->bp;
	board_next->fp = board;
	board_next->bp->fp = board_next;
	board_next->fp->bp = board_next;
}

void freeBoard(tBoard* pboard)
{
	tBoard* tmp;
	tmp = pboard;
	pboard->bp->fp = pboard->fp;
	pboard->fp->bp = pboard->bp;
	free(pboard);
}

int reverse(int turn) {
	return (turn == BLACK) ? WHITE : BLACK;
}
