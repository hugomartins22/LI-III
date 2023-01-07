#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "struct.h"


/**
 * \brief Função hash
 * 
 * É a função reponsável pelo catálogo dos dados 
 * Dada uma chave diz a posição no array
 */
unsigned long hash(KEY k) 
{
  unsigned long hash = 5381;
  int c;
  while ((c = *(k++)))
      hash = ((hash << 5) + hash) + c;
  return hash;
}


/**
 * \brief Função initUsers
 * 
 * É a função reponsável por inicializar a tabela de users
 * @param table tabela de users
 */
void initUsers (USERS table)
{
	int i;
	for (i=0; i<MAX_USERS; table[i++].state = 0);
}


/**
 * \brief Função add_users
 * 
 * É a função reponsável por adicionar os users à tabela de hash
 * @param i id a adicionar
 * @param log login do user
 * @param t type do user
 * @param follower_list string que tem a lista dos seguidores
 * @param following_list string que tem a lista dos utilizadores que um user segue
 * @param table tabela de users
 * @param followers quantidade de seguidores
 * @param following qunatidade de pessoas que segue
 */
int add_users (KEY i, char* log, char* t,char* follower_list,char* following_list , USERS table,int followers,int following)
{
	unsigned long p = hash (i)%MAX_USERS;
	while (table[p].state && strcmp (table[p].id,i))
	    p = (p+1)%MAX_USERS;

	if (table[p].state) return 1;
	table[p].id=i;
	table[p].state = 1;
	table[p].login=log;
	table[p].type=t;
	table[p].follower_list=follower_list;
    table[p].following_list=following_list;
	table[p].followers=followers;
    table[p].following=following;
    table[p].repos=NULL;
return 0; 
}


/**
 * \brief Função getlogin
 * 
 * É a função reponsável por retornar o login de um user
 * @param table tabela de users
 * @param i id a procurar
 */
char* getlogin (USERS table , KEY i) 
{
	unsigned long p = hash (i)%MAX_USERS;
	while (table[p].state && strcmp (table[p].id,i))
	    p = (p+1)%MAX_USERS;
	if (table[p].state) return (table[p].login);
}


/**
 * \brief Função searchID
 * 
 * É a função reponsável por procurar se um user pertence à tabela de hash
 * @param table tabela de users
 * @param i id a procurar
 */
int searchID (KEY i,USERS table)
{
	unsigned long p = hash (i)%MAX_USERS;
	while (table[p].state && strcmp (table[p].id,i))
	    p = (p+1)%MAX_USERS;
	if (table[p].state) return 1;
	return 0;
}


/**
 * \brief Função printUSERS
 * 
 * É a função reponsável por imprimir os users 
 * @param t tabela de users
 */
void printUSERS(USERS t)
{
  int i;
  for (i=0; i<MAX_USERS; i++) if (t[i].state) 
  {
    printf("##### ID:%s\n\n ",t[i].id) ; 
    printf("LOGIN:%s\n",t[i].login); 
    printf ("FOLLOWERS :%d\n ",t[i].followers);
    printf ("FOLLOWER list   %s \n",t[i].follower_list);
    printf ("FOLLOWING %d\n",t[i].following);
    printf ("FOLLOWING list  %s \n",t[i].following_list);
    printf ("repos %s \n",t[i].repos);
  }
}


/**
 * \brief Função initCOMMITSAUX
 * 
 * É a função reponsável por inicializar a tabela de commits 
 * @param com_table tabela de commits
 */
void initCOMMITS (COMMITS com_table) 
{
	int i;
	for (i=0; i<MAX_COMMITS; com_table[i++].state = 0) com_table[i++].bot = 0;
}


/**
 * \brief Função initCOMMITSAUX
 * 
 * É a função reponsável por inicializar a tabela de commits aux
 * @param com_table tabela de commits_Aux
 */
