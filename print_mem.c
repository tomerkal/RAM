#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"
#include "print.h"

/********************* print memory function ****************/

void print_mem(FILE *fp)
{
    int i=0, j=0;
    char *ptr, name[4];

    fprintf(fp, "\n\n--------------------------------------------------------------------------------------------------\n");
    fprintf(fp, "\t\t\t /* Memory printout *\\\n");
    fprintf(fp, "--------------------------------------------------------------------------------------------------\n");

    /*********** printing the RAM **************/

        fprintf(fp, "RAM:\n\n");

        for (ptr=RAM ; ptr<RAM+sizeofRam ; ptr++)
        {
            if (*ptr == '$')
                fprintf(fp, "$");
            else
            {
                if (*ptr == '*')
                    fprintf(fp, "*");
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
                    ptr = print_data(i, ptr, fp);
                }
            }
        }
        fprintf(fp, "\n\n");

    /************ printing the memory map *****************/

        fprintf(fp, "Memory map:\n\n");

        for (j=0 ; j<sizeofmapArray ; j++)
        {
            if (mapArray[j] == NULL)
                fprintf(fp, "*");
            else
                {
                    fprintf(fp, " ");
                    for (i=0 ; i<4 ; i++)
                        fprintf(fp, "%c", mapArray[j]->name[i]);
                    fprintf(fp, " ");
                }
        }
        fprintf(fp, "\n\n");

    /************************* printing the cache **************/

        fprintf(fp, "Cache:\n\n");

        for (ptr=cache ; ptr<cache+sizeofCache ; ptr++)
        {
            if (*ptr == '*')
                fprintf(fp, "*");
            else
            {
                /** print and save the name */
                for (i=0 ; i<4 ; i++)
                {
                    fprintf(fp, "%c", *(ptr));
                    name[i] = *(ptr);
                    ptr++;
                }

                fprintf(fp, "%d", *(int*)ptr); // print size
                ptr += 4;
                fprintf(fp, "%d", *ptr); // print dirty-bit
                ptr++;
                fprintf(fp, "%d", *(int*)ptr); // print LRU
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
                ptr = print_data(i, ptr, fp); // print data

            }
        }
        fprintf(fp, "\n\n\n");

}


/************ print data function ************************/

char* print_data(int mem_index, char* start_ptr, FILE *fp)
{
    int size=0;
    char type_tav, *end_index, *ptr=start_ptr;

    size = mapArray[mem_index]->size;
    type_tav = mapArray[mem_index]->type[0];
    end_index = ptr+size;
    while (ptr<end_index)
    {
        if (*ptr == 'x')
            fprintf(fp, "x");
        else
        {
            switch (type_tav)
            {
            case 'i':
                fprintf(fp, "%d", *(int*)ptr);
                ptr += 3;
                break;

            case 'c':
                fprintf(fp, "%c", *ptr);
                break;
            }
        }
        ptr++;
    }
    ptr--;
    return ptr;
}
