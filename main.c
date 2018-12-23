#include "othello.h"

/*****			functions			*****/
void Init(tBoard *, tPlaceList *, int*);
void PrintBoard(tBoard*, tPlaceList*, tPos *pos);
tBoard* MakeNextBoard(tBoard*, tPlaceList*, tBoard* board_tmp, tPos* pos, int* turn);
void PrintResult(tBoard*);

char turn_white[12] = "white:";
char turn_black[12] = "black:";

int main(int argc, char *argv[])
{
	assert(argc == 1 || argc == 3);

	tBoard board; //盤面双方向リスト
	tBoard *board_tmp = &board;
	tBoard *board_next;
	int turn; //ターン
	tPlaceList placelist; //おける場所
	char line[5]; //入力
	int mode = (argc == 1) ? VS_PLAYER : VS_CPU; //モード
	int cpu_level; //cpulevel VS_CPU
	int playerturn; //playerturn VS_CPU

	if (mode == VS_CPU) {
		if (strcmp(argv[1], "WHITE") == 0) {
			playerturn = WHITE;
			strcpy(turn_black, "black(CPU):");
		} else if (strcmp(argv[1], "BLACK") == 0) {
			playerturn = BLACK;
			strcpy(turn_white, "white(CPU):");
		} else {
			fprintf(stderr, "usage: ./othello [myturn(WHITE or BLACK)] [CPU_LEVEL]\n");
			exit(1);
		}
		cpu_level = atoi(argv[2]); //cpulevel
		if (cpu_level < CPU_MIN || cpu_level > CPU_MAX) {
			fprintf(stderr, "CPU_LEVEL: %d ～ %d\n", CPU_MIN, CPU_MAX);
			exit(1);
		}
	}
	
	Init(&board, &placelist, &turn);
	system("clear");
	PrintBoard(&board, &placelist, NULL);

	tPos pos;
	while (1) {
		printf("%s ", (turn == WHITE) ? turn_white : turn_black);

		if (mode == VS_PLAYER || turn == playerturn) {
			//player_input
			while (1) {
				memset(&line, 0, 20);
				scanf("%4[^\n]%*[^\n]", line);
				getchar();
				if (strcmp("quit", line) == 0) {
					exit(0);
				}
				if (strcmp("undo", line) == 0) {
					if (board_tmp == &board) {
						printf("%s ", (turn == WHITE) ? turn_white : turn_black);
						continue;
					} else 
						break;
				}
				if (strlen(line) != 2 || !(line[0] >= 'a' && line[0] <= 'a' + BOARD_SIZE - 1) 
					|| !(line[1] >= '1' && line[1] <= '1' + BOARD_SIZE - 1)) {
					printf("%s ", (turn == WHITE) ? turn_white : turn_black);
					continue;
				}
				pos.x = line[0] - 'a';
				pos.y = line[1] - '1';
				if (islisted(pos, &placelist)) break;
				printf("%s ", (turn == WHITE) ? turn_white : turn_black);
			}

			if (strcmp("undo", line) == 0) {
				do {
					tBoard* board_rm = board_tmp;
					board_tmp = board_tmp->bp;	
					freeBoard(board_rm);
					turn = board_tmp->turn;
				} while ((mode == VS_CPU && turn != playerturn) && board_tmp != &board);
				getlist(board_tmp, &placelist, turn);
				system("clear");
				PrintBoard(board_tmp, &placelist, NULL);
				if (placelist.next == NULL) {
					turn = (turn == BLACK) ? WHITE : BLACK;
					getlist(board_tmp, &placelist, turn);
					if (placelist.next == NULL) 
						break;
					else
						printf("skipped\n");
						continue;
				}
				continue;
			}
			
			if ((board_tmp=MakeNextBoard(&board, &placelist, board_tmp, &pos, &turn)) == NULL)
				break;
			else
				continue;	

		} else if (mode == VS_CPU && turn != playerturn) {
			/*
			printf("Calculating"); fflush(stdout); sleep(1);
			int i;
			for (i = 0; i < CPU_CALCULATE_TIME; i++) {
				putchar('.'); fflush(stdout); sleep(1);
			}
			*/
			tPos pos = othello_cpu(board_tmp, &placelist, cpu_level);
			if ((board_tmp=MakeNextBoard(&board, &placelist, board_tmp, &pos, &turn)) == NULL)
				break;
			else
				continue;	
		}
	} 

	PrintResult(board.bp);

	return 0;
}

