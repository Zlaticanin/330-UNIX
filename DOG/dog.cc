/* Milena Zlaticanin - Z1847022
   CSCI 330-1
   Assignment 4
   Due 10/02/20
*/

#include "subroutines.h"
#include <string.h>
#include <iostream>
// Headers for OPEN
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> // Header for READ

#define DEFAULTBUFFSIZE 256 // Macro for the Default Buffer Size

using namespace std;

int main(int argc, char *argv[]){ // Entry point
 
  int fd; // declaring the file descriptor
  int opt; // declaring opt for the while loop

  // Setting up flags for new functions
  bool ceaserCipherflag = false; 
  int ceaserCipherShift = 0;
  bool rotationflag = false;
  int rotationShift = 0;
  bool hexaOutflag = false;
  bool binOutflag = false;
  
  size_t buffsize = DEFAULTBUFFSIZE; // setting the buffsize to be the default buffer size
  size_t maxRead = 0; // maximum number of bytes read
  
  while( (opt = getopt(argc, argv, "s:n:c:r:xb")) != -1){
    switch(opt){
    case 's': // to change the size of the buffer used for calls to read and write to be x bytes
      if(atoi(optarg) > 0)
	buffsize = atoi(optarg);
      else {
	cerr << "Error! Buffer size must be larger than 0" << endl;
        return 1;
      }
      break;

    case 'n': // to change the number of bytes to read from each file to x
      if(atoi(optarg) >= 0)
	maxRead = atoi(optarg);
      else {
	cerr << "Error! Negative numbers are not allowed for -n option" << endl;
	return 1;
      }
      break;

    case 'c': // to apply a Ceaser cipher
      ceaserCipherflag = true;
      ceaserCipherShift = atoi(optarg);
      break;

    case 'r': // to apply a rotation
      rotationflag = true;
      rotationShift = atoi(optarg);
      break;

    case 'x': // to output data as hexadecimal
      hexaOutflag = true;
      break;

    case 'b': // to output data in binary notation
      binOutflag = true;
      break;
 
    default:
      abort();
    }
  }
  //If both -r and -c are specified print an error message
  //Both options can't be specified because the behavior can change depending on which is executed first
  if(ceaserCipherflag && rotationflag){
    cerr << "Error! Can't specify both -r -c" << endl;
    exit(1);
  }

  //If both -x and -b are specified print an error message
  //Both options can't be specified because binary and hexadecimal flags are mutually exclusive - not possible to represent data as both simultaneously
  if(hexaOutflag && binOutflag){
    cerr << "Error! Can't specify both -x -b" << endl;
    exit(1);
  }

  //setting new buffers for the functions
  char *buffer = new char[buffsize];
  char *hexabuff = new char[2*buffsize]; //multiplying the buffsize for hexabuff because it is based on a power of 2
  char *binarybuff = new char[8*buffsize]; //multiplying the buffsize for binary because output data will be 8 times longer than input data

  
  if(argc == 1) {
    // If there is an error, print out the message
    perror("Not enough arguments");
    return 1;
  }
  for(int i=optind; i<argc; i++){ 
    if(argv[i][0] == '-' && argv[i][1] == 0) // If "-" is specified as one of the files to read and output, read the data from standard input
      fd = 0;
    else
      fd = open(argv[i], O_RDONLY, 0644); // parameters and flags.
    // If there is an error, print out what it is
    if(fd == -1) {
      char errorMessage[270] = "Error opening ";
      strcat(errorMessage, argv[i]);
      perror(errorMessage); // printing and error for a specific file that was not found
      return 2; // exit
    }
    
    ssize_t nr; // variable for number successfully read
    size_t bytesRem = maxRead; //setting the bytes remaining to the number of maxium bytes read
    size_t totalRead = 0; //initializing total number of bytes read to 0
    size_t nextBytes = buffsize;

    if(bytesRem < buffsize)
      nextBytes = bytesRem;
    else
      nextBytes = buffsize;
    
    while ((nr = read(fd, buffer, nextBytes)) > 0 && (bytesRem > 0 || maxRead == 0)){
      totalRead += nr;
      bytesRem = maxRead - totalRead;

      if(bytesRem < buffsize)
	nextBytes = bytesRem;
      else
	nextBytes = buffsize;

      //Calling new functions
      //Ceaser Cipher
      if(ceaserCipherflag)
	ceaserCipher(buffer, nr, ceaserCipherShift);

      //Rotation
      if(rotationflag)
	rotation(buffer, nr, rotationShift); 
      
      //Hexa Output
      if(hexaOutflag)
	{
	  hexaOut(buffer, nr, hexabuff);
	  write(1, hexabuff, nr * 2);
	}
      //Bin Output
      else if(binOutflag)
	{
	  binOut(buffer, nr, binarybuff);
	  write(1, binarybuff, nr * 8);
	}
      else
        { 
	  write(1, buffer, nr); // Write to a file
        }
    }
    if(fd != 0)
      close(fd); // Closing the file descriptor
  }
  //deleting buffers
  delete[] buffer;
  delete[] hexabuff;
  delete[] binarybuff;
  
  return 0;

}
