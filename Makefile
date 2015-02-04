CC = gcc
SRC = Source/Main.c
TARGET = output
OBJS = $(addsuffix .o,$(basename $(wildcard $(SRC))))
LIBPATH = lib
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -g3 -Iinclude -c
LDFLAGS = `pkg-config --libs gtk+-3.0`
HEADERS = $(wildcard include/*)
LIBS = -lMain -lArchive -lbz2 -lfno

all: $(TARGET)

$(TARGET) : $(OBJS) $(HEADERS)
	$(CC) $(LDFLAGS) -o $@ $< -L$(LIBPATH) $(LIBS)

$(OBJS) : $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(OBJS)
	
clean :
	rm -f $(TARGET) $(OBJS)