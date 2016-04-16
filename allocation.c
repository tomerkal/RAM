#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"
#include "allocation.h"
#include "ram.h"

/************************ dynamic allocation function **************/

void dynamicAllocation(char name[], char type[], int size, FILE *fp)
{
    int size_required = size, counter=0;
    char *ptr;

    if (*type == 'i')
        size_required *= 4;


    for (ptr=RAM+sizeofRam-1 ; ptr > last_stack ; ptr--)
    {
        if (*ptr != '*')
            counter = 0;
        while (*ptr != '*')
            ptr--;
        counter++;
        if (counter == size_required) /** means we found a good space */
        {
            put_x(ptr, size_required);
            update_last_heap(ptr);
            create_map(name, type, size_required, ptr, fp);
            return;
        }
    }
    fprintf(fp, "Allocation failed!\n");
}


/************************ static allocation function **************/

void staticAllocation(char name[], char type[], int size, FILE *fp)
{
    int size_required = size, counter=0;
    char *ptr;

    if (*type == 'i')
        size_required *= 4;

    for (ptr=last_stack ; ptr < last_heap ; ptr++)
    {
        counter++;
        if (counter == size_required) /** means we found a good space */
        {
            put_x(last_stack, size_required);
            create_map(name, type, size_required, last_stack, ptr);
            update_last_stack(ptr);
            return;
        }
    }
    fprintf(fp, "Stack overflow!\n");
}


/****************** create map array item function *********************/

void create_map(char *name, char *type, int size, char *address, FILE *fp)
{
    int i=0;

    while (mapArray[i] != NULL)
        i++;

    mapArray[i] = (memIdentifier*)malloc(sizeof(memIdentifier));
    if (mapArray[i] == NULL)
    {
        fprintf(fp, "Allocation of memory identifier failed!");
        return;
    }

    strncpy(mapArray[i]->name, name, 4);

    strncpy(mapArray[i]->type, type, 4);

    mapArray[i]->size = size;
    mapArray[i]->addressInRam = address;

}
