/*****************************************************************************
*Write a program that executes two commands using a pipe*
*The two commands should be entered by the user as arguments enclosed by " " and
separated by |, e.g. ./mypipe "command1 | command2"
*If no arguments are entered by the user, the program will assume command 1 is ls -
l and command 2 is sort.
*The correctness of both commands is totally at the discretion of the user
*
*The program should execute  the commands in pipe and show the output (if any)
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>               /* strsep, etc. */
#define MAX_NUM_ARGS 20           /* Maximum number of arguments allowed */
#define MAX_STR_LEN 200           /* Maximum string length */

void runpipe();

int main(int argc, char * argv[])
{
       int fd[2];                 /* Two ends of the pipe */
       char * lhs = NULL;         /* Left hand side command of the pipe */
       char * rhs = NULL;          /* Right hand side command of the pipe */
       char * lhscommand = "ls -l";  /* Default command name on left hand side of pipe */
       char * rhscommand = "sort"; /* Default command name on right hand side of pipe */
       char * lhsargs[MAX_NUM_ARGS] = { "ls", "-l", NULL };   /* Default LHS args */
       char * rhsargs[MAX_NUM_ARGS] = { "sort", NULL };       /* Default RHS args */
       char *r = NULL;

       /*Parse the user input to extract the commands and their arguments*/
       /*Hint: read about strsep(3) */
       if (argc == 1)
       {
         //execute default commands
         lhs = lhscommand;
         rhs = rhscommand;
       }
       else if (argc == 2)
       {
         r = argv[1];
         //printf("char *r: %s\n", r);
         lhs = strsep(&r, "|");
         rhs = strsep(&r, "|");
       }

       //printf("lhs: %s\n", lhs);
       //printf("rhs: %s\n", rhs);

       int pid, status;

       /* Create the pipe */
       pipe(fd);     /* fd[0] is read end, fd[1] is write end */

       /* Do the forking */
       switch (pid = fork())
       {
               /* The LHS of command 'ls -l|sort' i.e. 'ls' should be
               executed in the child. */
               /*The RHS of command 'ls -l|sort' i.e. 'sort' should be
                executed in the parent. */

       case 0: //child
         dup2(fd[1], 1); //write end of pipe points to stdout of child
         close(fd[0]); //child does not need this end of the pipe
         execl("/bin/sh", "/bin/sh", "-c", lhs, (char *)0);
         exit(0);

       case -1:
         perror("fork error");
         exit(1);

       default: //parent
         dup2(fd[0], 0); //read end of pipe points to stdin of parent
         close(fd[1]); //parent does not need this end of the pipe

         execl("/bin/sh", "/bin/sh", "-c", rhs, (char *)0);
         break;
       }

       exit(0);
}
