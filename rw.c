#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryUnits.h"
#include "funcs.h"
#include "cache.h"
#include "ram.h"
#include "rw.h"

/********** write to memory function ***************/

void write(FILE *fp, FILE *fp_out)
{
    int i=0, j=0, offset=0, size=0, val=0, len=0;
    char chr, name[5], type_tav, *place_incache, *place_inram, input[41];


    fseek(fp,1,1);
    chr = getc(fp);
    if (chr == '*')
    {
        fseek(fp,1,1);
        for (i=0 ; i < 4 ; i++)
        {
            chr = getc(fp);
            name[i] = chr;
        }
        if (is_inCache(name) == NULL && is_inRam(name) == NULL)
        {
            name[4] = '\0';
            fprintf(fp_out, "The variable %s is being used without being initialized!\n", name);
            return;
        }
        fseek(fp,1,1);
        offset = txt2int(fp, ')');
        fseek(fp,1,1);
        for (i=0 ; i<sizeofmapArray ; i++)
        {
            if (is_name(mapArray[i]->name, name))
                break;
        }
        type_tav = mapArray[i]->type[0];
        size = mapArray[i]->size;
        if (type_tav == 'i')
            offset *= 4;
        if (offset>size-1)
        {
            fprintf(fp_out, "Memory Corruption!\n");
            return;
        }
        switch (type_tav)
        {
        case 'i':
            val = txt2int(fp, '\n');
            place_incache = is_inCache(name);
            if (place_incache == NULL)
            {
                fprintf(fp_out, "Cache miss!\n\n");
                place_inram = is_inRam(name);
                strncpy(place_inram+offset, (char*)&val, 4);
                place_incache = place_in_cache(size);
                while (place_incache == NULL)
                {
                    delete_item();
                    place_incache = place_in_cache(size);
                }
                create_cache_item(place_incache, i);
            }
            else
                update_cache(place_incache, val, offset);
            return;

        case 'c':
            chr = getc(fp);
            place_incache = is_inCache(name);
            if (place_incache == NULL)
            {
                fprintf(fp_out, "Cache miss!\n\n");
                place_inram = is_inRam(name);
                *(place_inram+offset) = chr;
                place_incache = place_in_cache(size);
                while (place_incache == NULL)
                {
                    delete_item();
                    place_incache = place_in_cache(size);
                }
                create_cache_item(place_incache, i);
            }
            else
                update_cache_char(place_incache, chr, offset);
            return;
        }
    }

    else
    {
        fseek(fp,-1,1);
        for (i=0 ; i < 4 ; i++)
        {
            chr = getc(fp);
            name[i] = chr;
        }
        if (is_inCache(name) == NULL && is_inRam(name) ==NULL)
        {
            name[4] = '\0';
            fprintf(fp_out, "The variable %s is being used without being initialized!\n", name);
            return;
        }
        fseek(fp,1,1);
        offset = txt2int(fp, '=');

        for (i=0 ; i<sizeofmapArray ; i++)
        {
            if (is_name(mapArray[i]->name, name))
                break;
        }
        size = mapArray[i]->size;

        j=0;
        while (((chr=getc(fp)) != '\n') && chr != EOF)
        {
            input[j] = chr;
            j++;
        }
        input[j] = '\0';
        len = strlen(input);
        if (offset+len > size)
        {
            fprintf(fp_out, "Memory Corruption!\n");
            return;
        }

        place_incache = is_inCache(name);
        if (place_incache == NULL)
        {
            fprintf(fp_out, "Cache miss!\n\n");
            place_inram = is_inRam(name);
            strncpy(place_inram+offset, input, len);
            place_incache = place_in_cache(size);
            while (place_incache == NULL)
            {
                delete_item();
                place_incache = place_in_cache(size);
            }
            create_cache_item(place_incache, i);
        }
        else
            update_cache_str(place_incache, input, len, offset);
        return;
    }
}


/********** read from memory function ***************/

void read(FILE *fp, FILE *fp_out)
{
    int i=0, j=0, n=0, offset=0, size=0;
    char chr, name[5], type_tav, *place_incache, *place_inram;

    fseek(fp,1,1);
    chr = getc(fp);
    if (chr == '*')
    {
        fseek(fp,1,1);
        for (i=0 ; i < 4 ; i++)
        {
            chr = getc(fp);
            name[i] = chr;
        }
        place_incache = is_inCache(name);
        place_inram = is_inRam(name);
        if (place_incache == NULL && place_inram == NULL)
        {
            name[4] = '\0';
            fprintf(fp_out, "The variable %s could not be found in memory!\n", name);
            return;
        }
        fseek(fp,1,1);
        offset = txt2int(fp, ')');
        while ((chr=getc(fp)!='\n') && (chr != EOF));
        for (i=0 ; i<sizeofmapArray ; i++)
        {
            if (is_name(mapArray[i]->name, name))
                break;
        }
        type_tav = mapArray[i]->type[0];
        size = mapArray[i]->size;
        if (type_tav == 'i')
            offset *= 4;
        if (offset>size-1)
        {
            fprintf(fp_out, "Memory Corruption!\n");
            return;
        }
        switch (type_tav)
        {
        case 'i':
            if (place_incache == NULL)
            {
                fprintf(fp_out, "%d\n", *(int*)((mapArray[i]->addressInRam)+offset));
                place_incache = place_in_cache(size);
                while (place_incache == NULL)
                {
                    delete_item();
                    place_incache = place_in_cache(size);
                }
                create_cache_item(place_incache, i);
            }
            else
            {
                place_incache = update_lru(place_incache, offset);
                fprintf(fp_out, "%d\n", *(int*)(place_incache));
            }
            return;

        case 'c':
            if (place_incache == NULL)
            {
                fprintf(fp_out, "%c\n", *(place_inram+offset));
                place_incache = place_in_cache(size);
                while (place_incache == NULL)
                {
                    delete_item();
                    place_incache = place_in_cache(size);
                }
                create_cache_item(place_incache, i);
            }
            else
            {
                place_incache = update_lru(place_incache, offset);
                fprintf(fp_out, "%c\n", *(place_incache));
            }
            return;
        }
    }

    else
    {
        fseek(fp,-1,1);
        for (i=0 ; i < 4 ; i++)
        {
            chr = getc(fp);
            name[i] = chr;
        }
        name[4] = '\0';
        place_incache = is_inCache(name);
        place_inram = is_inRam(name);
        if (place_incache == NULL && place_inram == NULL)
        {
            name[4] = '\0';
            fprintf(fp_out, "The variable %s could not be found in memory!\n", name);
            return;
        }
        fseek(fp,1,1);

        offset = txt2int(fp, ' ');
        n = txt2int(fp, '\n');

        for (i=0 ; i<sizeofmapArray ; i++)
        {
            if (is_name(mapArray[i]->name, name))
                break;
        }

        size = mapArray[i]->size;
        if (offset+n > size)
        {
            fprintf(fp_out, "Memory Corruption!\n");
            return;
        }

        if (place_incache == NULL)
        {
            for (j=0 ; j<n ; j++)
            {
                fprintf(fp_out, "%c", *(place_inram+offset+j));
            }
            fprintf(fp_out, "\n");
            place_incache = place_in_cache(size);
            while (place_incache == NULL)
            {
                delete_item();
                place_incache = place_in_cache(size);
            }
            create_cache_item(place_incache, i);
        }

        else
        {
            place_incache = update_lru(place_incache, offset);
            for (j=0 ; j<n ; j++)
            {
                fprintf(fp_out, "%c", *(place_incache+j));
            }
            fprintf(fp_out, "\n");
        }
        return;
    }
}
