CC = gcc
CFLAGS = -Wall -Wextra  -ggdb
FLAGS = $(shell pkg-config --cflags --libs libcurl)
LDFLAGS = $(shell pkg-config --libs libcurl)

SRC =  tests/cJSON.c tests/main.c src/term_sdk.c
HEADERS = src/term_sdk.h

TARGET = a.out

compile: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(FLAGS) $(SRC) -o $(TARGET)  $(LDFLAGS)

run: $(SRC) $(TARGET)
	@echo "Executing..." $(TARGET)
	@$(shell cat .env) ./$(TARGET)

gf:
	@$(shell cat .env) gf2 ./$(TARGET)


py:
	python -i ./respone_run.py


.PHONY: clean
clean:
	rm -f $(TARGET)