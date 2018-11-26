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
  for(i = 0; i < 20; i++){
    k = strsep(&line, " ");
    ary[i] = k;
  }
  return ary;
}

int main(){
  char** line;
  char command[256];
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
      char** cur = calloc(20, sizeof(char));
      int x = 0;
      for(int i = 0; i < 20; i ++){
	if(!(line[i])){
	  if(strcmp(cur[0], "exit") == 0){
	    return 0;
	  }else if(strcmp(cur[0], "cd") == 0){
	    if(chdir(cur[1]) == -1){
	      printf("Error: %s\n", strerror(errno));
	    }
	  }else{
	    int f = fork();
	    if(!f){
	      if(execvp(cur[0], cur) == -1){
		printf("Error: %s\n", strerror(errno));
	      }
	      return 0;
	    }else{
	      int status;
	      wait(&status);
	    }
	    i = 20;
	    free(cur);
	  }
	}else if(strcmp(line[i], ";") == 0){
	  if(strcmp(cur[0], "exit") == 0){
	    return 0;
	  }else if(strcmp(cur[0], "cd") == 0){
	    if(chdir(cur[1]) == -1){
	      printf("Error: %s\n", strerror(errno));
	    }
	  }else{
	    int f = fork();
	    if(!f){
	      if(execvp(cur[0], cur) == -1){
		printf("Error: %s\n", strerror(errno));
	      }
	      return 0;
	    }else{
	      int status;
	      wait(&status);
	    }
	    free(cur);
	    cur = calloc(20, sizeof(char));
	    x = 0;
	  }
	}else{
	  cur[x] = line[i];
	  x++;
	}
      }
    }
    line = NULL;
    strcpy(command, "");
  }
}
