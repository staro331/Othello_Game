#include "othello.h"

tPos cpu_level2(tBoard*, tPlaceList*, int);

tPos othello_cpu(tBoard* pboard, tPlaceList* placelist, int cpu_level) {
	tPos pos;
	srand((unsigned)time(NULL));

	switch (cpu_level) {
	case 0:
		pos = cpu_random(placelist);
		break;
	case 1:
		pos = cpu_minimax(pboard, placelist, 6);
		break;
	case 2:
		pos = cpu_level2(pboard, placelist, 8);
		break;
	default:
		fprintf(stderr, "unvalid cpu_level\n");
		exit(1);
	}
	return pos;
}

//random
tPos cpu_random(tPlaceList* placelist) {
	int num = rand() % getlistnum(placelist);
	tPlaceList* p = placelist;
	int i;
	for (i = 0; i < num + 1; i++) 
		p = p->next;

	return p->pos;
}

tPos cpu_level2(tBoard* pboard, tPlaceList* placelist, int limit)
{
	int boardnum = pboard->blacknum + pboard->whitenum;
	int rest = 64 - boardnum;
	if(rest > 12) {
		return cpu_minimax(pboard, placelist, limit);
	} else {
		return cpu_maxstone(pboard, placelist, 12);
	}
}
