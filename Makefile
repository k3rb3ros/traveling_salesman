#########################################
# Makefile for Genetic Salesman Problem #
#########################################
CC=g++ -std=c++0x
CFLAGS= -O0 -g -Wall -Wextra
LDFLAGS=
SOURCES=tsp.cpp
OBJECTS=$(SOURCES:.cpp=.o)
BINARY=TSP

all: $(BINARY) $(OBJECTS)
.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

clean: 
	rm -rfv $(BINARY) $(OBJECTS)
