.POSIX:
.PHONY: all clean

CFLAGS := -Wall -Werror
LDFLAGS := -ldl

all: bot
clean:
	rm -rf *.o bot

bot: main.o module.so
	$(CC) $(LDFLAGS) -o $@ $<

%.so: %.o
	$(CC) $(LDFLAGS) -shared -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<
