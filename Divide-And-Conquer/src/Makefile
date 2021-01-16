#
# Copyright ©2018 Gaohong Liu.  All rights reserved.
# Name: Gaohong Liu
# Student#: 1561226
# Email: gaohol@uw.edu
#
# This Makefile compiles everything
#

CXX = g++
CFLAGS = -g -Wall -std=c++11
EXE = dc
OBJS = dc.o
# this is the default rule that fires if a user
# just types "make" in the same directory as this Makefile.
all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ 

%.o: %.cc 
	$(CXX) $(CFLAGS) -c $< 

clean: FORCE
	/bin/rm -rf $(EXE) *.o *~ *.dSYM

FORCE:

