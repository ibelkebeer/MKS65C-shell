#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

/*
Takes a command line prompt as an input
Seperates line at ; and places each command in a char* ary
returns char* ary
 */
char ** parse_args_semicolon(char* line){
  char** ary = calloc(256, sizeof(char**));
  int i = 0;
  char * k;
  while((k = strsep(&line, ";"))){
    ary[i] = k;
    i++;
  }
  return ary;
}

/*
Takes a command as an input
Seperates command at spaces and places each delimiter in a char* ary
returns char* ary
 */
char ** parse_args_space(char* line){
  char** ary = calloc(256, sizeof(char*));
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

/*
Takes a command including > or >> as an input
Seperates command at > or >>
Replace stdout with file specified and creates or appends to that file the output of the command
nothing returned
 */
void redirect_out(char** line, int mode){
  char** command = calloc(256, sizeof(char*));
  char* output = calloc(256, sizeof(char));
  int i = 0;
  while(i < 20){
    if(line[i]){
      if(strcmp(line[i], ">") && strcmp(line[i], ">>")){
	command[i] = line[i];
      }else{
	i++;
	break;
      }
    }
    i++;
  }
  output = line[i];
  i++;
  while(line[i]){
    command[i-2] = line[i];
    i++;
  }
  int y;
  if(mode){
    y = open(output, O_WRONLY | O_APPEND | O_CREAT, 0644);
  }else{
    y = open(output, O_WRONLY | O_CREAT, 0644);
  }
  if(y == -1){
    printf("Error; %s\n", strerror(errno));
  }else{
    int f = fork();
    if(!f){
      dup2(y, 1);
      if(execvp(command[0], command) == -1){
      	printf("Error: %s\n", strerror(errno));
      	execlp("rm", "rm", output, (char*)NULL);
      }
    }else{
      int status;
      wait(&status);
    }
    close(y);
  }
}

/*
Takes a command including < as input
Seperates command at <
Replaces stdin with file specified and executes command
nothing returned
 */
void redirect_in(char** line){
  char** command = calloc(256, sizeof(char*));
  char* input = calloc(256, sizeof(char));
  int i = 0;
  while(i < 20){
    if(line[i]){
      if(strcmp(line[i], "<") && strcmp(line[i], "<<")){
	command[i] = line[i];
      }else{
	i++;
	break;
      }
    }
    i++;
  }
  input = line[i];
  i++;
  while(line[i]){
    command[i-2] = line[i];
    i++;
  }
  int y = open(input, O_RDONLY, 0644);
  if(y == -1){
    printf("Error; %s\n", strerror(errno));
  }else{
    int f = fork();
    if(!f){
      dup2(y, 0);
      if(execvp(command[0], command) == -1){
	       printf("Error: %s\n", strerror(errno));
      }
    }else{
      int status;
      wait(&status);
    }
    close(y);
  }
}

/*
Takes command with | as input
Seperates command before and after | into 2 different char* arys
Pipes, executes command before | and writes to fds[1], executes command after | using fds[0] as input
nothing returned, terminates shell
*/
void redirect_pipe(char** line){
  char** command1 = calloc(256, sizeof(char*));
  char** command2 = calloc(256, sizeof(char*));
  int i = 0;
  int index = 0;
  while(i < 256){
    if(line[i]){
      if(strcmp(line[i], "|")){
	       command1[index] = line[i];
         index++;
      }else{
	       i++;
	       break;
      }
    }
    i++;
  }
  index = 0;
  while(i < 256){
    if(line[i]){
      command2[index] = line[i];
      index++;
    }
    i++;
  }
  int fds[2];
  if(pipe(fds) == -1){
    printf("Error: %s\n", strerror(errno));
  }
  int f = fork();
  if(f){
    close(fds[0]);
    dup2(fds[1], 1);
    if(execvp(command1[0], command1) == -1){
	      printf("Error: %s\n", strerror(errno));
    }
  }else{
    close(fds[1]);
    dup2(fds[0], 0);
    wait(NULL);
    f = fork();
    if(f){
      if(execvp(command2[0], command2) == -1){
	       printf("Error: %s\n", strerror(errno));
      }
    }else{
      int status;
      wait(&status);
    }
  }
  close(fds[0]);
}

static void sighandler(int signo){
  if(signo == 2){
    printf("\nProgram exited due to SIGINT");
    exit(0);
  }
}

/*
Infinitely loops until user exits or SIGINT is received
Uses getcwd to get path to current dir, uses scanf to get user input
Seperates input by ;, then seperates each parsed command by space
Checks if command involves redirection, if it does call corresponding redirect function
Else, execute command and loop
 */
int main(){
  signal(SIGINT, sighandler);
  char** input;
  char** line;
  char command[256];
  char dir[256];
  int i;
  int j;
  int run;
  while(1){
    printf("%s$ ", getcwd(dir, sizeof(dir)));
    scanf("%[^\n]", command);
    getchar();
    input = parse_args_semicolon(command);
    for(i = 0; i < 256; i ++){
      if(input[i]){
      	line = parse_args_space(input[i]);
      	if(strcmp(line[0], "exit") == 0){
      	  return 0;
      	}else if(strcmp(line[0], "cd") == 0){
      	  if(chdir(line[1]) == -1){
      	    printf("Error: %s\n", strerror(errno));
      	  }
      	}else{
      	  run = 0;
      	  for(j = 0; j < 256; j ++){
      	    if(line[j]){
      	      if(!(strcmp(line[j],">"))){
            		redirect_out(line, 0);
            		run = 1;
      	      }
      	      if(!(strcmp(line[j],">>"))){
            		redirect_out(line, 1);
            		run = 1;
      	      }
      	      if(!(strcmp(line[j],"<"))){
            		redirect_in(line);
            		run = 1;
      	      }
      	      if(!(strcmp(line[j], "|"))){
            		redirect_pipe(line);
            		run = 1;
      	      }
      	    }
      	  }
      	  if(!run){
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
    }
  }
}
