# THIS FILE IS DESIGNED FOR GNU MAKE

PROG_NAME ?= boun-c-castle
CFLAGS ?= -Wall --pedantic -lm -std=gnu11 -g

objects = src/main.o src/term.o

all: $(PROG_NAME)

$(PROG_NAME): $(objects)
	$(CC) $(objects) -o $(PROG_NAME) $(CFLAGS) 

.PHONY: clean
clean:
	rm -f $(PROG_NAME) $(objects)

FORCE: 
