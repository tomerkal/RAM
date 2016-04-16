#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"
#include "ram.h"
#include "defrag.h"


/*********************** defragment function ********************/

void defragment()
{
    int counter=0, i=0;
    char *ptr;

    for (ptr=RAM+sizeofRam-1 ; ptr >= last_heap ; ptr--)
    {
        if (counter == 0 && *ptr != '*')
            continue;
        if (*ptr == '*')
            counter++;
        else
        {
            *(ptr+counter) = *ptr;
            *ptr = '*';
            for (i=0 ; i < sizeofmapArray ; i++)
            {
                if (mapArray[i] != NULL)
                {
                    if (mapArray[i]->addressInRam == ptr)
                    {
                        mapArray[i]->addressInRam = ptr+counter;
                        break;
                    }
                }
            }
        }
    }
    for (ptr=last_heap ; *ptr == '*' ; ptr++);
    update_last_heap(ptr);

    defragment_cache();
 }


/*********************** defragment function for cache ********************/

void defragment_cache()
{
    int counter=0;
    char *ptr;

    for (ptr=cache ; ptr<cache+sizeofCache ; ptr++)
    {
        if (counter == 0 && *ptr != '*')
            continue;
        if (*ptr == '*')
            counter++;
        else
        {
            *(ptr-counter) = *ptr;
            *ptr = '*';
        }
    }
}


