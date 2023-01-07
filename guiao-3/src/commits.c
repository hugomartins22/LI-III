#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <ncurses.h>
#include "data.h"
#include "lang.h"
#include "users.h"
#include "repos.h"
#include "commits.h"
#include "generic.h"


struct commits 
{
	KEY id_repo;
	char state; // 0 = free; 1 = used;
	int size; //tamanho do array de linhas
	int p; //posiçao no array de linhas 
	int bot; // 0 não tem bots , 1 tem bot
	char ** commit_line; //array de strings (commits)
};


struct commits_aux
{
	KEY author_id;
	char state; // 0 = free; 1 = used;

};


/**
 * \brief Função initCOMMITSAUX
 * 
 * É a função reponsável por inicializar a tabela de commits 
 * @param com_table tabela de commits
 */
COMMITS initCOMMITS () 
{
    COMMITS tabelacommit=malloc(MAX_COMMITS*sizeof(struct commits));
	int i;
	for (i=0; i<MAX_COMMITS; tabelacommit[i++].state = 0) tabelacommit[i++].bot = 0;
    return tabelacommit;
}


/**
 * \brief Função initCOMMITSAUX
 * 
 * É a função reponsável por inicializar a tabela de commits aux
 * @param com_table tabela de commits_Aux
 */
COMMITS_AUX initCOMMITSAUX () 
{
    COMMITS_AUX tabelacommit=malloc(MAX_COMMITS*sizeof(struct commits_aux));
	int i;
	for (i=0; i<MAX_COMMITS; tabelacommit[i++].state = 0);
    return tabelacommit;
}

/**
 * \brief Função addLineCOMMITS
 * 
 * É a função reponsável por adicionar a linha dos commits ao array de commits
 * @param com_table tabela de commits
 * @param i posição na tabela de hash
 * @param x linha do commit
 */
void addLineCOMMITS(COMMITS com_table, int i, char *linha)
{
    if (com_table[i].size==com_table[i].p)
	{
      com_table[i].size*=2;
      com_table[i].commit_line=realloc(com_table[i].commit_line,com_table[i].size*(sizeof(char *)));
    }
    com_table[i].commit_line[(com_table[i].p)++]=linha;
}


/**
 * \brief Função createLineCOMMITS
 * 
 * É a função reponsável por criar o array de commits 
 * @param com_table tabela de commits
 * @param i posição na tabela de hash
 * @param x linha do commit
 */
void createLineCOMMITS(COMMITS com_table, int i, char *x)
{
  com_table[i].size=1;
  com_table[i].p=1;
  com_table[i].commit_line=malloc(sizeof (char *));
  com_table[i].commit_line[0]=x;
}


/**
 * \brief Função addCOMMITSM
 * 
 * É a função reponsável por adicionar cada commit ou à tabela de hash caso nao exista ainda algum com esse id de repositório
 * ou caso exista adiciona a linha aos commits
 * @param i id do repo
 * @param com_table tabela de commits
 * @param x linha de commit
 * @param num 1 caso possua bot , 0 caso nao possua
 */
int addCOMMITSM (KEY i, COMMITS com_table,char *x,int num)
{
	unsigned long p = hash (i)%MAX_COMMITS;
	while (com_table[p].state && strcmp(com_table[p].id_repo,i))
	   p = (p+1)%MAX_COMMITS;
	if (com_table[p].state) 
	{
    addLineCOMMITS(com_table,p,x);
    return 1;
 	}
	com_table[p].id_repo=i;
	com_table[p].state = 1;
	com_table[p].bot = num;


  createLineCOMMITS(com_table,p,x);
	 return 0;
}

/**
 * \brief Função cont_commits
 * 
 * É a função reponsável por contar os commits de um user
 * @param table Array de commits
 * @param i  
 * @param user 
 * 
 */
int cont_commits (COMMITS table,KEY i,KEY user)
{
    unsigned long p = hash (i)%MAX_COMMITS;
    int cont=0;
    while (table[p].state && strcmp (table[p].id_repo,i))
        p = (p+1)%MAX_COMMITS;
    if (table[p].state && strcmp (table[p].id_repo,i)==0)
    {
        for (int j=0 ;j<table[p].p && table[p].commit_line[j];j++)
        {
            int collumn=0;
            char * token;
            char * linecopy= strdup (table[p].commit_line[j]);
            while (token = strsep (&linecopy,";"))
            {
                if (collumn == 2) 
                {
                    if (strcmp(token,user)==0)
                        cont++;
                }
                collumn ++;
            }
        }

    }
return cont;
}


/**
 * \brief Função addCOMMITS_Aux
 * 
 * É a função reponsável por verificar se ja existe aquele commit no array dos commits
 * @param commit_line Array de commits
 * @param n tamanho 
 * @param x linha a comparar
 * 
 */
