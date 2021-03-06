#!/bin/make

# Shared-object library code for libftw.so

# Default install is to user home

HOME_DIR:=$$HOME
INCLUDE_DIR:=$(HOME_DIR)/usr/include
LIBRARY_DIR:=$(HOME_DIR)/usr/lib
INSTALL_DIR:=$(HOME_DIR)/usr/bin

CC:=icc
CFLAGS:= -O2 -Wall -fPIC

# The header files here contain definitions of ftw-kernel codes
# And the c files are their implementation
# The master copies of the header files are kept HERE, and installed with 
# 'make install-headers'
#
# The ftw_types.h is maintained here, as well.
# All library routines here are prefixed with ftw_

libftw.so: ftw_types.h ftw_rng.o ftw_prng.o ftw_std.o ftw_param.o ftw_rng2.o ftw_cav_parser.o ftw_config_parser.o
	$(CC) $(CFLAGS) -shared -o libftw.so *.o 

ftw_rng.o:	ftw_rng.c ftw_rng.h ftw_types.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c ftw_rng.c

ftw_rng2.o:	ftw_rng2.c ftw_rng2.h ftw_types.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c ftw_rng2.c

ftw_prng.o:	ftw_prng.c ftw_prng.h 
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c ftw_prng.c

ftw_std.o:	ftw_std.c ftw_std.h ftw_types.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c ftw_std.c

ftw_param.o:	ftw_param.c ftw_param.h ftw_types.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c ftw_param.c

ftw_config_parser.o:	ftw_config_parser.c ftw_config_parser.h ftw_types.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c ftw_config_parser.c

ftw_cav_parser.o:	ftw_cav_parser.c ftw_cav_parser.h ftw_types.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c ftw_cav_parser.c

# ftw_command_line_parser is deprecated in favor of ftw_param (as of March 24, 2011...)
# Originally written as command_line_parser, I'm leaving it in the build tree until I clean up usage.
# Any instances won't be able to find the original name, but it's the same API

install:	install-headers install-lib

install-lib:    libftw.so 
	mv libftw.so $(LIBRARY_DIR)

clean:
	rm -f *.o libftw.so

install-headers: ftw_types.h ftw_command_line_parser.h ftw_rng.h ftw_rng2.h ftw_prng.h ftw_std.h ftw_param.h ftw_config_parser.h ftw_cav_parser.h 
	cp ftw_types.h ftw_command_line_parser.h ftw_rng.h ftw_rng2.h ftw_prng.h ftw_std.h ftw_param.h ftw_config_parser.h ftw_cav_parser.h $(INCLUDE_DIR)

test:	test.c
	$(CC) $(CFLAGS) -lm -lftw -pthread -I$(INCLUDE_DIR) -L$(LIBRARY_DIR) test.c
