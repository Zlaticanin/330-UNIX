/* Milena Zlaticanin - Z1847022
   CSCI 330-1
   Assignment 5
   Due 10/23/20
*/

#include <iostream>
#include <unistd.h> // for fork()
// for wait()
#include <sys/types.h>
#include <sys/wait.h>

#include <cstring>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


int main()
{
  // commands 1 and 2 that are going to be passes on cmd line
  char command1[255];
  char command2[255];

  // chars for command1 and 2 options
  char *cmd1Option[10];
  char *cmd2Option[10];

  char quit[] = "quit"; // for the while loop, so the user can quit
  
  int count = 0; // for the strtok loop
  int wstat; // for the waitpid status

  // Taking the user's input for command1
  cout << "command1? ";
  cin.getline(command1, 255);

  while(strcmp(command1, quit) != 0)
    {
      cout << "command2? ";
      cin.getline(command2, 255);

      if(strcmp(command2, quit) == 0) // in case the user wants to quit on command2
	break;

      // Breaking down each part of the command and putting it in the right place
      count = 0;
      cmd1Option[count++] = strtok(command1, " "); // returning the first piece of command1
      while((cmd1Option[count] = strtok(NULL, " ")) != 0) // getting all the peices of the first command
	count++;

      count = 0; // resetting the count
      cmd2Option[count++] = strtok(command2, " "); // returning the first piece of command2
      while((cmd2Option[count] = strtok(NULL, " ")) != 0) // getting all the pieces of the second command
	count++;

      // Pipe
      int pipefd[2]; // 0-read, 1-write, 2-error

      if(pipe(pipefd) == -1)
	{
	  perror("Error! Can't create the pipe!");
	  exit(-1);
	}

      // Fork
      int pidChild1 = fork(); // creating fork

      if(pidChild1 < 0) // checking for errors for Child1
	{
	  perror("Error!");
	  exit(-1);
	}

      if(pidChild1 == 0)
	{
	  close(pipefd[0]); // closing the read end
	  dup2(pipefd[1], 1); // dup2 is copying from the standard output into 1
	  close(pipefd[1]); // closing the write end

	  // Replacing the child process with execvp
	  int err1Child = execvp(cmd1Option[0], cmd1Option);

	  if(err1Child < 0)
	    {
	      perror("exec for child1 failed!");
	      exit(-1);
	    }

	}

      waitpid(-1, &wstat, 0); // from man page for waitpid

      int pidChild2 = fork(); // creating a new fork for the parent process

      if(pidChild2 == 0)
	{
	  close(pipefd[1]); // closing the write end
	  dup2(pipefd[0],0); // dup2 is copying from the standard input into 0
	  close(pipefd[0]); // closing the read end

	  // Replacing the current child process with execvp
	  int err2Child = execvp(cmd2Option[0], cmd2Option);

	  if(err2Child < 0)
	    {
	      perror("exec for child2 failed!");
	      exit(-1);
	    }
	}

      // Closing functions after only the parent is left
      close(pipefd[0]); // closing the read end of parent
      close(pipefd[1]); // closing the write end of parent
      waitpid(-1, &wstat, 0); // from man page for waitpid


      // After everything is closed, ask the user for more commands or to quit the program
      cout << "command1? ";
      cin.getline(command1, 255);
    }
    
    return 0;
}    
    
    
