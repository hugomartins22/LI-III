typedef struct stats *  STATS;

void query1 (STATS stats1,int N);
void query2 (STATS stats1,int N);
void query3 (STATS stats1,int N);
void query4(STATS stats1,int N);
void query5(USERS users, COMMITS commits, DATA data1, DATA data2, int top,int N);
void query6(USERS users, REPOS repos, COMMITS commits_table, int n, char* lang,int N);
void query7(COMMITS Tcommits,REPOS Trepos,int cAno ,int cMes,int cDia,int N);
void query8 (REPOS repos, COMMITS commits, int top, DATA data,int N);
void query9(USERS tUser,COMMITS tCom,REPOS tRepo,int N,int totalusers,int q);
void query10(COMMITS tabela,int top,USERS tabelauser,int N);
int get_utilizadores(STATS stats);
STATS init_stats ();
STATS insere_stats(STATS stats1,int bot,int user,int organ,int totalusers);
STATS insere_stats_repos (STATS stats1,int contrepos);
STATS insere_stats_commits_bots (STATS stats1,int totalcommits,int contbot);
STATS insere_stats_colabs (STATS stats1,int colaboradores);