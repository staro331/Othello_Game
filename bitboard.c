#include "bitboard.h"

BitBoard cr2bitboard(int row, int col)
{
	BitBoard b = 0x8000000000000000;
	return b >> (row + col * 8);
}

int getnumall(BitBoard white, BitBoard black)
{
	return getnum(white) + getnum(black);
}

int getnum(BitBoard board)
{
	BitBoard b = board;
	b = (b & 0x5555555555555555) & ((b & 0xaaaaaaaaaaaaaaaa) >> 1);
	b = (b & 0x3333333333333333) & ((b & 0xcccccccccccccccc) >> 2);
	b = (b & 0x0f0f0f0f0f0f0f0f) & ((b & 0xf0f0f0f0f0f0f0f0) >> 4);
	b = (b & 0x00ff00ff00ff00ff) & ((b & 0xff00ff00ff00ff00) >> 8);
	b = (b & 0x0000ffff0000ffff) & ((b & 0xffff0000ffff0000) >> 16);
	b = (b & 0x00000000ffffffff) & ((b & 0xffffffff00000000) >> 32);
	return b;
}

int getplacenum(BitBoard white, BitBoard black, int turn)
{
	//printf("%s place: ", (turn == WHITE) ? "white": "black");
	int count = 0;
	BitBoard b, rev;
	for (b = 0x8000000000000000; b != 0; b = b >> 1) {
		if ((b & (white | black)) != 0) continue;
		if ((rev = getRevPat(white, black, turn, b)) == 0)
			continue;
			//tPos pos = bitboard2cr(b);
			//printf(" %c%c", 'a'+ pos.x, '1' + pos.y);
		count++;
	}
	//putchar('\n');
	return count;
}

