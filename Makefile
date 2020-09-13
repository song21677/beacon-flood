CFLAGS = -g
LDLIBS = -ltins

all: main

main: main.cpp

clean:
	rm -f main
