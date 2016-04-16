#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"
#include "ram.h"
#include "cache.h"
#include "defrag.h"


/********* initializing RAM function *********/

void initRam()
{
    int i=0;

    last_stack = RAM;
    last_heap = RAM+sizeofRam-1;
    max_LRU = 0;

    for (i=0 ; i < sizeofRam ; i++)
        RAM[i] = '*';

    for (i=0 ; i < sizeofCache ; i++)
        cache[i] = '*';

}

/********* updating RAM function *********/

void update_ram(char* ptr_incache)
{
    int i=0, j=0, size=0;
    char *ptr=ptr_incache;

    for (i=0 ; i < sizeofmapArray ; i++)
    {
        if (mapArray[i] != NULL)
        {
           if (is_name(mapArray[i]->name, ptr))
           {
               size = mapArray[i]->size;
               break;
            }
        }
    }
    ptr += 13;
    for (j=0 ; j<size ; j++)
    {
        *((mapArray[i]->addressInRam)+j) = *(ptr+j);
    }
}

/********* is in RAM function *********/

char* is_inRam(char* name)
{
    int i=0;

    for (i=0 ; i<sizeofmapArray ; i++)
    {
        if (mapArray[i] != NULL)
        {
            if (is_name(mapArray[i]->name, name))
                return mapArray[i]->addressInRam;
        }
    }
    return NULL;
}


/**************** update last stack function ****************/

void update_last_stack(char* ptr)
{
    if (ptr > last_stack)
        last_stack = ptr+1;
}



/**************** update last heap function ****************/

void update_last_heap(char* ptr)
{
    if (ptr < last_heap)
        last_heap = ptr;
}


/****************** free memory function ****************/

void free_mem(char var_name[])
{
    int i=0, size=0, temp_size=0;
    char *ptr;

    for (i=0 ; i < sizeofmapArray ; i++)
    {
        if (mapArray[i] != NULL)
        {
           if (is_name(mapArray[i]->name, var_name))
           {
               size = mapArray[i]->size;
               put_star(mapArray[i]->addressInRam, size);
               free(mapArray[i]);
               mapArray[i] = NULL;
               break;
            }
        }
    }

    for (ptr=cache ; ptr<cache+sizeofCache ; ptr++)
    {
        if (*ptr == '*')
            return;
        if (is_name(ptr, var_name))
        {
            put_star(ptr, 13+size);
            defragment_cache();
            return;
        }
        else
        {
            ptr += 4;
            temp_size = *(int*)ptr;
            ptr += (8+temp_size);
        }

    }
}

