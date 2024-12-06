#makefile for MytreeTest.c
#camden Mills (millscam@pdx.edu)


CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g


TARGET = mytree_test.exe


SRCS = Mytree.c MytreeTest.c iom361_r2.c float_rndm.c
OBJS = $(SRCS:.c=.o)
HEADERS = Mytree.h iom361_r2.h float_rndm.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

