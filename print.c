#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"
#include "print.h"

/********************* print memory function ****************/

void print_mem()
{
    int i=0, j=0;
    char *ptr, name[4];

    printf("\t\t\t /* Memory printout *\\\n");
    printf("-------------------------------------------------------------------------\n");

    /*********** printing the RAM **************/

        printf("RAM:\n\n");

        for (ptr=RAM ; ptr<RAM+sizeofRam ; ptr++)
        {
            if (*ptr == '$')
                printf("$");
            else
            {
                if (*ptr == '*')
                    printf("*");
                else
                {
                    for (i=0 ; i<sizeofmapArray ; i++)
                    {
                        if (mapArray[i] != NULL)
                        {
                            if (mapArray[i]->addressInRam == ptr)
                                break;
                        }
                    }
                    ptr = print_data(i, ptr);
                }
            }
        }
        printf("\n\n");

    /************ printing the memory map *****************/

        printf("Memory map:\n\n");

        for (j=0 ; j<sizeofmapArray ; j++)
        {
            if (mapArray[j] == NULL)
                printf("*");
            else
                {
                    printf(" ");
                    for (i=0 ; i<4 ; i++)
                        printf("%c", mapArray[j]->name[i]);
                    printf(" ");
                }
        }
        printf("\n\n");

    /************************* printing the cache **************/

        printf("Cache:\n\n");

        for (ptr=cache ; ptr<cache+sizeofCache ; ptr++)
        {
            if (*ptr == '*')
                printf("*");
            else
            {
                /** print and save the name */
                for (i=0 ; i<4 ; i++)
                {
                    printf("%c", *(ptr));
                    name[i] = *(ptr);
                    ptr++;
                }

                printf("%d", *(int*)ptr); // print size
                ptr += 4;
                printf("%d", *ptr); // print dirty-bit
                ptr++;
                printf("%d", *(int*)ptr); // print LRU
                ptr += 4;

            /** search in map array for corresponding variable */
                for (i=0 ; i<sizeofmapArray ; i++)
                {
                    if (mapArray[i] != NULL)
                    {
                        if (is_name(mapArray[i]->name, name))
                            break;
                    }
                }
                ptr = print_data(i, ptr); // print data

            }
        }
        printf("\n\n");

    printf("-------------------------------------------------------------------------\n");
}


/************ print data function ************************/

char* print_data(int mem_index, char* start_ptr)
{
    int size=0;
    char type_tav, *end_index, *ptr=start_ptr;

    size = mapArray[mem_index]->size;
    type_tav = mapArray[mem_index]->type[0];
    end_index = ptr+size;
    while (ptr<end_index)
    {
        if (*ptr == 'x')
            printf("x");
        else
        {
            switch (type_tav)
            {
            case 'i':
                printf("%d", *(int*)ptr);
                ptr += 3;
                break;

            case 'c':
                printf("%c", *ptr);
                break;
            }
        }
        ptr++;
    }
    ptr--;
    return ptr;
}
