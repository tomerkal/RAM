#ifndef CACHE_H_INCLUDED
#define CACHE_H_INCLUDED

int max_LRU, min_LRU;

char* is_inCache(char* name);
char* find_min_LRU();
void update_cache_str(char* place_incache, char str[], int len, int offset);
void update_cache_char(char* place_incache, char chr, int offset);
void update_cache(char* place_incache, int val, int offset);
char* place_in_cache(int size);
void create_cache_item(char* place_incache, int i);
void delete_item();
char* update_lru(char* place_incache, int offset);

#endif // CACHE_H_INCLUDED