void Init(tBoard *pboard, tPlaceList *placelist, int* turn)
{
	//boardinit
	pboard->fp = pboard->bp = pboard;
	memset(pboard->board, 0, sizeof(int) * BOARD_SIZE * BOARD_SIZE);
	pboard->blacknum = pboard->whitenum = 0;
	jplace(pboard, BOARD_SIZE / 2 - 1, BOARD_SIZE / 2 - 1, WHITE);
	jplace(pboard, BOARD_SIZE / 2, BOARD_SIZE / 2 - 1, BLACK);
	jplace(pboard, BOARD_SIZE / 2 - 1, BOARD_SIZE / 2, BLACK);
	jplace(pboard, BOARD_SIZE / 2, BOARD_SIZE / 2, WHITE);
	pboard->turn = BLACK;

	//turninit
	*turn = BLACK;

	//placelistinit
	placelist->next = NULL;
	getlist(pboard, placelist, *turn);
}

void PrintBoard(tBoard* pboard, tPlaceList* placelist, tPos* pos)
{
	int i, j;
	printf(" ");
	for (i = 0; i < BOARD_SIZE; i++) {
		printf("%c ", 'a' + i);
	}
	putchar('\n');

	for (i = 0; i < BOARD_SIZE; i++) {
		printf("%d", i + 1);
		for (j = 0; j < BOARD_SIZE; j++) {
			switch (pboard->board[i][j]) {
			case BLANK:
				printf("□");
				break;
			case WHITE:
				printf("●");
				break;
			case BLACK:
				printf("◯");
				break;
			}
		}
		putchar('\n');
	}

	printf("%s %d %s %d\n", turn_white, pboard->whitenum, turn_black, pboard->blacknum);
	if (pos != NULL)
		printf("placed %c%c\n", 'a' + pos->x, '1' + pos->y);
	if (placelist->next != NULL) {
		printf("placelist:");
		tPlaceList *p;
		for (p = placelist->next; p != NULL; p = p->next) {
			printf(" %c%c", 'a' + p->pos.x, '1' + p->pos.y); 
		}
		putchar('\n');
	}
}

tBoard* MakeNextBoard(tBoard* pboard, tPlaceList* placelist, tBoard* board_tmp, tPos* pos, int* turn)
{
	tBoard* board_next;
	if ((board_next = (tBoard*) malloc(sizeof(tBoard))) == NULL) {
		perror("malloc");
		exit(1);
	}
	*board_next = *board_tmp;
	board_next->turn = *turn;
	place(board_next, pos->x, pos->y, *turn);
	insertBoard(pboard, board_next);
	board_tmp = board_next;

	*turn = (*turn == BLACK) ? WHITE : BLACK;
	board_next->turn = *turn;
	getlist(board_tmp, placelist, *turn);
	system("clear");
	PrintBoard(board_tmp, placelist, pos);
	
	if (placelist->next == NULL) {
		*turn = (*turn == BLACK) ? WHITE : BLACK;
		getlist(board_tmp, placelist, *turn);
		if (placelist->next == NULL) 
			return NULL;
		else {
			system("clear");
			PrintBoard(board_tmp, placelist, pos);
			printf("skipped\n");
			board_next->turn = *turn;
			return board_next;
		}
	}
	return board_next;
}

void PrintResult(tBoard* pboard)
{
	if (pboard->blacknum == pboard->whitenum) {
		printf("DRAW\n");
	} else if (pboard->blacknum > pboard->whitenum) {
		printf("BLACK WIN\n");
	} else {
		printf("WHITE WIN\n");
	}
}
