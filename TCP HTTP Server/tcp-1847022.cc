/* Milena Zlaticanin - Z1847022
   CSCI 330-1
   Assignment 8
   Due 11/20/20
*/

// Example of running the server side: ./a.out 9003 ~/
// Example of running the client side: telnet localhost 9003

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <stdlib.h>
#include <sys/uio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>

using std::cout;
using std::endl;
using std::cerr;

int main (int argc, char *argv[])
{
  if (argc < 2) {
    cout << "Wrong arguments! Correct: 'port number' and 'root directory'" << "\n";
    exit(EXIT_FAILURE);
  }

  int newSock, sock, received; 	
  unsigned int serverlen, clientlen; // lenght for server and client address

  // Creating address structures
  struct sockaddr_in echoserver;
  struct sockaddr_in echoclient;
	
  DIR *dirp; 
  struct dirent *dirE;
  
  if ((dirp = opendir(argv[2])) == 0) { 
    perror(argv[2]); 
    exit(EXIT_FAILURE);
  }
  closedir(dirp);

  // Creating TCP socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Failed to create socket");
    exit (EXIT_FAILURE);
  }

  // Construct the server sockaddr_in structure
  memset(&echoserver, 0, sizeof(echoserver)); // Clear struct  
  echoserver.sin_family = AF_INET;	      // Internet IP
  echoserver.sin_addr.s_addr = INADDR_ANY;    // Any IP address
  echoserver.sin_port = htons(atoi(argv[1])); // Server port

  // Bind socket to handle incoming messages at given port
  serverlen = sizeof(echoserver);
  if (bind(sock, (struct sockaddr *) &echoserver, serverlen) < 0) {
    perror ("Failed to bind server socket");
    exit(EXIT_FAILURE);
  }

  if (listen(sock, 64) < 0) {
    perror("listen");
    exit(EXIT_FAILURE); 
  }

  // Run until canceled
  while(newSock = accept(sock,(struct sockaddr*)&echoclient, &clientlen)) 
    { 
      if (fork()) { // parent	    
	close(newSock);	// keep the connection open
      }
      else // child
	{      
	  cout << "Client connected: " << inet_ntoa(echoclient.sin_addr) << "\n";
			
	  if (dup2(newSock,1) < 0) { 
	    perror("socket1 failed"); 
	    exit(EXIT_FAILURE);
	  }
	  
	  if (dup2(newSock,2) < 0) { 
	    perror("socket2 failed"); 
	    exit(EXIT_FAILURE);
	  }				
			

	  while(true)
	    {
	      int success = 1;
	      int cnt = 0;
	      char buffer[1024], path[1024], readPath[1024];

	      // Reading from the client
	      if ((received = read(newSock, buffer, 1024)) < 0) {
		perror ("read failed");
		exit (EXIT_FAILURE);
	      }
	      buffer[received] = '\0';	// terminating client message

	      // Separating from the client
	      while(buffer[cnt + 5] != '\0')
		{
		  path[cnt] = buffer[cnt + 5];
		  cnt++;
		}
	      
	      // Checking for substring
	      for (int j = 0; j < cnt; j++)
		{
		  if (path[j] == '.')
		    {
		      if (path[j+1] == '.') {
			cout << "Error! It may not contain the substring!" << "\n";
			success = 0;
			break;
		      }
		    }
		}					

	      // Checking for the user input (i.e. GET /)
	      if (!strncmp(buffer,"GET /",5) && success == 1) 
		{	
				
		  if (isalpha(buffer[5]) || isdigit(buffer[5]) || buffer[5] == '_' || buffer[5] == '-') 
		    {
		      strcpy(readPath, argv[2]);
		      path[cnt - 1] = '\0';
		      strcat(readPath, path);
		      		      
		      // Removing the trailing newline
		      int newline = strlen(readPath);
		      readPath[newline - 1] = '\0';
		      cout << "Read path value: " << readPath << "\n";
		      cout << endl;

		      // Opening directory
		      if ((dirp = opendir(readPath)) == 0) { 
			if (errno == 20)
			  execlp("less","less", readPath,(char*)(NULL));
			else
			  {
			    perror(readPath); 
			    exit(EXIT_FAILURE);
			  }
		      }

		      // Searching for "index.html"
		      while((dirE = readdir(dirp)) != NULL)
			{
			  if (!strcmp(dirE->d_name, "index.html"))	
			    execlp("less","less", "index.html",(char*)(NULL));
			}
		      closedir(dirp);

		      // Opening directory
		      if ((dirp = opendir(readPath)) == 0) { 
			perror(readPath); 
			exit(EXIT_FAILURE);}
						
		      while((dirE = readdir(dirp)) != NULL)
			{							
			  if (write(newSock,dirE->d_name,strlen(dirE->d_name)) != (unsigned int)strlen(dirE->d_name))
			    {
			      perror("Error");
			      exit(EXIT_FAILURE);
			    }
			  cout << "\n";
			}						
		      closedir(dirp);	
		    } 
										
		  else if (success == 1)
		    {
		      cout << "Root is here!" << "\n";;
		      if ((dirp = opendir(argv[2])) == 0) { 
			perror(argv[2]); 
			exit(EXIT_FAILURE);}
					
		      while((dirE = readdir(dirp)) != NULL) 
			{
			  if (!strcmp(dirE->d_name, "index.html"))
			    {
			      execlp("less","less","index.html",(char*)(NULL)); 
			    }
			}
		      closedir(dirp);
		      
		      if ((dirp = opendir(argv[2])) == 0) { 
			perror(argv[2]); 
			exit(EXIT_FAILURE);
		      }
						
		      while((dirE = readdir(dirp)) != NULL)
			{
			  if (strncmp(dirE->d_name, ".", 1))
			    {
			      if (write(newSock, dirE->d_name, strlen(dirE->d_name)) != (unsigned int)strlen(dirE->d_name))
				{
				  perror("Error!");
				  exit(EXIT_FAILURE);
				}
			      cout << "\n";
			    }
			}
		    } 	
		} // END OF GET
	      else if (success != 0) 
		{
		  if (strncmp(buffer,"GET /",5)) 
		    cout << "Wrong use of the GET command! Use: 'GET /'" << "\n"; 
		}
	      cout << "\n" << "Enter another 'GET /'" << "\n"; 
	    } // END OF WHILE TRUE LOOP
	} // END OF THE CHILD
      close(newSock);
	} // END OF ACCEPT LOOP	

return 0;
} // END OF MAIN
