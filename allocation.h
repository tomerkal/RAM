#ifndef ALLOCATION_H_INCLUDED
#define ALLOCATION_H_INCLUDED

void dynamicAllocation(char name[], char type[], int size, FILE *fp);
void staticAllocation(char name[], char type[], int size, FILE *fp);
void create_map(char *name, char *type, int size, char *address, FILE *fp);

#endif // ALLOCATION_H_INCLUDED
