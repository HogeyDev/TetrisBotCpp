CC := g++
CCARGS := -Wall -Werror -Wpedantic -g

.PHONY: clean
all: clean compile run

compile:
	$(CC) src/*.cpp -o build/main -I./src/include $(CCARGS)

run:
	./build/main

bear:
	bear -- make

clean:
	rm -rf build
	mkdir build

debug: compile
	gdb --args ./build/main