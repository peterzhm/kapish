#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);
int lsh_setenv(char **args);
int lsh_unsetenv(char **args);
int lsh_exit(char **args);

void  INThandler(int sig)
{
printf("parent cannot be terminated\n");
}
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "setenv",
  "unsetenv",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_setenv,
  &lsh_unsetenv,
  &lsh_exit
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int lsh_cd(char **args)
{
  if(args[2]!=NULL){
    printf("error\n"); 
  }else{
  if (args[1] == NULL) {
    printf("cd\n");
    getenv("HOME");
  } else {
    if (chdir(args[1]) != 0) {
      printf("cd\n");
    }
  }
 }
  return 1;
}

int lsh_setenv(char **args){
   //printf("qqqqq\n");
   if(args[3]!=NULL){
     printf("error\n");
   }
   if (args[1] == NULL) {
     printf("error\n");
   } else {
       if(args[2]!=NULL){
         setenv(args[1],args[2],1);
         printf("%s %s %s\n",args[0],args[1],args[2]);
       }else{
       setenv(args[1]," ",1);
       printf("%s %s %s\n",args[0],args[1]," ");
       }
   }
    return 1;
}
int lsh_unsetenv(char **args){
     if (args[1] == NULL||args[2]!=NULL) {
        printf("error\n");
     }
     printf("%s %s\n",args[0],args[1]);
     unsetenv(args[1]);
     return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int lsh_exit(char **args)
{
  return 0;void  INThandler(int sig)
{
printf("parent cannot be terminated\n");
}
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      printf("error\n");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    printf("error\n");
  } else {
    // Parent process
     signal(SIGINT,SIG_IGN);
     pid_t pid1 = wait(&status);
  }

  return 1;
  }
/**
   @brief Execute shell built-in or launch program.
   @param args Null tevoid  INThandler(int sig)
{
printf("parent cannot be terminated\n");
}rminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}
#define LSH_RL_BUFSIZE 513
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *lsh_read_line(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
    
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void lsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("? ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
  // Load config files, if any.

  // Run command loop.
  FILE *fp;
  fp = fopen(strcat(getenv("HOME"),".kapishrc"),"r");
  lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}