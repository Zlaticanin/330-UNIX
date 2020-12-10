/* Milena Zlaticanin - Z1847022
   CSCI 330-1
   Assignment 3
   Due 09/18/20
*/

#include <iostream>
// Headers for OPEN
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> // Header for READ
#define BUFFSIZE 256 // Macro for the Buffer Size

using namespace std;

int main(int argc, char *argv[]){ // Entry point
  char buffer[BUFFSIZE]; // declaring buffer for the read command
  int fd; // declaring the file descriptor
  
  if(argc == 1) {
    // If there is an error, print out the message
    perror("Not enough arguments");
    return 1;
  }
  for(int i=1; i<argc; i++){ 
    if(argv[i][0] == '-' && argv[i][1] == 0) // If "-" is specified as one of the files to read and output, read the data from standard input
      fd = 0;
    else
      fd = open(argv[i], O_RDONLY, 0644); // parameters and flags.
    // If there is an error, print out what it is
    if(fd == -1) {
      perror("open");
      return 2; // exit
    }
    
    ssize_t nr; // variable for number successfully read
    while ((nr = read(fd, buffer, BUFFSIZE)) > 0){
      write(1, buffer, nr); // Write to a file
    }
    if(fd != 0)
      close(fd); // Closing the file descriptor
  }
  return 0;

}
