#ifndef MEMORY_UNITS_H
#define MEMORY_UNITS_H

#define sizeofRam (10000)
#define sizeofCache (1000)
#define sizeofIdentiy (16)
#define sizeofmapArray (100)




char RAM[sizeofRam];
char cache[sizeofCache];


typedef struct
{
	char name[4];
	char type[4];
	int  size;
	char *addressInRam;


}memIdentifier;


memIdentifier *mapArray[sizeofmapArray];

#endif // MEMORY_UNITS_H
