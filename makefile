all: minlex
CC=g++
CPPFLAGS= -Ofast -std=c++11 -march=native -Wall -Wextra

minlex: minlex.cpp
	$(CC) $(CPPFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f minlex
