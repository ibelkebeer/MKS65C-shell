#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

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
  char command[256];\
  char dir[256];
  while(1){
    printf("%s$ ", getcwd(dir, sizeof(dir)));
    scanf("%[^\n]", command);
    getchar();
    line = parse_args(command);
    if(strcmp(line[0], "exit") == 0){
      return 0;
    }else if(strcmp(line[0], "cd") == 0){
      if(chdir(line[1]) == -1){
	printf("Error: %s\n", strerror(errno));
      }
    }else{
      int f = fork();
      if(!f){
	char** cur = calloc(20, sizeof(char));
	int i;
	int temp;
	for(i = 0; i < 20; i++){
	  if(strcmp(line[i], ";") == 0){
	    temp = fork();
	    if(!f){
	      if(execvp(cur[0], cur) == -1){
		printf("Error: %s\n", strerror(errno));
	      }
	    }else{
	      int status;
	      wait(&status);
	    }
	  }else if(!(line[i])){
	    if(execvp(cur[0], cur) == -1){
	      printf("Error: %s\n", strerror(errno));
	    }
	  }else{
	    cur[i] = line[i];
	  }
	  return 0;
	}
      }else{
	int status;
	wait(&status);
      }
    }
    line = NULL;
    strcpy(command, "");
  }
}
