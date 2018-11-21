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

char ** parse_args(char* line){
  char** ary = calloc(20, sizeof(char*));
  int i;
  char * k;
  char * m = " ";
  int ctr = 0;
  for(i = 0; i < 20; i++){
    k = strsep(&line, " ");
    ary[i] = k;
  }
  return ary;
}

int main(){
  char** line;
  char command[256];
  while(1){
    printf("Enter command: ");
    scanf("%[^\n]", command);
    getchar();
    line = parse_args(command);
    if(strcmp(line[0], "exit") == 0){
      return 0;
    }
    int f = fork();
    if(!f){
      execvp(line[0], line);
      return 0;
    }else{
      int status;
      wait(&status);
    }
    line = NULL;
    strcpy(command, "");
  }
}
