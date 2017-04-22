CC = gcc

OUTPUT = tuner

all:
	$(CC) tune_in.c -o $(OUTPUT)
	./$(OUTPUT)
