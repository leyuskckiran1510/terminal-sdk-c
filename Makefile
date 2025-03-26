CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = tests/main.c src/term_sdk.c
HEADERS = src/term_sdk.h

TARGET = a.out

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)