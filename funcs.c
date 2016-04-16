#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"

/****************** put star *********************/
void put_star(char* ptr, int size)
{
    char* temp;

    for (temp = ptr ; temp < ptr+size ; temp++)
        *temp = '*';

}


/***************** put x function **********************/
void put_x(char *ptr, int size)
{
    char *put;

    for (put = ptr ; put < ptr+size ; put++)
        *put = 'x';
}


/******************** is name function ***************/
int is_name(char *name, char *str)
{
    int i=0;

    for (i=0 ; i < 4 ; i++)
    {
        if (name[i] != str[i])
            return 0;
    }
    return 1;

}


/****************** change string from txt to int function *********/

int txt2int(FILE *fp, char end)
{
    int i=0;
    char chr, strofint[11];

    while ((chr=getc(fp)) != end && chr != EOF)
    {
        strofint[i] = chr;
        i++;
    }
    strofint[i] = '\0';
    return atoi(strofint);
}
