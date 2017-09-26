all: a.out

a.out: minlex.cpp
	g++ -O3 -std=c++11 -march=native $< -o $@

clean:
	rm -f a.out
