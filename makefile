CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g -O0
TARGET = main

SRCS = main.c wms_gen.c data.c
OBJS = $(SRCS:.c=.o)
HEADERS = wms_gen.h data.h

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

