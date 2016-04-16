#ifndef FUNCS_H_INCLUDED
#define FUNCS_H_INCLUDED


void first_word_checker(FILE *fp, FILE *fp_out, char word[]);
void put_x(char *ptr, int size_required);
void put_star(char* ptr, int size);
int is_name(char *name, char *str);
int txt2int(FILE *fp, char end);



#endif // FUNCS_H_INCLUDED
