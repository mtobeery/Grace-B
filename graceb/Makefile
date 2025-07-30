CC = gcc
CFLAGS = -Iinclude -Wall -Wextra

SRC = src/main.c src/lexer.c src/parser.c src/ast_print.c
OUT = graceb

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