//whiteをひっくり返すパターン
BitBoard getRevPat(BitBoard white, BitBoard black, int turn, BitBoard m)
{
	BitBoard rev = 0;
	BitBoard tmp;
	if (turn == WHITE) {
		tmp = white;
		white = black;
		black = tmp;
	}

	//right
	BitBoard wh = white & 0x7e7e7e7e7e7e7e7e;
	BitBoard m2, m3, m4, m5, m6;
	BitBoard m1 = m >> 1;

	if ((m1 & wh) != 0) {
		if (((m2 = m1 >> 1) & wh) == 0) {
			if ((m2 & black) != 0)
				rev |= m1;
		} else if (((m3 = m2 >> 1) & wh) == 0) {
			if ((m3 & black) != 0)
				rev |= m1 |= m2;
		} else if (((m4 = m3 >> 1) & wh) == 0) {
			if ((m4 & black) != 0)
				rev |= m1 |= m2 |= m3;
		} else if (((m5 = m4 >> 1) & wh) == 0) {
			if ((m5 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4;
		} else if (((m6 = m5 >> 1) & wh) == 0) {
			if ((m6 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		} else {
			if (((m6 >> 1) & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		}
	}

	//rightdown
	wh = white & 0x007e7e7e7e7e7e00;
	m1 = m >> 9;

	if ((m1 & wh) != 0) {
		if (((m2 = m1 >> 9) & wh) == 0) {
			if ((m2 & black) != 0)
				rev |= m1;
		} else if (((m3 = m2 >> 9) & wh) == 0) {
			if ((m3 & black) != 0)
				rev |= m1 |= m2;
		} else if (((m4 = m3 >> 9) & wh) == 0) {
			if ((m4 & black) != 0)
				rev |= m1 |= m2 |= m3;
		} else if (((m5 = m4 >> 9) & wh) == 0) {
			if ((m5 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4;
		} else if (((m6 = m5 >> 9) & wh) == 0) {
			if ((m6 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		} else {
			if (((m6 >> 9) & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		}
	}

	//down
	wh = white & 0x00ffffffffffff00;
	m1 = m >> 8;

	if ((m1 & wh) != 0) {
		if (((m2 = m1 >> 8) & wh) == 0) {
			if ((m2 & black) != 0)
				rev |= m1;
		} else if (((m3 = m2 >> 8) & wh) == 0) {
			if ((m3 & black) != 0)
				rev |= m1 |= m2;
		} else if (((m4 = m3 >> 8) & wh) == 0) {
			if ((m4 & black) != 0)
				rev |= m1 |= m2 |= m3;
		} else if (((m5 = m4 >> 8) & wh) == 0) {
			if ((m5 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4;
		} else if (((m6 = m5 >> 8) & wh) == 0) {
			if ((m6 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		} else {
			if (((m6 >> 8) & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		}
	}

	//leftdown
	wh = white & 0x007e7e7e7e7e7e00;
	m1 = m >> 7;

	if ((m1 & wh) != 0) {
		if (((m2 = m1 >> 7) & wh) == 0) {
			if ((m2 & black) != 0)
				rev |= m1;
		} else if (((m3 = m2 >> 7) & wh) == 0) {
			if ((m3 & black) != 0)
				rev |= m1 |= m2;
		} else if (((m4 = m3 >> 7) & wh) == 0) {
			if ((m4 & black) != 0)
				rev |= m1 |= m2 |= m3;
		} else if (((m5 = m4 >> 7) & wh) == 0) {
			if ((m5 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4;
		} else if (((m6 = m5 >> 7) & wh) == 0) {
			if ((m6 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		} else {
			if (((m6 >> 7) & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		}
	}

	//left
	wh = white & 0x7e7e7e7e7e7e7e7e;
	m1 = m << 1;

	if ((m1 & wh) != 0) {
		if (((m2 = m1 << 1) & wh) == 0) {
			if ((m2 & black) != 0)
				rev |= m1;
		} else if (((m3 = m2 << 1) & wh) == 0) {
			if ((m3 & black) != 0)
				rev |= m1 |= m2;
		} else if (((m4 = m3 << 1) & wh) == 0) {
			if ((m4 & black) != 0)
				rev |= m1 |= m2 |= m3;
		} else if (((m5 = m4 << 1) & wh) == 0) {
			if ((m5 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4;
		} else if (((m6 = m5 << 1) & wh) == 0) {
			if ((m6 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		} else {
			if (((m6 << 1) & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		}
	}

	//leftup
	wh = white & 0x007e7e7e7e7e7e00;
	m1 = m << 9;

	if ((m1 & wh) != 0) {
		if (((m2 = m1 << 9) & wh) == 0) {
			if ((m2 & black) != 0)
				rev |= m1;
		} else if (((m3 = m2 << 9) & wh) == 0) {
			if ((m3 & black) != 0)
				rev |= m1 |= m2;
		} else if (((m4 = m3 << 9) & wh) == 0) {
			if ((m4 & black) != 0)
				rev |= m1 |= m2 |= m3;
		} else if (((m5 = m4 << 9) & wh) == 0) {
			if ((m5 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4;
		} else if (((m6 = m5 << 9) & wh) == 0) {
			if ((m6 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		} else {
			if (((m6 << 9) & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		}
	}

	//up
	wh = white & 0x00ffffffffffff00;
	m1 = m << 8;

	if ((m1 & wh) != 0) {
		if (((m2 = m1 << 8) & wh) == 0) {
			if ((m2 & black) != 0)
				rev |= m1;
		} else if (((m3 = m2 << 8) & wh) == 0) {
			if ((m3 & black) != 0)
				rev |= m1 |= m2;
		} else if (((m4 = m3 << 8) & wh) == 0) {
			if ((m4 & black) != 0)
				rev |= m1 |= m2 |= m3;
		} else if (((m5 = m4 << 8) & wh) == 0) {
			if ((m5 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4;
		} else if (((m6 = m5 << 8) & wh) == 0) {
			if ((m6 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		} else {
			if (((m6 << 8) & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		}
	}

	//rightup
	wh = white & 0x007e7e7e7e7e7e00;
	m1 = m << 7;

	if ((m1 & wh) != 0) {
		if (((m2 = m1 << 7) & wh) == 0) {
			if ((m2 & black) != 0)
				rev |= m1;
		} else if (((m3 = m2 << 7) & wh) == 0) {
			if ((m3 & black) != 0)
				rev |= m1 |= m2;
		} else if (((m4 = m3 << 7) & wh) == 0) {
			if ((m4 & black) != 0)
				rev |= m1 |= m2 |= m3;
		} else if (((m5 = m4 << 7) & wh) == 0) {
			if ((m5 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4;
		} else if (((m6 = m5 << 7) & wh) == 0) {
			if ((m6 & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		} else {
			if (((m6 << 7) & black) != 0)
				rev |= m1 |= m2 |= m3 |= m4 |= m5;
		}
	}
	
	return rev;
}

tPos bitboard2cr(BitBoard b) {
	tPos pos;
	int count = 0;
	while (b != 0) {
		b = b >> 1;
		count++;
	}
	pos.x = (64-count) % 8;
	pos.y = (64-count) / 8;
	return pos;
}
