all: a.out b.out
CC=g++
CPPFLAGS= -Ofast -std=c++11 -march=native -Wall -Wextra

b.out: minlex.cpp
	$(CC) $(CPPFLAGS) $< -o $@

a.out: bit.cpp
	$(CC) $(CPPFLAGS) $< -o $@

clean:
	rm -f a.out b.out
