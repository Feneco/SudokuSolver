#include <stdio.h>
#include <stdlib.h>

#define MAX_INTER 100

typedef enum {false, true} bool; // Bool type
typedef unsigned int uint; // Unsigned int as uint for facility
typedef struct _square // elements struct
{
  /* ELEMENT struct. It have value, possible values and a SET bool*/
  uint value;
  uint possibleValues;
  bool SET;
    
} ELEMENT;

void scanNumbers(ELEMENT ( *element )[9][9]);
void updateScreen(ELEMENT element[][9]);
void printBitNumber(uint value);

void findPossibles(ELEMENT ( *element )[9][9]);
uint lastNumbersLine(ELEMENT element[][9], int line);
uint lastNumbersColumn(ELEMENT element[][9], int column);
uint lastNumbersBlock(ELEMENT element[][9], int X, int Y);

// this guys return 1 if they have done something
int findUniquePossibles(ELEMENT ( *element )[9][9]);
int findUniqueLines(ELEMENT ( *element )[9][9]);
int findUniqueColumns(ELEMENT ( *element )[9][9]);
int findUniqueBlocks(ELEMENT ( *element )[9][9]);

// Returns 1 if solution is not found yet, and 0 if it have
int isSolutionFound(ELEMENT element[][9]);

int isSoloBit(uint value);

int main()
{
  int k;
  ELEMENT element[9][9]; // Main Sudoku declaration

  scanNumbers( &element ); // Get numbers from screen
  findPossibles( &element ); // Find possible values for each non set square

  for(k=0; k<MAX_INTER; k++) // Loop MAX_INTER times
    {    

      while(findUniquePossibles( &element )) // find unique possibles
	{
	  //If it have found uniques, it executes this
	  findPossibles( &element ); // find possibles for next interaction
	}

      while(findUniquesLines( &element ))
	{
	  findPossibles( &element ); // find possibles for next interaction
	}
      
      while(findUniquesColumns( &element ))
	{
	  findPossibles( &element ); // find possibles for next interaction
	}
      
      while(findUniquesBlocks( &element ))
	{
	  findPossibles( &element ); // find possibles for next interaction
	}

      if(isSolutionFound(element)) break; // If true, finish loop
      
    }

  updateScreen(element); // Show results
  if(k>=MAX_INTER) return 1; // Return 1 if it have reached MAX_INTER
#ifdef DEBUG_
  printf("\n number of interactions: %d \n", k); //Print Numbers of interactions
#endif
  return 0;
}


void
scanNumbers( ELEMENT ( *element )[9][9])
{
  /* Scans and stores to pointed variable. If a input is not a valuable
   * number, the function will store 0 on it instead.
   * Usage: (ELEMENT *square[9][9])
   * Scanf 81 times for every square of sudoku.
   */
  
  int i, j;
  char localBuffer;
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{

	  scanf(" %c", &localBuffer);
	  if(localBuffer == 'X')
	    {//Is not set
	      (*element)[i][j].value = 0;
	      (*element)[i][j].possibleValues = ~0;
	      (*element)[i][j].SET = false;
	    }
	  else
	    {//Is set
	      (*element)[i][j].value = localBuffer - '0';
	      (*element)[i][j].possibleValues = 0;
	      (*element)[i][j].SET = true;
	    }
	  
	}
    }
}


void
updateScreen(ELEMENT element[][9])
{
  
  /* print results to the screen
   * usage: void updateScreen(ELEMENT element[][9]);
   */
    
  int i, j, k;
  for(i=0; i<9; i++)
    {
      for(j=0; j<9; j++)
	{

	  if(j%3 == 0 && j != 0) printf("| ");
	  printf("%d", element[i][j].value);
	  if(j != 8) printf(" ");
	  
	}
      
      printf("\n");

#ifndef DEBUG_ // If it is NOT a debug compile
      if(i==8) return;
#endif 
#ifdef DEBUG_ // If it IS a debug compile
      if(i==8) break;
#endif
	
      if((i+1)%3 == 0)
	{	  
	  for(k=0; k<21; k++)
	    {	
	      if(k%2 == 0) printf("-");
	      else printf(" ");
	    }
	}	
      printf("\n");
    }
  
  printf("\n");
  
#ifdef DEBUG_ // If is a debug compile, run this code as well
  printf("\n");
  
  for(i=0; i<9; i++)
    {
      for(j=0; j<9; j++)
	{

	  if(j%3 == 0 && j != 0) printf("| ");

	  if(element[i][j].SET == true)
	    {
	      printf("%d", element[i][j].value);
	    }
	  else
	    {
	      printBitNumber(element[i][j].possibleValues);
	    }
	  if(j != 8) printf(" ");
	  
	}

      printf("\n");
      if(i==8) return;
      if((i+1)%3 == 0)
	{	  
	  for(k=0; k<40; k++)
	    {	
	      if(k%2 == 0) printf("-");
	      else printf(" ");
	    }
	}
      printf("\n");
      
    }
  
  printf("\n");
#endif
  
}		    