int addCOMMITS_Aux (KEY i, COMMITS_AUX com_table,char *x)
{

	unsigned long p = hash (i)%MAX_COMMITS;
	if(searchCOMMITAUX(i,com_table)==1) return 1; 
	while (com_table[p].state && strcmp (com_table[p].author_id,i))
	{
		p=(p+1)%MAX_COMMITS;
	}
	if (com_table[p].state) 
	{ 
    return 1;
 	}
	com_table[p].author_id=i;
	com_table[p].state = 1;
	return 0;
}


/**
 * \brief Função elemCOMMITS
 * 
 * É a função reponsável por verificar se ja existe aquele commit no array dos commits
 * @param commit_line Array de commits
 * @param n tamanho 
 * @param x linha a comparar
 * 
 */
int elemCOMMITS (char** commit_line,int n, char *x)
{
  for (int i=0; i<n; i++)  if (strcmp(commit_line[i],x)==0) return 1;
  return 0;
}

/**
 * \brief Função printCOMMITS
 * 
 * É a função reponsável por verificar se ja existe aquele id na tabela de commits
 * @param i Id a procurar
 * @param com_table tabela de commts auxiliar
 * 
 */
int searchCOMMITAUX (KEY i,COMMITS_AUX com_table)
{
	unsigned long p = hash (i)%MAX_COMMITS;
	while (com_table[p].state && strcmp (com_table[p].author_id,i))
	    p = (p+1)%MAX_COMMITS;
	if (com_table[p].state) return 1;
	return 0;
}


/**
 * \brief Função printCOMMITS
 * 
 * É a função reponsável por imprimir todos os commits de uma tabela de hash
 * @param c (Tabela de commits)
 * 
 */
void printCOMMITS(COMMITS c){
  int i,j=0;
  for (i=0; i<MAX_COMMITS; i++) if (c[i].state) 
  { 
	  printf("IDREPO:%s \n",c[i].id_repo); 
	  printf ("tamanho  dinamico : %d\n", c[i].size);
	  printf ("tamanho  usado : %d\n", c[i].p);
	  for (j=0;j<c[i].p;j++) 
	  {
	printf ("(%s)\n " , c[i].commit_line[j]);
	  }
	  
}
}



/**
 * \brief Função ncommitsvalid
 * 
 * É a função reponsável por retornar o número válido de commits associados a um repositório. Para os commits serem válidos têm de ser depois da data recebida.
 * @param data (DATA)
 * @param commit (struct commits)
 * @param id_repo (KEY)
 * 
 */
int ncommitsvalid(DATA data, struct commits commit, KEY id_repo){

    DATA commit_at = initdata();
    int cont=0;
	char *data_commit;

	if (commit.id_repo != id_repo) return 0;
    
    for (int j=0; j<commit.p  && commit.commit_line[j] ; j++)
    {	
		data_commit= extract_commit_at_date(commit.commit_line[j]);
		commit_at=getdata(data_commit);
        if(compare_DATAS(commit_at, data, 0)) {
			cont++;
		}
    }  
	
    return cont;
}


/**
 * \brief Função findCOMMIT
 * 
 * Função responsável por retornar um commit dado o id do repositório
 * @param commits (COMMITS)
 * @param id (KEY)
 */
COMMITS findCOMMIT(COMMITS commits, KEY id)
{
    COMMITS aux = initcommit();
	unsigned long p = hash (id)%MAX_COMMITS;
    while (commits[p].state){
        if (!strcmp (commits[p].id_repo,id))
        {
            aux = commits+p;
			return aux;
        }
        else p = (p+1)%MAX_COMMITS;	
    }	

}

/**
 * \brief Função extract_author_id
 * 
 * Função responsável por retornar o author_id de um commit.
 * @param commit_line (char*)
 */
char* extract_author_id(char* commit_line){
	char* linecopy, *token;
	int collumn=0;

    linecopy= strdup (commit_line);
    
    while (token = strsep (&linecopy,";"))
    {
        if (collumn == 1) return strdup(token);
        collumn++;
    }
}





/**
 * \brief Função extract_commit_at_date
 * 
 * Função responsável por retornar a data de um commit. 
 * @param commit_line(char*)
 */
char* extract_commit_at_date(char* commit_line){
	char *secure,*linecopy, *token;
	char *tokencopy=malloc(sizeof(char)*11);
	int collumn=0;
    linecopy= strdup (commit_line);
    secure=linecopy;
    while (token = strsep (&linecopy,";")){
		if (collumn == 3) {
			for(int i=0; i<10; i++) tokencopy[i]=token[i];
			tokencopy[10]='\0';
			free(secure);
            return tokencopy;
		}
        collumn++;
    }
    
}

/**
 * \brief Função initcommit
 * 
 * Função responsável por inicializar a tabela de commits 
 *
 */
COMMITS initcommit()
{
	COMMITS commitaux=malloc(sizeof(struct commits));
	return commitaux;
}



/**
 * \brief Função aux_q7
 * 
 * função auxiliar à realização da query 7
 *
 * @param cAno (Ano a comparar)
 * @param cMes (Mes a comparar)
 * @param cDia (Dia a comparar)
 * @param Trepos (STRUCT REPOS)
 * @param N número do ficheiro de output
 * @param Tcommits (STRUCT COMMITS)
 */
