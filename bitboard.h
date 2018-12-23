#ifndef BITBOARD
#define BITBOARD

#include "othello.h"

typedef unsigned long long BitBoard;

BitBoard cr2bitboard(int row, int col);
BitBoard getRevPat(BitBoard, BitBoard, int, BitBoard);
int getnumall(BitBoard, BitBoard);
int getnum(BitBoard);
int getplacenum(BitBoard, BitBoard, int);

tPos bitboard2cr(BitBoard b);

#endif