void
printBitNumber(uint value)
{
  int i;
  printf("(");
  for(i=0; i<9; i++)
    {
      if((value & (1<<i)))
	printf("%d", i+1);
    }
  printf(")");
}

void
findPossibles(ELEMENT ( *element )[9][9])
{
  /* Returns the possible values for all non-set elements
   */
  int i, j,
    X, Y;
  uint lasting;

  // Scaning possible values on lines
  for(i=0; i<9; i++)
    {
      lasting = lastNumbersLine((*element), i);

      for(j=0; j<9; j++)
	{
	  if((*element)[i][j].SET == false)
	    (*element)[i][j].possibleValues &= lasting;
	}
    }
  // End on finding on lines
  
  // Scaning possible values on columns
  for(i=0; i<9; i++)
    {
      lasting = lastNumbersColumn((*element), i);

      for(j=0; j<9; j++)
	{
	  if((*element)[j][i].SET == false)
	    (*element)[j][i].possibleValues &= lasting;
	}
    }
  // End on finding on columns
  
  int startX, endX,
    startY, endY;
  
  // Scaning possible values on blocks
  for(i=0; i<3; i++)
    {
      for(j=0; j<3; j++)
	{
	  lasting = lastNumbersBlock((*element), i, j);
	  
	  startX = 3 * i;
	  endX = startX + 3;

	  startY = 3 * j;
	  endY = startY + 3;
	  
	  for(X=startX; X<endX; X++)
	    {
	      for(Y=startY; Y<endY; Y++)
		{
		  if((*element)[X][Y].SET == false)
		    (*element)[X][Y].possibleValues &= lasting;		  
		}
	    }

	}
    }
  //End of finding on blocks
  
}


uint
lastNumbersLine(ELEMENT element[][9], int line)
{
  uint lasting;
  lasting = ~0;

  int j;
  for(j=0; j<9; j++)
    {
      if(element[line][j].SET == false)
	lasting &= ~(1 << (element[line][j].value - 1));
    }
  
  return lasting;
}

uint
lastNumbersColumn(ELEMENT element[][9], int column)
{
  uint lasting;
  lasting = ~0;

  int i;
  for(i=0; i<9; i++)
    {
      if(element[i][column].SET == false)
	lasting &= ~(1 << (element[i][column].value - 1));
    }
  
  return lasting;
}

uint
lastNumbersBlock(ELEMENT element[][9], int X, int Y)
{
  
  uint lasting;
  lasting = ~0;

  int i, j;
  int startX, endX,
    startY, endY;

  startX = X * 3;
  endX = startX + 3;

  startY = Y * 3;
  endY = startY + 3;

  for(i=startX; i<endX; i++)
    {
      for(j=startY; j<endY; j++)
	{
	  if(element[i][j].SET == false)
	    lasting &= ~(1 << (element[i][j].value - 1));	  
	}
    }
  
  return lasting;
}


int
findUniquePossibles(ELEMENT (*element)[9][9])
{
  
  int i, j;
  int change;
  changed = 0;
  
  for(i=0; i<9; i++)
    {
      for(j=0; j<9; j++)
	{
	  if((*element)[i][j].SET == false && \
	     isSoloBit((*element)[i][j].possibleValues & 0x1ff) == 0)
	    {
	      (*element)[i][j].value = (*element)[i][j].possibleValues;
	      (*element)[i][j].possibleValues = 0;
	      (*element)[i][j].SET = true;
	      changed = 1;
	    }
	}
    }

  return changed;
}


int findUniqueLines(ELEMENT ( *element )[9][9])
{

  int i, j;
  uint 
}


int
isSolutionFound(ELEMENT element[][9])
{
  /* if have not found solution yet, return 1, else return 0 */
  int i, j;
  
  for(i=0; i<9; i++)
    {
      for(j=0; j<9; j++)
	{
	  if(element[i][j].SET == false) return 1;
	}
    }
  return 0;
}


int
isSoloBit(uint b)
{
  /* Returns 0, if have just one bit set, and != 0 if there is more than one, 
   * or no bit set
   */
  return b && !(b &(b-1));
}
