#define MAX_USERS 6000000
#define MAX_REPOS 20000000
#define MAX_COMMITS 20000000
#define MAX_SIZE 200000

typedef char *KEY;


typedef struct stats
{
	int user;
	int bot;
	int organization;
	int repos ;
	int colaboradores ;
	int bots_colab;
	int utilizadores;
	int commits;
}*  STATS;


typedef struct users
{
	KEY id;
	char state; // 0 = free; 1 = used
	int followers;
	int following;
	char* login;
	char* type;
	char* follower_list;
    char* following_list;
	char* repos;
} * USERS;


typedef struct commits 
{
	KEY id_repo;
	char state; // 0 = free; 1 = used;
	int size; //tamanho do array de linhas
	int p; //posiçao no array de linhas 
	int bot; // 0 não tem bots , 1 tem bot
	char ** commit_line; //array de strings (commits)

	
} * COMMITS;



typedef struct commits_aux
{
	KEY author_id;
	char state; // 0 = free; 1 = used;

} * COMMITS_AUX;


typedef struct data
{
	int ano;
	int mes;
	int dia; 

}* DATA;

typedef struct repos 
{
	KEY id; 
	char state; // 0 = free; 1 = used

	int owner_id;
	char* language;
	DATA updated_at; 
	char* description;

} * REPOS;


typedef struct aux10
{
	KEY author_id;
	int size; 

} * AUX10;

typedef struct lang_arr
{
	char* language;
	int number;
	int pos;
	int size;

} * LANG_ARR;

typedef struct ids_arr
{
	KEY id;
	int number;
	int pos;
	int size;

} * IDS_ARR;



unsigned long hash(KEY k);
void initUsers (USERS table);
int add_users (KEY i, char* log, char* t,char* follower_list,char* following_list , USERS table,int followers,int following);
int searchID (KEY i,USERS table);

void initCOMMITS (COMMITS com_table);

int elemCOMMITS (char** commit_line,int n, char *x);
void addLineCOMMITS(COMMITS com_table, int i, char *linha);
void initCommits (COMMITS com_table) ;
void createLineCOMMITS(COMMITS com_table, int i, char *x);
int addCOMMITSM (KEY i, COMMITS com_table,char *x,int num);
void printUSERS(USERS t);

int searchCOMMIT1 (KEY i,COMMITS com_table);
int searchCOMMITAUX (KEY i,COMMITS_AUX com_table);
int searchCOMMIT2 (KEY i,COMMITS com_table, char *x);
void printCOMMITS(COMMITS c);
void initREPOS (REPOS repo_table) ;
int addREPOS (KEY i, REPOS repo_table,int owner,char* lingua,DATA date,char* descricao);
int searchREPOS (KEY i,REPOS repo_table);
void printREPOS(REPOS r);
void initCOMMITSAUX (COMMITS_AUX com_table);
int addCOMMITS_Aux (KEY i, COMMITS_AUX com_table,char *x);

int checkbot (KEY i ,USERS table ) ;
DATA getdata (char*date)  ;

int comparedata (DATA updated_at , int compano  ,int compmes ,int compdia) ;
char* getlogin (USERS table , KEY i) ;
char* getdescription (REPOS Trepos,KEY id);
int comparedate (DATA  d1, DATA  d2);
int comparedate2 ( DATA  d1,  int d2ano , int d2mes,int d2dia );


char* getFriends (KEY i,USERS table);
char* getFriendRepo (KEY i,USERS table);
int cont_commits (COMMITS table,KEY i,KEY user);
char* comuns (char* l1, char* l2,int followers,int following);
int elem(char* str,char* arr[],int size);
void addRepoToUser (char* repo_id,int owner_id,USERS tabelauser);


char* searchlang(REPOS repos, KEY id_repo);
int ncommitsvalid(DATA data, struct commits commit, KEY id_repo);
int compare_DATAS (DATA data1, DATA ref, int N);
int checkLANG(LANG_ARR arr, int size, char* lang);
void add_language(LANG_ARR arr, char* lang, int pos);
void searchIDREPOSlang(REPOS repos, int* array_ids, char* lang);
int checkID(IDS_ARR ids_array, int size, KEY id);
COMMITS findCOMMIT(COMMITS commits, KEY id);
char* extract_author_id(char* commit_line);
void add_AUTHORID(IDS_ARR ids_array,char* author_id,int pos);
char* extract_commit_at_date(char* commit_line);
void printAUTHORIDS_ARR(IDS_ARR ids_arr, USERS users_table,int size);
void mergeSort_Lang(LANG_ARR arr, int l, int r);
void merge_Lang(LANG_ARR arr, int l, int m, int r);
void mergeSort_IDS(IDS_ARR arr, int l, int r);
void merge_IDS(IDS_ARR arr, int l, int m, int r);
void reverseArray(AUX10 arr, int start, int end);
void mergeSort(AUX10 arr, int l, int r);
void merge(AUX10 arr, int l, int m, int r);
