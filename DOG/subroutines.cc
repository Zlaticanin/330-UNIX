/* Milena Zlaticanin - Z1847022
   CSCI 330-1
   Assignment 4
   Due 10/02/20
*/

#include "subroutines.h"

void ceaserCipher(char* buffer , const size_t buffsize, const int ceaserShift)
{
  for(size_t i=0; i<buffsize; i++)
 {
   if(isupper(buffer[i])) // if buffer is uppercase letter apply shift
     buffer[i] = char(int(buffer[i] + ceaserShift - 65) % 26 + 65);
     
   else if (islower(buffer[i])) // if buffer is lowercase letter apply shift
     buffer[i] = char(int(buffer[i] + ceaserShift - 97) % 26 + 97);
 }
}

void rotation(char* buffer, const size_t buffsize, const int rotateCount)
{
  for(size_t i=0; i < buffsize; i++)
    {
      buffer[i] = (buffer[i] + rotateCount) % 256; // applying to each one-byte value interpreted as 0-255 
    }
}

void hexaOut(const char* buffer, const size_t buffsize, char* hexabuff)
{  
  char cnvrt[] = "0123456789abcdef"; // array to convert number 0-15 to a character '0' to 'F'
 
    for(size_t i=0; i < buffsize; i++)
    {
      int rightNibble = buffer[i] & 0x0F; //using bitwise & to mask off the right nibble (half a byte) with 0x0F
      int leftNibble = (buffer[i] & 0xF0) >> 4; //using bitwise & to mask off the left nibble (half a byte) with 0xF0 and then shift it 4 places
        
        hexabuff[(2 * i)] = cnvrt[leftNibble];
        hexabuff[(2 * i + 1)] = cnvrt[rightNibble];
    }
    
}

void binOut(const char* buffer, const size_t buffsize, char* binbuff)
{
  int mask[]= {128,64,32,16,8,4,2,1}; // array for masks for all 8 bits
  
  for(size_t i=0; i < buffsize; i++)
    {
      for(int j = 0, k = 7; j < 8; j++, k--)
	{
	  binbuff[(8 * i) + j] = ((buffer[i] & mask[j]) >> k) + 48; // using ascii table for 0 and adding it to the binbuff
	}
    }
}
