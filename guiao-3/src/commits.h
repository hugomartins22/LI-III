#define MAX_COMMITS 20000000
#define MAX_SIZE 200000

typedef char *KEY;
typedef struct commits * COMMITS;
typedef struct commits_aux * COMMITS_AUX;


COMMITS initCOMMITS ();
COMMITS_AUX initCOMMITSAUX ();
void addLineCOMMITS(COMMITS com_table, int i, char *linha);
void createLineCOMMITS(COMMITS com_table, int i, char *x);
int addCOMMITSM (KEY i, COMMITS com_table,char *x,int num);
int cont_commits (COMMITS table,KEY i,KEY user);
int addCOMMITS_Aux (KEY i, COMMITS_AUX com_table,char *x);
int elemCOMMITS (char** commit_line,int n, char *x);
int searchCOMMITAUX (KEY i,COMMITS_AUX com_table);
void printCOMMITS(COMMITS c);
int ncommitsvalid(DATA data, struct commits commit, KEY id_repo);
COMMITS findCOMMIT(COMMITS commits, KEY id);
char* extract_author_id(char* commit_line);
char* extract_commit_at_date(char* commit_line);
COMMITS initcommit();
void aux_q8(DATA data,REPOS repos, COMMITS commits,LANG_ARR lang_array);
int contabots(COMMITS tabelacommit);
char* get_commitline(COMMITS commitaux,int i);
int get_p (COMMITS commitaux);
int get_p2 (COMMITS commitaux,int i);
char* get_commitline2(COMMITS commitaux,int i,int j);
char get_state2(COMMITS tabela,int i);
KEY get_id_repo(COMMITS tabela,int i);
void aux_q7(int cAno ,int cMes,int cDia,REPOS Trepos,int N,COMMITS Tcommits);


