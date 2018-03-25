#include <stdio.h>
#include <stdlib.h>

void hello(FILE *s, char *sender, char *channel, char *msg) {
  char *nick;
  sscanf(sender, ":%m[^!]!", &nick);
  fprintf(s, "PRIVMSG %s :Hello %s\r\n", channel, nick);
  free(nick);
}
