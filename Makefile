.POSIX:
.PHONY: all clean

CC := gcc
CFLAGS := -fPIC -Wall -Werror
LDFLAGS := -ldl

all: bot
clean:
	rm -rf *.o *.so bot

bot: main.o module.so
	$(CC) $(LDFLAGS) -o $@ $<

%.so: %.o
	$(CC) $(LDFLAGS) -shared -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<
