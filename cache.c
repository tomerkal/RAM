#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"
#include "cache.h"
#include "defrag.h"
#include "ram.h"

/*********************** is in cache function *******************/

char* is_inCache(char* name)
{
    int curr_size=0;
    char *ptr;

    for (ptr=cache ; ptr<cache+sizeofCache ; ptr++)
    {
        if (*ptr == '*')
            break;
        if (is_name(ptr, name))
            return ptr;
        ptr += 4;
        curr_size = *(int*)ptr;
        ptr += 8+curr_size;
    }
    return NULL;
}


/*********************** find minimum LRU function *******************/

char* find_min_LRU()
{
    /** finds the minimum LRU and returns pointer to that variable */

    int minLRU=2147483647, curr_size=0, curr_LRU=0;
    char *curr_min, *ptr;

    for (ptr=cache ; ptr<cache+sizeofCache ; ptr++)
    {
        if (*ptr == '*')
            break;
        ptr += 4;
        curr_size = *(int*)ptr;
        ptr += 5;
        curr_LRU = *(int*)ptr;
        if (curr_LRU < minLRU)
        {
            minLRU = curr_LRU;
            curr_min = ptr-9;
        }
        ptr += 3+curr_size;
    }
    return curr_min;
}


/*********************** update cache function FOR STRINGS *******************/

void update_cache_str(char* place_incache, char str[], int len, int offset)
{
    int i=0;
    char *ptr=place_incache;

    max_LRU++;
    ptr += 8;
    *ptr = 1; // update dirty bit
    ptr++;
    strncpy(ptr, (char*)&max_LRU, 4); // update LRU
    ptr += (4+offset);
    for (i=0 ; i<len ; i++)
        *(ptr+i) = *(str+i);
}


/*********************** update cache function FOR CHARS *******************/

void update_cache_char(char* place_incache, char chr, int offset)
{
    char *ptr=place_incache;

    max_LRU++;
    ptr += 8;
    *ptr = 1; // update dirty bit
    ptr++;
    strncpy(ptr, (char*)&max_LRU, 4); // update LRU
    ptr += (4+offset);
    *ptr = chr;
}


/*********************** update cache function FOR INTS *******************/

void update_cache(char* place_incache, int val, int offset)
{
    char *ptr=place_incache;

    max_LRU++;
    ptr += 8;
    *ptr = 1; // update dirty bit
    ptr++;
    strncpy(ptr, (char*)&max_LRU, 4); // update LRU
    ptr += (4+offset);
    strncpy(ptr, (char*)&val, 4);
}


/*********************** place in cache finder function *******************/

char* place_in_cache(int size)
{
    /** looks for place in cache, returns pointer to start point, or null if no place found */

    int counter=0, size_required=13+size;
    char *start_index, *ptr;

    for (ptr=cache ; ptr<cache+sizeofCache ; ptr++)
    {
        while (*ptr != '*')
            ptr++;
        if (*(ptr-1)!='*' || ptr == cache)
        {
            start_index = ptr;
            counter = 0;
        }
        counter++;
        if (counter==size_required)
            return start_index;

    }
    return NULL;
}


/*********************** put item in cache function *******************/

void create_cache_item(char* place_incache, int i)
{
    int j=0;
    char *ptr=place_incache;

    strncpy(ptr, mapArray[i]->name, 4);
    ptr += 4;
    strncpy(ptr, (char*)&(mapArray[i]->size), 4);
    ptr += 4;
    *ptr = 0;
    ptr++;
    max_LRU++;
    strncpy(ptr, (char*)&max_LRU, 4);
    ptr += 4;
    for (j=0 ; j<mapArray[i]->size ; j++)
    {
        *(ptr+j) = *((mapArray[i]->addressInRam)+j);
    }
}


/************* update LRU function FOR STRINGS *********************/

char* update_lru(char* place_incache, int offset)
{
    char *ptr=place_incache;

    ptr += 9;
    max_LRU++;
    strncpy(ptr, (char*)&max_LRU, 4);
    ptr += (4+offset);
    return ptr;
}


/************** clear space in cache function ***********************/

void delete_item()
{
    char *to_delete;

    to_delete = find_min_LRU();
    if (*(to_delete+8) == 1)
        update_ram(to_delete);
    put_star(to_delete, 13+(*(int*)(to_delete+4)));
    defragment_cache();
}
