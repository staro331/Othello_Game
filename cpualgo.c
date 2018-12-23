#include "othello.h"

tPos othello_cpu(tBoard* pboard, tPlaceList* placelist, int cpu_level) {
	tPos pos;
	srand((unsigned)time(NULL));

	switch (cpu_level) {
	case 0:
		pos = cpu_random(placelist);
		break;
	case 1:
		pos = cpu_minimax(pboard, placelist);
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

