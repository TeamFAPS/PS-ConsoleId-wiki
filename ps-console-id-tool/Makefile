CC = gcc
CFLAGS = -Wall -g
TARGET = ps-console-id-tool
OBJS = main.o

ifeq ($(DEBUG), 1)
CFLAGS+=-g -O0
else
CFLAGS+=-O2
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -L -lz -llibz

clean:
	$(RM) *.o $(TARGET) *.exe *.exe.stackdump
