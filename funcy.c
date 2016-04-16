#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"
#include "funcy.h"
#include "ram.h"
#include "allocation.h"

/********************** call a function ********************/

void funcy(FILE *fp, FILE *fp_out)
{
    char chr;

    *last_stack = '$';
    last_stack+=1;
    while ((chr=getc(fp))!='(');
    get_parameter(fp, fp_out);
    fseek(fp, 2, 1);
    get_parameter(fp, fp_out);
    while (((chr=getc(fp))!='\n') && (chr != EOF));
}


/**************** get parameters from function call ***********/

void get_parameter(FILE *fp, FILE *fp_out)
{
    int i=0, size=1;
    char type[5], name[5], chr;

    fscanf(fp, "%s", type);
    fseek(fp, 1, 1);
    for (i=0 ; i<4 ; i++)
    {
        name[i] = getc(fp);
    }
    if ((chr = getc(fp)) == '[')
    {
        size = txt2int(fp, ']');
    }
    staticAllocation(name, type, size, fp_out);
}


/****************** return from function ***************/

void ret()
{
    int i=0;
    char *ptr=last_stack-1;

    while (*ptr!='$')
    {
        for (i=0 ; i < sizeofmapArray ; i++)
        {
            if (mapArray[i] != NULL)
            {
                if (mapArray[i]->addressInRam == ptr)
                {
                    free_mem(mapArray[i]->name);
                    break;
                }
            }
        }

        ptr--;
    }
    *ptr = '*';
    last_stack = ptr;
}
