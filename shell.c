#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

static void sighandler(int signo){
}

int main(){
  printf("Enter command: ");
  char command[256];
  fgets(command, 256, fp);
  return 0;
}
