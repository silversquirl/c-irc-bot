#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dlfcn.h>

const char *server = "195.148.124.79";
const int port = 6667;

#define NICK "vktec-cbot"
#define CHAN "##vktecs-botland"

const char *reg = "NICK " NICK "\r\nUSER " NICK " localhost localhost :" NICK "\r\n";
const char *join = "JOIN " CHAN "\r\n";
const char *msg = "PRIVMSG " CHAN " :hello\r\n";
const char *quit = "QUIT :Bye!\r\n";

void *mod;
void (*hello)(FILE *s, char *sender, char *channel, char *msg);

void load_module(int reload) {
  if (reload) dlclose(mod);

  mod = dlopen("./module.so", RTLD_NOW);
  if (!mod) {
    fprintf(stderr, "dlopen: %s\n", dlerror());
    exit(4);
  }

  hello = dlsym(mod, "hello");
  char *result = dlerror();
  if (result) {
    fprintf(stderr, "dlsym: %s\n", result);
    exit(5);
  }
}

int main() {
  load_module(0);

  struct sockaddr_in addr = { AF_INET, htons(port) };
  if (inet_pton(addr.sin_family, server, &addr.sin_addr) <= 0)
    return 1;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (connect(sock, (struct sockaddr *)&addr, sizeof addr)) {
    perror("connect");
    return 2;
  }

  FILE *s = fdopen(sock, "r+");

  fputs(reg, s);
  fputs(join, s);
  fputs(msg, s);
  char buf[1025];
  while (fgets(buf, sizeof buf, s)) {
    printf("%s", buf);
    char *sender = strtok(buf, " ");
    if (!strncmp(strtok(NULL, " "), "PRIVMSG", strlen("PRIVMSG"))) {
      char *channel = strtok(NULL, " ");
      char *msg = strtok(NULL, "");
      if (*msg == ':') ++msg;
      if (!strncmp(msg, "!hello", strlen("!hello"))) {
        hello(s, sender, channel, msg);
      } else if (!strncmp(msg, "!reload", strlen("!reload"))) {
        load_module(1);
        fprintf(s, "PRIVMSG %s :Reloaded\r\n", channel);
      } else if (!strncmp(msg, "!quit", strlen("!quit"))) {
        break;
      }
    }
  }
  fputs(quit, s);
  fclose(s);
  dlclose(mod);
  return 0;
}
