#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"
#include "cache.h"
#include "ram.h"
#include "allocation.h"
#include "print.h"
#include "defrag.h"
#include "funcy.h"
#include "rw.h"

int main()
{
    int i=0, endof=0;
    char first_word[16];
    FILE *commands, *writeFile;

    initRam();
    commands = fopen("test.txt", "r");
    writeFile = fopen("out.txt", "w");
    fseek(commands, 0, 2);
    endof = ftell(commands);
    rewind(commands);

    while (ftell(commands) < endof-1)
    {
        fscanf(commands, "%s", first_word);
        first_word_checker(commands, writeFile, first_word);
    }

    fclose(commands);
    fclose(writeFile);

    for (i=0 ; i < sizeofmapArray ; i++)
    {
        if (mapArray[i] != NULL)
        {                               /** release all map array before exit due to malloc */
            free(mapArray[i]);
        }
    }

    return 0;
}


void first_word_checker(FILE *fp, FILE *fp_out, char word[])
{
    char name[5], type[5];
    int size=0;

    if (!strcmp(word, "dynamic"))
    {
        fseek(fp,13,1);
        fscanf(fp, "%s %s %d", type, name, &size);
        dynamicAllocation(name, type, size, fp_out);
    }

    else if (!strcmp(word, "static"))
    {
        fseek(fp,13,1);
        fscanf(fp, "%s %s %d", type, name, &size);
        staticAllocation(name, type, size, fp_out);
    }

    else if (!strcmp(word, "write:"))
    {
        write(fp, fp_out);
    }

    else if (!strcmp(word, "Defragmentation"))
    {
        defragment();
    }

    else if (!strcmp(word, "print"))
    {
        print_mem(fp_out);
        fseek(fp,5,1);
    }

    else if (!strcmp(word, "free"))
    {
        fscanf(fp, "%s", name);
        free_mem(name);
        fseek(fp, 1, 1);
    }

    else if (!strcmp(word, "read:"))
    {
        read(fp, fp_out);
    }

    else if (!strcmp(word, "call:"))
    {
        funcy(fp, fp_out);
    }

    else if (!strcmp(word, "ret"))
    {
        ret();
    }
}
