#define MAX_REPOS 20000000
#define MAX_SIZE 200000

typedef char *KEY;

typedef struct repos * REPOS;

char* getdescription (REPOS Trepos,KEY id);
REPOS initREPOS ();
int addREPOS (KEY i, REPOS repo_table,int owner,char* lingua,DATA date ,char*descricao);
int searchREPOS (KEY i,REPOS repo_table);
void printREPOS(REPOS r);
void printUMREPO(struct repos r);
char* searchlang(REPOS repos, KEY id_repo);
void searchIDREPOSlang(REPOS repos, int array_ids[], char* lang);
