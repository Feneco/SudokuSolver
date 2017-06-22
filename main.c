#include <stdio.h>
#include <stdlib.h>

#define _BIT0 0b1
#define _BIT1 0b10
#define _BIT2 0b100
#define _BIT3 0b1000
#define _BIT4 0b10000
#define _BIT5 0b100000
#define _BIT6 0b1000000
#define _BIT7 0b10000000
#define _BIT8 0b100000000

typedef enum {false, true} bool;  // Boolean type
typedef unsigned int uint;  // Unsigned int as uint for facility
typedef struct _square  //peep struct
{

    uint value;
    uint possibleValues;

} peep;


uint toogleBit(uint data, uint bit)
{
    return data ^= 1<<bit;
}

uint eraseBit(uint data, uint bit)
{
    return data &= ~(1<<bit);
}

uint writeBit(uint data, uint bit)
{
    return data |= (1<<bit);
}

uint readBit(uint data, uint bit)
{
    if(data &= 1<<bit) return 1;
    else return 0;
}


uint lastNumbersLine(peep square[9][9], uint line)
{
    uint lasting = ~0;
    int i;
    for(i=0; i<9; i++)
    {
        lasting = eraseBit(lasting, square[line][i].value - 1);
    }

    return lasting;
}


uint lastNumbersColumn(peep square[9][9], uint column)
{
    uint lasting = ~0;
    int i;
    for(i=0; i<9; i++)
    {
        lasting = eraseBit(lasting, square[i][column].value - 1);
    }

    return lasting;
}


uint lastNumbersBlock(peep square[9][9], uint block_x, uint block_y)
{
    uint lasting = ~0;
    const uint range_x = 3 * (block_x + 1);
    const uint range_y = 3 * (block_y + 1);
    
    int i, j;
    
    for(i=block_x*3; i<range_x; i++)
    {
        for(j=block_y*3; j<range_y; j++)
        {
            lasting = eraseBit(lasting, square[i][j].value - 1);
        }
    }

    return lasting;
}

uint scanNumbers(peep *square[9][9])
{
    int i, j, k;
    // Get numbers from user
    fflush(stdin);
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            // If result is not uint, store zero.
            if(!scanf("%u", &square[i][j]->value)) square[i][j]->value = 0;
            square[i][j]->possibleValues = ~0;
        }
    }
}

void updateScreen(peep square[9][9])
{
    int i, j, k;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            printf("%u_", square[i][j].value);
            if((j+1)%3 == 0 && j) printf("|_");
        }

        printf("\n");
        if((i+1)%3 == 0)
        {
            for(k=0; k<(2*9 + 2*3 - 1); k++) printf("-");
            printf("\n");
        }
        printf("\n");
    }
}


int main ()
{
    int i, j, k, // First indices
        ii, jj, kk; // Second indices
    peep square[9][9]; // Main Matrix declaration
    
    scanf("%d", &ii); // To use default sudoku, use negative number 
    if(ii<0)
    {
        jj = 0;
        for(i=0; i<9; i++)
        {
            for(j=0; j<9; j++)
            {
                square[i][j].value = (jj % 9) + 1;
                jj++;
            }
        }
    }
    else scanNumbers(&square);
    
    uint lasting;
    //Possible lasting values for each square in all lines
    for(i=0; i<9; i++)
    {
        lasting = lastNumbersLine(square, i);
        for(j=0; j<9; j++)
        {
            square[i][j].possibleValues &= lasting;    
        }
    }
    
    //Possible lasting values for each square in all columns
    for(i=0; i<9; i++)
    {
        lasting = lastNumbersColumn(square, i);
        for(j=0; j<9; j++)
        {
            square[j][i].possibleValues &= lasting;  // Note orientation
        }
    }
    
    //Possible lasting values for each block 
    for(i=0; i<3; i++)
    {
        for(j=0; j<3; j++)
        {
            lasting = lastNumbersBlock(square, i, j);
            for(ii=i*3; ii<i*3+3; ii++)
            {
                for(jj=j*3; jj<j*3+3; jj++)
                {
                    square[ii][jj].possibleValues &= lasting; 
                }
            }
        }
    }
    
    // The possible values for each square should be now complete.
    // Now we need to find squares that have just one possible value and assign
    //it to the square value.

    updateScreen(square);
    return 0;
}
