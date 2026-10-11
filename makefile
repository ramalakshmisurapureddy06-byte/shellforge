CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude -I.
SRC := $(filter-out builtin.current.backup.c,$(wildcard *.c)) src/history.c

TARGET=shellforge

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -lreadline -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
