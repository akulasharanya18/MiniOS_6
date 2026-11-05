CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -O0 -D_DEFAULT_SOURCE -Iinclude

LDFLAGS = -pthread

TARGET = minios

SRC = src/main.c \
      src/shell.c \
      src/monitor.c \
      src/process.c \
      src/ipc.c \
      src/jobs.c \
      src/workers.c \
      src/sync.c \
      src/queue.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)