void initCOMMITSAUX (COMMITS_AUX com_table) 
{
	int i;
	for (i=0; i<MAX_COMMITS; com_table[i++].state = 0);
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
 * \brief Função getdata
 * 
 * É a função reponsável por converter uma string para uma struct data
 * @param date string a converter
 */
DATA getdata (char*date)  
{
	DATA datafinal = malloc(sizeof(struct data));
	char *linecopy,*token;
	linecopy= strdup (date);

	int collumn=0;
	
 	while (token = strsep (&linecopy,"-"))
        {
        if (collumn ==0) datafinal->ano = atoi(token);
        if (collumn ==1) datafinal->mes = atoi(token);
        if (collumn ==2) datafinal->dia = atoi(token);
        collumn++;
        }
	return datafinal;
}


/**
 * \brief Função comparedate
 * 
 * É a função reponsável por comparar duas datas
 * @param d1 primeira data a comparar
 * @param d2 segunda data a comparar
 */
int comparedate ( DATA  d1,  DATA  d2)
{
    if (d1->ano < d2->ano)
        return 1;
    if (d1->ano == d2->ano && d1->mes < d2->mes)
        return 1;
    if (d1->ano == d2->ano && d1->mes == d2->mes &&
                              d1->dia < d2->dia)
        return 1;
  
    return 0;
}


/**
 * \brief Função printdata
 * 
 * É a função reponsável por imprimir uma data
 * @param date data a imprimir
 *
 */
void printdata (DATA date ) {
    printf ("ano %d|",date->ano );
    printf ("mes %d|",date->mes );
    printf ("dia %d|\n",date->dia );

}


/**
 * \brief Função comparedate2
 * 
 * É a função reponsável por comparar duas datas
 * @param d1 primeira data a comparar
 * @param d2ano ano
 * @param d2mes mes
 * @param d2dia dia
 *
 */
int comparedate2 ( DATA  d1,  int d2ano , int d2mes,int d2dia )
{
    if (d1->ano < d2ano)
        return 1;
    if (d1->ano == d2ano && d1->mes < d2mes)
        return 1;
    if (d1->ano == d2ano && d1->mes == d2mes &&
                              d1->dia < d2dia)
        return 1;
  
    // If none of the above cases satisfy, return false
	
    return 0;
}


/**
 * \brief Função getdescription
 * 
 * É a função reponsável para obter a descrição de um repositório
 * @param Trepos tabela de repos
 * @param id id do repositório a procurar
 *
 */
char* getdescription (REPOS Trepos,KEY id)
{
	unsigned long p = hash (id)%MAX_REPOS;
	while (Trepos[p].state && strcmp (Trepos[p].id,id))
	    p = (p+1)%MAX_REPOS;
	if (Trepos[p].state) return (Trepos[p].description);

}

/**
 * \brief Função checkbot
 * 
 * É a função reponsável por verificar se um user é ou não bot
 * @param i id a procurar
 * @param table tabela de users
 *
 */
int checkbot (KEY i ,USERS table ) {
	unsigned long p = hash (i)%MAX_USERS;
	while (table[p].state && strcmp (table[p].id,i))
	    p = (p+1)%MAX_USERS;
	if ((table[p].state) && strcmp ((table[p].type),"Bot")==0)  return 1;
	return 0;
}


/**
 * \brief Função elem
 * 
 * É a função reponsável por retornar o numero de comuns  entre o array de strings )
 * @param str string a comparar
 * @param arr array de strings
 * @param size tamanho do array
 *
 */
int elem(char* str,char* arr[],int size)
{
    int i;
    for(i=0;i<size;i++)
    {
        char aux[20];
        sprintf(aux," %s",str);
        if (strcmp(str,arr[i])==0 || strcmp(aux,arr[i])==0 ) return 1;
    }
return 0;
}


/**
 * \brief Função comuns
 * 
 * É a função reponsável por retornar os amigos de um user (estão contidos na lista following e followers)
 * @param l1 array de followers
 * @param l2 array de following
 * @param followers número de seguidores
 * @param following numero de pessoas que um user segue
 *
 */
char* comuns (char* l1, char* l2,int followers,int following)
{
    char *token,*linecopy,*linecopy2,*retorno;
    char* arrL1[followers];
    char* arrL2[following];
    linecopy= strdup (l1+1);//+1 para retirar o primeiro [
    linecopy2= strdup (l2+1);//+1 para retirar o primeiro [

    linecopy[strlen(linecopy)-1]=0;
    linecopy2[strlen(linecopy2)-1]=0;

    int i=0;
    while (token = strsep (&linecopy,","))
    {
        arrL1[i]=token;
        i++;
    }
    free(linecopy);

    i=0;
    while (token = strsep (&linecopy2,","))
    {
        arrL2[i]=token;
        i++;
    }
    free(linecopy2);

    i=0;
    int flag=0;
    retorno=malloc(strlen(l1));
    strcpy(retorno,"");

    while(i<followers)
    {
        if(elem(arrL1[i],arrL2,following)==1)
        {
            strcat(retorno,arrL1[i]); 
            strcat(retorno,",");
            flag++;
        }
        i++;
    }
    if(flag!=0) retorno[strlen(retorno)-1]=0;

    return retorno;
}


/**
 * \brief Função getFriends
 * 
 * É a função reponsável por retornar os amigos de um user
 * @param i  user a procurar os amigos
 * @param table Tabela de Users
 */
char* getFriends (KEY i,USERS table)
{
    unsigned long p = hash (i)%MAX_USERS;
    char* friends;

    while (table[p].state && strcmp (table[p].id,i))
        p = (p+1)%MAX_USERS;
    if (table[p].state)
    {
        friends=comuns(table[p].follower_list,table[p].following_list,table[p].followers,table[p].following);
        return(friends);
    }
    return NULL;
}


/**
 * \brief Função getFriendRepo
 * 
 * É a função reponsável por encontrar os repositórios de um amigo
 * @param table Tabela de Users
 * @param i  id do dono do repositório
 */
char* getFriendRepo (KEY i,USERS table)
{
    unsigned long p = hash (i)%MAX_USERS;

    while (table[p].state && strcmp (table[p].id,i))
        p = (p+1)%MAX_USERS;
    if (table[p].state)
    {
        return(table[p].repos);
    }
    return NULL;
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
 * \brief Função initREPOS
 * 
 * É a função reponsável por iniciar o parametro da taebla de hash "state" a 0 
 * @param repo_table (Tabela de repositórios
 * 
 */
void initREPOS (REPOS repo_table) 
{
	int i;
	for (i=0; i<MAX_REPOS; repo_table[i++].state = 0);
}


/**
 * \brief Função addREPOS
 * 
 * É a função reponsável por adicionar um repositório à tabela de hash
 * @param i (KEY que se pretende procurar)
 * @param repo_table (Tabela Repos)
 * @param owner (dono do repositório)
 * @param lingua (Linguagem do repositório
 * @param date (Updated at daquele repositório
 * @param descricao (descrição do repositório)
 * 
 */
int addREPOS (KEY i, REPOS repo_table,int owner,char* lingua,DATA date ,char*descricao)
{
	unsigned long p = hash (i)%MAX_REPOS;
	while (repo_table[p].state && strcmp (repo_table[p].id,i))
	    p = (p+1)%MAX_REPOS;
	if (repo_table[p].state) return 1;
	repo_table[p].id=i;
	repo_table[p].owner_id=owner;
	repo_table[p].language=lingua;
	repo_table[p].updated_at=date;
	repo_table[p].description=descricao;

	repo_table[p].state = 1;
	return 0;
}


/**
 * \brief Função addRepoToUser
 * 
 * É a função reponsável por adicionar a um user os repositórios que ele é dono , caso isto aconteça
 * @param repo_id (KEY que se pretende procurar)
 * @param owner_id (ID Do dono do repositório
 * @param tabelauser (Tabela de users)
 * 
 */
void addRepoToUser (char* repo_id,int owner_id,USERS tabelauser)
{
    char i[20];
    sprintf(i,"%d",owner_id);

    unsigned long p = hash (i)%MAX_USERS;
    while (tabelauser[p].state && strcmp (tabelauser[p].id,i))
        p = (p+1)%MAX_USERS;

    if(tabelauser[p].state)
    {
        if(tabelauser[p].repos==NULL)
        {
            tabelauser[p].repos=strdup(repo_id);
        }
        else
        {
            char* pointer =malloc(strlen(tabelauser[p].repos)+strlen(repo_id)+2);
            sprintf(pointer,"%s,%s",tabelauser[p].repos,repo_id);
            free(tabelauser[p].repos);
            tabelauser[p].repos=pointer;
        }
    }
}
	

/**
 * \brief Função searchREPOS
 * 
 * É a função reponsável por procurar um repositório existe na tabela de hash 
 * @param i (KEY que se pretende procurar)
 * @param repo_table (Tabela Repos)
 */
int searchREPOS (KEY i,REPOS repo_table)
{
	unsigned long p = hash (i)%MAX_REPOS;
	while (repo_table[p].state && strcmp (repo_table[p].id,i))
	    p = (p+1)%MAX_REPOS;
	if (repo_table[p].state) return 1;
	return 0;
}


/**
 * \brief Função printREPOS
 * 
 * É a função reponsável por dar print de todos os repositórios
 * @param r (REPOS)
 */
void printREPOS(REPOS r)
{
  int i;

  for (i=0; i<MAX_REPOS; i++) if (r[i].state)  {
  	printf("ID:%s \n",r[i].id);
	  printf ("####Ano = %d #####\n",r[i].updated_at->ano);
	  printf ("####Mes = %d #####\n",r[i].updated_at->mes);
	  printf ("####Mes = %d #####\n",r[i].updated_at->dia);
  }
}

/**
 * \brief Função printUMREPO
 * 
 * É a função reponsável por dar print de um repositório
 * @param r (REPOS)
 * 
 */
void printUMREPO(struct repos r)
{
		printf ("\nID:%s",r.id);
		printf ("\nstate= %d", r.state);
		printf ("\nlanguage= %s", r.language);
		printf ("\nstate= %d", r.state);
		printf ("\nupdated_at: ");
		printf (" Ano = %d |",r.updated_at->ano);
		printf (" Mes = %d |",r.updated_at->mes);
		printf (" Dia = %d \n",r.updated_at->dia);
		
}

/**
 * \brief Função searchlang
 * 
 * É a função reponsável por retornar a linguagem de um repositório dado o id do repositório
 * @param repos (REPOS)
 * @param id_repo (KEY)
 * 
 */
char* searchlang(REPOS repos, KEY id_repo){
    
    unsigned long p = hash (id_repo)%MAX_REPOS;
    while (repos[p].state){
        if (!strcmp (repos[p].id,id_repo)){
            return repos[p].language;
        }
        else p = (p+1)%MAX_REPOS;	
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

    DATA commit_at = malloc(sizeof(struct data));
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
 * \brief Função compare_DATAS
 * 
 * É a função reponsável por comparar a data 1 com a data 2 de acordo com o Parametro N. N define se é para ser data1>=ref(data depois de ref é N=0) ou data1<=ref(data antes de ref é N=1)
 * @param data1 (DATA)
 * @param ref (DATA)
 * @param N (int) 
 */
int compare_DATAS(DATA data1, DATA ref, int N){

	int soma1, somaref,
	data1_ano,
	data1_mes,
	data1_dia,
	dataref_ano,
	dataref_mes,
	dataref_dia;

	data1_ano= data1->ano;
	data1_mes= data1->mes;
	data1_dia= data1->dia;

	dataref_ano= ref->ano;
	dataref_mes= ref->mes;
	dataref_dia= ref->dia;

	soma1 = data1_ano*365 + ((data1_mes-1)*30) + data1_dia;
	somaref= dataref_ano*365 + ((dataref_mes-1)*30) + dataref_dia;

	//data depois de ref é 0) ou <=(data antes de ref é 1)
	if (soma1 >= somaref && N==0) return 1;
	else if (soma1 >= somaref && N==1) return 0;
	else if (soma1 <= somaref && N==0) return 0;
	else if (soma1 <= somaref && N==1) return 1;
}


/**
 * \brief Função add_language
 * 
 * É a função reponsável por adicionar uma linguagem a um array de linguagens. 
 * @param arr (LANG_ARR)
 * @param lang (char*)
 * @param pos (int)
 * 
 */
void add_language(LANG_ARR arr, char* lang, int pos){
		arr->size++;
		arr[pos].language= lang;
        arr[pos].number=0;
		arr[pos].pos=pos;
        arr[pos].size= pos+1;
        
}	
//verifica se a linguagem existe no array. (retorna a posição ou -1 se nao tiver)
/**
 * \brief Função checkLANG
 * 
 * É a função reponsável por verificar se uma linguagem pertence a um array de linguagens. Caso pertença retorna a sua posição, caso não pertença retorna -1.
 * @param arr (LANG_ARR)
 * @param size (int)
 * @param char* (lang)
 * 
 */
int checkLANG(LANG_ARR arr, int size, char* lang){

	for(int i=0; i<size; i++){
		if (!strcmp(arr[i].language, lang)) return i;
	}
	return -1;
}
/**
 * \brief Função searchIDREPOSlang
 * 
 * É a função reponsável por criar uma lista de ids de repositórios de uma linguagem.
 * @param repos (REPOS)
 * @param array_ids[] (int )
 * @param lang (char*)
 * 
 */
void searchIDREPOSlang(REPOS repos, int array_ids[], char* lang){
	char* token;
	int k=0, i=0;
	for (int i=0; i<MAX_REPOS; i++) if(repos[i].state){
		if (!strcasecmp(repos[i].language, lang)) {
			token=repos[i].id;
			array_ids[k]= atoi(token);
			k++;
		}
	}
	array_ids[k]=-12345;
}

/**
 * \brief Função merge
 * 
 * Função auxiliar da mergeSORT
 * @param arr (AUX10)
 * @param l  (int)
 * @param m (int)
 * @param r (int)
 */
void merge(AUX10 arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    AUX10 L=malloc(sizeof(struct aux10)*(n1)), R=malloc(sizeof(struct aux10)*(n2));
  
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0; 
    k = l; 

    while (i < n1 && j < n2) {
        if (L[i].size <= R[j].size) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
  
/**
 * \brief Função mergeSort
 * 
 * Função responsável pela organização de uma lista
 * 
 * @param arr (AUX10)
 * @param l (int)
 * @param r (int)
 */
void mergeSort(AUX10 arr, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
  
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
  
        merge(arr, l, m, r);
    }
}

void reverseArray(AUX10 arr, int start, int end)
{
    struct aux10 temp;
    while (start < end)
    {
        temp = arr[start];  
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }  
} 

/**
 * \brief Função checkID
 * 
 * Função auxiliar da mergeSORT
 * @param ids_array (IDS_ARR)
 * @param size (int)
 * @param id (KEY)
 */
int checkID(IDS_ARR ids_array, int size, KEY id){
	
	for(int i=0; i<size; i++){
		if (!strcmp(ids_array[i].id, id)) return i;
	}
	return -1;
}

/**
 * \brief Função findCOMMIT
 * 
 * Função responsável por retornar um commit dado o id do repositório
 * @param commits (COMMITS)
 * @param id (KEY)
 */
COMMITS findCOMMIT(COMMITS commits, KEY id){

	unsigned long p = hash (id)%MAX_COMMITS;
    while (commits[p].state){
        if (!strcmp (commits[p].id_repo,id)){
			return commits+p;
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
        if (collumn == 1) return token;
        collumn++;
    }
}

/**
 * \brief Função add_AUTHORID
 * 
 * Função reponsável por adicionar um author_id a uma lista de ids.
 * @param ids_array (IDS_ARR)
 * @param author_id  (char*)
 * @param pos (int)
 */
void add_AUTHORID(IDS_ARR ids_array,char* author_id,int pos){


	ids_array->size++;
	ids_array[pos].id= author_id;
    ids_array[pos].number=1;
	ids_array[pos].pos=pos;
    ids_array[pos].size= pos+1;
}

/**
 * \brief Função extract_commit_at_date
 * 
 * Função responsável por retornar a data de um commit. 
 * @param commit_line(char*)
 */
char* extract_commit_at_date(char* commit_line){
	char *linecopy, *token;
	char *tokencopy=malloc(sizeof(char)*11);
	int collumn=0;
    linecopy= strdup (commit_line);
    
    while (token = strsep (&linecopy,";")){
		if (collumn == 3) {
			for(int i=0; i<10; i++) tokencopy[i]=token[i];
			tokencopy[10]='\0';
			return tokencopy;
		}
        collumn++;
    }

}


/**
 * \brief Função merge_LANG
 * 
 * Função auxiliar da mergeSORT_Lang.
 * @param arr (LANG_ARR)
 * @param l  (int)
 * @param m (int)
 * @param r (int)
 */
void merge_Lang(LANG_ARR arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    LANG_ARR L=malloc(sizeof(struct lang_arr)*(n1)), R=malloc(sizeof(struct lang_arr)*(n2));
  
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0; 
    k = l; 

    while (i < n1 && j < n2) {
        if (L[i].number >= R[j].number) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/**
 * \brief Função mergeSort_Lang
 * 
 * Função responsável pela organização de uma lista de linguagens de modo decrescente.
 * 
 * @param arr (LANG_ARR)
 * @param l (int)
 * @param r (int)
 */
void mergeSort_Lang(LANG_ARR arr, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
  
        mergeSort_Lang(arr, l, m);
        mergeSort_Lang(arr, m + 1, r);
  
        merge_Lang(arr, l, m, r);
    }
}


/**
 * \brief Função merge_IDS
 * 
 * Função auxiliar da mergeSORT_IDS
 * @param arr (IDS_ARR)
 * @param l  (int)
 * @param m (int)
 * @param r (int)
 */
void merge_IDS(IDS_ARR arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    IDS_ARR L=malloc(sizeof(struct ids_arr)*(n1)), R=malloc(sizeof(struct ids_arr)*(n2));
  
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0; 
    k = l; 

    while (i < n1 && j < n2) {
        if (L[i].number >= R[j].number) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/**
 * \brief Função mergeSort_IDS
 * 
 * Função responsável pela organização de uma lista de modo decrescente.
 * 
 * @param arr (IDS_ARR)
 * @param l (int)
 * @param r (int)
 */
void mergeSort_IDS(IDS_ARR arr, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
  
        mergeSort_IDS(arr, l, m);
        mergeSort_IDS(arr, m + 1, r);
  
        merge_IDS(arr, l, m, r);
    }
}
