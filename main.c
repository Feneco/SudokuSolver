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
typedef struct _block  //peep struct
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


uint lastNumbersLine(peep block[9][9], uint line)
{
    uint lasting = ~0;
    int i;
    for(i=0; i<9; i++)
    {
        lasting = eraseBit(lasting, block[line][i].value - 1);
    }

    return lasting;
}

uint lastNumbersCollumm(peep block[9][9], uint collumm)
{
    uint lasting = ~0;
    int i;
    for(i=0; i<9; i++)
    {
        lasting = eraseBit(lasting, block[i][collumm].value - 1);
    }

    return lasting;
}

void updateScreen(peep block[9][9])
{
    int i, j, k;
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            printf("%u ", block[i][j].value);
            if((j+1)%3 == 0) printf("| ");
        }

        printf("\n");
        if((i+1)%3 == 0)
        {
            for(k=0; k<(2*9 + 2*3 - 1); k++) printf("_");
            printf("\n");
        }
        printf("\n");
    }
}

int main ()
{
    int i, j, k;
    peep block[9][9];

    //Get numbers from user
    fflush(stdin);
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            if(!scanf("%u", &block[i][j].value)) block[i][j].value = 0;  // If it's not an integer
            //block[i][j].value = (i+j + 1)%9;
            block[i][j].possibleValues = ~0;
        }
    }

    updateScreen(block[9][9]);
    return 0;
}
