CC = gcc
CFLAGS = -Wall -g
BISON = bison -d
FLEX = flex

SRC = src
BIN = bin
TARGET = $(BIN)/texting

OBJS = $(SRC)/ast.o $(SRC)/lex.yy.o $(SRC)/parser.tab.o $(SRC)/compiler.o

all: $(BIN) $(TARGET)

$(BIN):
mkdir -p $(BIN)

$(TARGET): $(OBJS)
$(CC) -o $@ $^ -lm

$(SRC)/parser.tab.c $(SRC)/parser.tab.h: $(SRC)/parser.y
$(BISON) -o $(SRC)/parser.tab.c $<

$(SRC)/lex.yy.c: $(SRC)/lexer.l
$(FLEX) -o $@ $<

$(SRC)/%.o: $(SRC)/%.c
$(CC) $(CFLAGS) -c $< -o $@

clean:
rm -f $(SRC)/lex.yy.c $(SRC)/parser.tab.c $(SRC)/parser.tab.h
rm -f $(OBJS) $(TARGET)

test: $(TARGET)
@echo "=== Probando ejemplos ==="
@for f in hola variables condicional bucle mientras completo; do \
echo "--- $$f.tg ---"; \
./$(TARGET) ejemplos/$$f.tg ejemplos/$$f.asm; \
if [ -f ejemplos/$$f.asm ]; then \
nasm -f bin ejemplos/$$f.asm -o ejemplos/$$f.bin 2>/dev/null; \
echo "  ✅ $$f.bin generado"; \
fi; \
done

.PHONY: all clean test