void aux_q7(int cAno ,int cMes,int cDia,REPOS Trepos,int N,COMMITS Tcommits)
{
    int i,collumn;
    char* token,*linecopy;

    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w");

    for (i=0; i<MAX_COMMITS; i++) if (Tcommits[i].state) 
    { 
    DATA recente = initdata();
    DATA date = initdata();

        for (int j=0 ;j<Tcommits[i].p && Tcommits[i].commit_line[j];j++)
        {
            collumn=0;
            linecopy= strdup (Tcommits[i].commit_line[j]);
            while (token = strsep (&linecopy,";"))
            {
                if (collumn == 3) 
                    { 
                        date = getdata(token); 
                    }
                collumn ++;
            }
  
            if (comparedate(date,recente)!=1) 
            {
                recente=insere_Data(recente,date);
            }
        }

        if(comparedate2(recente,cAno,cMes,cDia)==1)
        {
            fprintf(f, "%s;%s\n" ,Tcommits[i].id_repo,getdescription(Trepos,Tcommits[i].id_repo));
        }
        free(date);
        free(recente);
    }
    free(exitfile);
    fclose(f);
}


/**
 * \brief Função aux_q8
 * 
 * Função auxiliar à realização da query 8
 *
 * @param data (Data a comparar)
 * @param repos (STRUCT REPOS)
 * @param commits (STRUCT COMMITS)
 * @param lang_array (STRUCT LANG_ARR)
 */
void aux_q8(DATA data,REPOS repos, COMMITS commits,LANG_ARR lang_array)
{
    int i,count;
    char *lang;

    for (i=0; i<MAX_COMMITS; i++)
    {
        if (commits[i].state )  
        {   
            
            lang = searchlang(repos, commits[i].id_repo); //procura nos repos a linguagem do id_repo
            int pos= checkLANG(lang_array, lang); //pos=posição da lang ou -1 se não estiver
            
            if (pos==-1) 
            {
                add_language(lang_array, lang);
                count = ncommitsvalid(data, commits[i], commits[i].id_repo);
                pos= checkLANG(lang_array,lang);
                incrementa2(lang_array,pos,count);
            }
            else if (pos>=0) 
            {
                count = ncommitsvalid(data, commits[i], commits[i].id_repo);
                incrementa2(lang_array,pos,count);
            }
        }
    }
}





/**
 * \brief Função contabots
 * 
 * Função que diz de um repositório tem ou não bots como colaboradores
 *
 * @param tabelacommit (STRUCT COMMITS)
 */
int contabots(COMMITS tabelacommit)
{
    int contbot=0;

    for (int z=0;z<MAX_COMMITS;z++) if (tabelacommit[z].state) 
    {
        if (tabelacommit[z].bot) contbot++;
    }

    return contbot;
    
}

/**
 * \brief Função get_p
 * 
 * Função que retorna  o campo tamanho do array de comits
 *
 * @param commitaux (STRUCT COMMITS)
 */
int get_p (COMMITS commitaux)
{
    int aux =0;
    aux = commitaux->p;
    return aux ;
}


/**
 * \brief Função get_commitline
 * 
 * Função que retorna  a linha de um commit
 *
 * @param commitaux (STRUCT COMMITS)
 * @param i posição no array
 */
char* get_commitline(COMMITS commitaux,int i)
{
    char *aux ;
    aux = strdup (commitaux->commit_line[i]);
    return aux;
}


/**
 * \brief Função get_p2
 * 
 * Função que retorna  o tamanho do array de commits 
 *
 * @param commitaux (STRUCT COMMITS)
 * @param i posição no array
 */
int get_p2 (COMMITS commitaux,int i)
{
    int k = commitaux[i].p;
    return k;
}

/**
 * \brief Função get_p2
 * 
 * Função que retorna a string correpondente aa uma posição no array de commits
 *
 * @param commitaux (STRUCT COMMITS)
 * @param i posição no na tabea de hash
* @param j posição no array de commits
 */
char* get_commitline2(COMMITS commitaux,int i,int j)
{
    char *aux ;
    aux =strdup (commitaux[i].commit_line[j]);
    return aux;
}

/**
 * \brief Função get_state2
 * 
 * Função que retorna se  a posição no array está ou não ocupada
 *
 * @param tabela (STRUCT COMMITS)
 * @param i posição no na tabea de hash
 */
char get_state2(COMMITS tabela,int i)
{
    char a;
    a = (tabela[i].state);
    return a;
}

/**
 * \brief Função get_id_repo
 * 
 * Função que retorna a chave correspondente a um repoitório
 *
 * @param tabela (STRUCT COMMITS)
 * @param i posição no na tabea de hash
 */
KEY get_id_repo(COMMITS tabela,int i)
{
    KEY aux = tabela[i].id_repo;
    return aux;
}
