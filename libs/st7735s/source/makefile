# MIT License
# Copyright (c) 2021, Michal Kozakiewicz, github.com/michal037
CODE_REPOSITORY = https://github.com/michal037/driver-ST7735S
CODE_VERSION = 3.0.0
#
# This file contains sample build rules for this project. It includes additional
# descriptions and references.

################################################################################
# GCC command options ##########################################################
################################################################################

#====
# standard: ANSI C standard (X3.159-1989)
# 1. https://stackoverflow.com/a/17209532
# 2. https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html
# 3. https://gcc.gnu.org/onlinedocs/gcc/Standards.html#Standards
# note: -ansi 'In C mode, this is equivalent to -std=c90. In C++ mode, it is
#   equivalent to -std=c++98.'
#
# flags: -ansi -pedantic-errors
#====

#====
# warnings and errors
# 1. https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
#
# flags: -Wall -Wextra -Werror
#====

#====
# Optimize code
# 1. https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
#
# flags: -O2
#====

#====
# strict aliasing rule
# 1. https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8
# 2. https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
#
# flags: -fstrict-aliasing
#====

#====
# position independent code for library
# 1. https://stackoverflow.com/a/5311538
# 2. https://gcc.gnu.org/onlinedocs/gcc/Code-Gen-Options.html
#
# flags: -fPIC
#====

################################################################################
# END GCC command options END ##################################################
################################################################################

################################################################################
# makefile notes ###############################################################
################################################################################

# 1. https://www.gnu.org/software/make/manual/make.html
# 2. https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# 3. https://stackoverflow.com/a/5542170
# 4. https://www.gnu.org/software/make/manual/make.html
#	   #Variables-Used-by-Implicit-Rules

################################################################################
# END makefile notes END #######################################################
################################################################################

# program for compiling C programs
CC = gcc

# extra flags to give to the C compiler
CFLAGS  = -ansi -pedantic-errors
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -O2
CFLAGS += -fstrict-aliasing
CFLAGS += -fPIC

# https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
# A phony target is one that is not really the name of a file; rather it is just
# a name for a recipe to be executed when you make an explicit request.
.PHONY: help clean example_wpi_test

help:
	@echo Makefile for the ST7735S library.
	@echo code repository: $(CODE_REPOSITORY)
	@echo code version: $(CODE_VERSION)
	@echo

	@echo make help
	@echo "  print this help and exit"

	@echo make clean
	@echo "  remove all generated files"

	@echo make example_wpi
	@echo "  build the example program (wpi)"

	@echo make example_wpi_test
	@echo "  clean, build and run the example program (wpi)"

st7735s.o: st7735s.c st7735s.h
	$(CC) $(CFLAGS) -c -o st7735s.o st7735s.c

main_example_wpi.o: main_example_wpi.c st7735s.h
	$(CC) $(CFLAGS) -c -o main_example_wpi.o main_example_wpi.c

# The result of this target is a program called 'example_wpi'.
# For a custom Wiring Pi library location, read the Wiring Pi reference.
# Wiring Pi reference: http://wiringpi.com/reference/
example_wpi: st7735s.o main_example_wpi.o
	$(CC) $(CFLAGS) -lwiringPi -lm -o example_wpi main_example_wpi.o st7735s.o

# clean, build and run routine
example_wpi_test: clean example_wpi
	./example_wpi

# remove all generated files
clean:
	rm -f st7735s.o main_example_wpi.o example_wpi
