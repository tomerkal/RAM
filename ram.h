#ifndef RAM_H_INCLUDED
#define RAM_H_INCLUDED

char *last_stack, *last_heap;

void initRam();
void update_ram(char* ptr_incache);
char* is_inRam(char* name);
void update_last_heap(char* ptr);
void update_last_stack(char* ptr);
void free_mem(char var_name[]);


#endif // RAM_H_INCLUDED
