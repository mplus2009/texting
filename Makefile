CC = gcc
CFLAGS = -Wall -g
LEX = flex
YACC = bison -d

SRC_DIR = src
BIN_DIR = bin

OBJS = $(SRC_DIR)/lex.yy.o $(SRC_DIR)/parser.tab.o $(SRC_DIR)/ast.o $(SRC_DIR)/interpreter.o

all: $(BIN_DIR) $(BIN_DIR)/texting

$(BIN_DIR):
mkdir -p $(BIN_DIR)

$(BIN_DIR)/texting: $(OBJS)
$(CC) -o $@ $^ -lm

$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/lexer.l
$(LEX) -o $@ $<

$(SRC_DIR)/parser.tab.c: $(SRC_DIR)/parser.y
$(YACC) -o $@ $<

$(SRC_DIR)/lex.yy.o: $(SRC_DIR)/lex.yy.c $(SRC_DIR)/parser.tab.h
$(CC) $(CFLAGS) -c -o $@ $<

$(SRC_DIR)/parser.tab.o: $(SRC_DIR)/parser.tab.c $(SRC_DIR)/ast.h $(SRC_DIR)/interpreter.h
$(CC) $(CFLAGS) -c -o $@ $<

$(SRC_DIR)/ast.o: $(SRC_DIR)/ast.c $(SRC_DIR)/ast.h
$(CC) $(CFLAGS) -c -o $@ $<

$(SRC_DIR)/interpreter.o: $(SRC_DIR)/interpreter.c $(SRC_DIR)/interpreter.h $(SRC_DIR)/ast.h
$(CC) $(CFLAGS) -c -o $@ $<

clean:
rm -f $(SRC_DIR)/lex.yy.c $(SRC_DIR)/parser.tab.c $(SRC_DIR)/parser.tab.h
rm -f $(OBJS)
rm -rf $(BIN_DIR)

test: all
./bin/texting ejemplos/ejemplo_completo.tg

.PHONY: all clean test
