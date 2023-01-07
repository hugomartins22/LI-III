#define MAX_SIZE 200000
typedef char *KEY;

typedef struct lang_arr * LANG_ARR;

void add_language(LANG_ARR arr, char* lang);
int checkLANG(LANG_ARR arr,char* lang);
void mergeSort_Lang(LANG_ARR arr, int l, int r);
LANG_ARR init_lang(int size);
void print_lang (LANG_ARR lang_array,int N,int top);
void incrementa2(LANG_ARR lang,int pos,int vezes);
