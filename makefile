CC= gcc
# flags de compilação
CFLAGS= -Wall -std=c11 -O3 -g
# gerar variaveis
SRCS=main.c
# nome do executavel
TARGET= palavras

# opção 'make' geral
all: $(CC) -Wall -o $(TARGET) main.c

clean:
	rm $(TARGET)

