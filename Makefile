CC = g++

CFLAGS = -lao -ldl -lm

TARGET = test_gen

all: $(TARGET) example

$(TARGET): $(TARGET).cpp wfassem.h wftypes.h
	$(CC) $(TARGET).cpp -o $(TARGET) $(CFLAGS)

example: ao_example.c
	$(CC) ao_example.c -o ao_example $(CFLAGS)