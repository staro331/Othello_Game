CC		= gcc
CFLAGS		=  
LDFLAGS		=

TARGET 		= othello
SRCS		= main.c othello.c cpualgo.c bitboard.c cpualgo_minimax.c cpualgo_maxstone.c
OBJS		= main.o othello.o cpualgo.o bitboard.o cpualgo_minimax.o cpualgo_maxstone.o

RM		= rm -f

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $<

main.o: othello.h
othello.o: othello.h
cpualgo.o: othello.h bitboard.h
bitboard.o: bitboard.h
cpualgo_minimax.o: othello.h bitboard.h
cpualgo_maxstone.o: othello.h bitboard.h

clean:
	$(RM) $(OBJS)
