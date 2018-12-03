# MKS65C-shell

# Features
Executes commands using fork  
Supports multiple commands seperated by ;  
Supports redirecting using >, >>, and <  
Supports piping using |  

# Attempted
Using pipe and continuing program  
Redirecting using <<  

# Bugs
Piping causes program to terminate  

# Function Headers
'''  
Takes a command line prompt as an input  
Seperates line at ; and places each command in a char* ary  
returns char* ary  

char ** parse_args_semicolon(char* line)  

========================================  

Takes a command as an input  
Seperates command at spaces and places each delimiter in a char* ary   
returns char* ary  

char ** parse_args_space(char* line)  

========================================  

Takes a command including > or >> as an input  
Seperates command at > or >>  
Replace stdout with file specified and creates or appends to that file the output of the command  
nothing returned  

void redirect_out(char** line, int mode)  

========================================  

Takes a command including < as input  
Seperates command at <  
Replaces stdin with file specified and executes command  
nothing returned  

void redirect_in(char** line)  

========================================

Takes command with | as input  
Seperates command before and after | into 2 different char* arys  
Pipes, executes command before | and writes to fds[1], executes command after | using fds[0] as input  
nothing returned, terminates shell  

void redirect_pipe(char** line)  
'''
