#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

char ** parse_args_semicolon(char* line){
  char** ary = calloc(20, sizeof(char**));
  int i = 0;
  char * k;
  while((k = strsep(&line, ";"))){
    ary[i] = k;
    i++;
  }
  return ary;
}

char ** parse_args_space(char* line){
  char** ary = calloc(20, sizeof(char*));
  int i = 0;
  char * k;
  while((k = strsep(&line, " "))){
    if(strcmp(k, "")){
      ary[i] = k;
      i++;
    }
  }
  return ary;
}

int main(){
  char** input;
  char** line;
  char command[256];
  char dir[256];
  while(1){
    printf("%s$ ", getcwd(dir, sizeof(dir)));
    scanf("%[^\n]", command);
    getchar();
    input = parse_args_semicolon(command);
    for(int num = 0; num < 20; num++){
      if(input[num]){
	line = parse_args_space(input[num]);
	if(strcmp(line[0], "exit") == 0){
	  return 0;
	}else if(strcmp(line[0], "cd") == 0){
	  if(chdir(line[1]) == -1){
	    printf("Error: %s\n", strerror(errno));
	  }
	}else{
	  int f = fork();
	  if(!f){
	    if(execvp(line[0], line) == -1){
	      printf("Error: %s\n", strerror(errno));
	    }
	    return 0;
	  }else{
	    int status;
	    wait(&status);
	  }
	}
      }
    }    
    strcpy(command, "");
  }
}
