CC = g++
CPPFLAGS = -g -Wall -Wextra -Werror -O2 -std=c++20 -Isrc/include -march=native
CPPFILES = $(shell find src -type f -name '*.cpp')
CPPOBJS = $(patsubst src/%,obj/%,$(CPPFILES:.cpp=.o))

OUT = compiler 

.PHONY: all clean

bin/$(OUT): $(CPPOBJS)
	mkdir -p "$$(dirname $@)"
	$(CC) $(CPPFLAGS) $(CPPOBJS) -o $@

obj/%.o: src/%.cpp
	mkdir -p "$$(dirname $@)"
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf obj bin

