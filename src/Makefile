#########################################
# Makefile for Genetic Salesman Problem #
#########################################
CC=g++ -std=c++0x
CFLAGS= -O0 -g -Wall -Wextra
LDFLAGS=
SRC=$(wildcard *.cpp)
OBJDIR=obj
OBJ=$(SRC:.cpp=.o)
BINARY=TSP

all: $(BINARY) $(OBJ)
.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$@

$(BINARY): $(OBJ)
	$(CC) $(CFLAGS) $(addprefix $(OBJDIR)/,$(OBJ)) -o  $@

clean: 
	rm -rfv $(BINARY) $(OBJ)
