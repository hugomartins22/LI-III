#define MAX_SIZE 200000
typedef char *KEY;

typedef struct aux10 * AUX10;
typedef struct ids_arr *IDS_ARR;

unsigned long hash(KEY k);
int elem(char* str,char* arr[],int size);
char* comuns (char* l1, char* l2,int followers,int following);
void mergeSort(AUX10 arr, int l, int r);
void reverseArray(AUX10 arr, int start, int end);
int checkID(IDS_ARR ids_arr ,KEY i, int size);
void mergeSort_IDS(IDS_ARR arr, int l, int r);
void add_AUTHORID(IDS_ARR ids_array,char* author_id);
IDS_ARR initIDS_ARR(int size);
void print_Q (IDS_ARR author_ids_array,USERS users,int top,int N, int size);
void print_q9(int N,USERS tUser,AUX10 arr,int q,int j);
AUX10 init_aux10 (int size);
AUX10 insere_id(AUX10 arr,int j,KEY tUser_id,int c);
void print_msg(AUX10 arr,int k,USERS tabelauser,KEY id_repo,FILE *f);
void free_arr(AUX10 arr);
AUX10 aux_q9 (USERS tUser,COMMITS tCom,AUX10 arr,int *pos);
IDS_ARR aux_q6 (COMMITS commitaux,IDS_ARR author_ids_array,int* array_id_repos,COMMITS commits_table, int size);
void aux_q10(COMMITS tabela,int top,USERS tabelauser,FILE *f,AUX10 arr);
void auxq5(COMMITS commits, IDS_ARR author_ids_array,DATA data1, DATA data2,DATA commit_at, int size);
int add_id_array (IDS_ARR ids_arr, KEY i, int size);
void freeIDS_ARR (IDS_ARR arr, int size);