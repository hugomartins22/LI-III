#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include <math.h>
#include "data.h"
#include "lang.h"
#include "users.h"
#include "repos.h"
#include "commits.h"
#include "generic.h"


struct aux10
{
	KEY author_id;
	int size; 

};

struct ids_arr
{
	KEY id;
    char state;
	int number;
};


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


IDS_ARR initIDS_ARR (int size)
{
    IDS_ARR ids_arr=malloc(size*sizeof(struct ids_arr));
	int i;
	for (i=0; i<size; ids_arr[i++].state = 0);

    return ids_arr;
}


void freeIDS_ARR (IDS_ARR arr, int size){

    for (int i=0; i<size; i++){
        free(arr[i].id);
    }
    free(arr);
}



int checkID(IDS_ARR ids_arr ,KEY i, int size)
{
	unsigned long p = hash (i)%size;
	while (ids_arr[p].state && strcmp(ids_arr[p].id,i))
	    p = (p+1)%size;

    if(ids_arr[p].state) return p;
    else return -1;
}


int add_id_array (IDS_ARR ids_arr, KEY i, int size)
{
	unsigned long p = hash (i)%size;
	while (ids_arr[p].state && strcmp(ids_arr[p].id,i))
	    p = (p+1)%size;

    if (ids_arr[p].state) return 1;

	ids_arr[p].id= i;
	ids_arr[p].state = 1;
    ids_arr[p].number=1;
}


void incrementa_id_array (IDS_ARR ids_arr, int p)
{
    if (ids_arr[p].state) ids_arr[p].number++;
}


IDS_ARR hash2list(IDS_ARR ids_arr, int size, int* sizelist){

    IDS_ARR list = malloc (sizeof(struct ids_arr)* size);
    int j=0;


    for(int i=0; i<size; i++) if (ids_arr[i].state){
        list[j].id=ids_arr[i].id;
        list[j].state=1;
        list[j].number=ids_arr[i].number;
        j++;
    }

    *sizelist= j;
    return list;
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



/**
 * \brief Função add_AUTHORID
 * 
 * Função reponsável por adicionar um author_id a uma lista de ids.
 * @param ids_array (IDS_ARR)
 * @param author_id  (char*)
 * @param pos (int)
 */
/*
void add_AUTHORID(IDS_ARR ids_array,char* author_id)
{
 int pos=ids_array->size;

	ids_array->size++;
	ids_array[pos].id= author_id;
    ids_array[pos].number=1;
	ids_array[pos].pos=pos;
    ids_array[pos].size= pos+1;
}



IDS_ARR incrementa(IDS_ARR author_ids_array,int pos)
{
    author_ids_array[pos].number++;
    return author_ids_array;
}
*/



/**
 * \brief Função print_q9
 * 
 * Função responsável por colocar o resultado das querie 9 em ficheiros
 * 
 * @param N correspondente ao número a colocar no ficheiro de output
 * @param tuser (STRUCT USERS)
 * @param arr (STRUCT AUX10)
 * @param q inteiro
 * @param j  inteiro
 */
void print_q9(int N,USERS tUser,AUX10 arr,int q,int j)
{
    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",q);
    FILE *f = fopen (exitfile,"w");

    mergeSort (arr,0,j-1);

    for (int k=j-1;N>0;N--)
    {
        if (arr[k].author_id)
        {

            fprintf (f,"%s;",arr[k].author_id);
            fprintf (f,"%s\n",getlogin (tUser,arr[k].author_id));
        }
        k--;
    }
    fclose(f);
}


/**
 * \brief Função init_aux10
 * 
 * Função responsável por inicializar o array
 * 
 * @param size tamanho
 */
AUX10 init_aux10 (int size)
{
    AUX10 arr = malloc(sizeof(struct aux10)*size);
    return arr;
}


/**
 * \brief Função insere_id
 * 
 * Função responsável por inicializar o array
 * 
 * @param arr STRUCT AUX10
 * @param j inteiro
 * @param tUser_id chave
 * @param c key
 */
AUX10 insere_id(AUX10 arr,int j,KEY tUser_id,int c)
{
    arr[j].author_id=tUser_id;
    arr[j].size+=c;
    return arr;
}


/**
 * \brief Função print_msg
 * 
 * Função responsável por imprimir a mensagem num ficheiro
 * 
 * @param arr (STRUCT AUX10)
 * @param k inteiro
 * @param tabelauser STRUCT USERS
 * @param id_repo chave
 * @param f apontador para o ficheiro
 */
void print_msg(AUX10 arr,int k,USERS tabelauser,KEY id_repo,FILE *f)
{
    fprintf(f, "%s;%s;%d;%s\n", arr[k].author_id,getlogin (tabelauser,arr[k].author_id ),arr[k].size,id_repo);
}


/**
 * \brief Função free_arr
 * 
 * Função responsável por libertar o array auxiliar aux10
 * 
 * @param ARR (STRUCT AUX10)
 */
void free_arr(AUX10 arr)
{
    for (int k=0;k<MAX_COMMITS;k++)
    {
        free(arr[k].author_id);
    }
    free(arr);
}


/**
 * \brief Função aux_q9
 * 
 * Função responsável por auxiliar na resolução da querie 9
 * 
 * @param tUser (STRUCT USER)
 * @param tCom (STRUCT COMMITS)
 * @param arr (STRUCT AUX10)
 * @param pos apontador para inteiro
 */
AUX10 aux_q9 (USERS tUser,COMMITS tCom,AUX10 arr,int *pos)
{
    int i,c,j=0;
    char *token,*token2,*friends,*linecopy,*linecopy2,*friend_repos;

    for (i=0; i<MAX_USERS; i++) 
    {

        if (get_state(tUser,i))
        {

            friends=getFriends(get_id(tUser,i),tUser); //friends do user id 
            linecopy= strdup (friends); //

            while (token = strsep (&linecopy,","))
            {   
                friend_repos=getFriendRepo(token,tUser);
                if(friend_repos)
                {
                    linecopy2= strdup (friend_repos);

                    while(token2 = strsep (&linecopy2,","))
                    {

                        if (get_id(tUser,i))
                        {
                            c=cont_commits(tCom,token2 ,get_id(tUser,i));
                            arr=insere_id(arr,j,get_id(tUser,i),c);

                        }
                    }

                }
            }
            j++;
        }
    }
    *pos=j;
return arr;
}




/**
 * \brief Função aux_q10
 * 
 * Função responsável por auxiliar na resolução da querie 10
 * 
 * @param tabela (STRUCT COMMITS)
 * @param top (inteiro
 * @param tabelauser  (STRUCT USERS)
 * @param f apontador para o ficheiro
 * @param arr (STRUCT AUX10)
 */
void aux_q10(COMMITS tabela,int top,USERS tabelauser,FILE *f,AUX10 arr)
{
    char*token , *linecopy ,*id,*msg;
    int collumn=0,i,h=0;


    for (i=0; i<MAX_COMMITS; i++)
    if (get_state2(tabela,i)) 
    { 
        h= get_p2(tabela,i);
        for (int j=0 ;j<get_p2(tabela,i) && get_commitline2(tabela,i,j);j++)
        {
            collumn=0;
            linecopy= strdup (get_commitline2(tabela,i,j));
            while (token = strsep (&linecopy,";"))
            {
                if (collumn == 1) {id = strdup(token);}
                if (collumn == 4) {msg = strdup(token);}
                collumn++;
            }
            arr=insere_id(arr,j,id,strlen(msg)-1);      
        }
        mergeSort (arr,0,h-1);
        reverseArray(arr,0,h-1);   

        for (int k=0;k<top&& k<get_p2(tabela,i);k++) 
        {
         print_msg( arr, k, tabelauser, get_id_repo(tabela,i),f);
        }
    }
}


/**
 * \brief Função aux_q6
 * 
 * Função responsável por auxiliar na resolução da querie 6
 * 
 * @param commitaux (STRUCT COMMITS)
 * @param author_ids_array (STRUCT IDS_ARR)
 * @param array_id_repos apontador para inteiro
 * @param commits_table (STRUCT COMMITS)
 */
IDS_ARR aux_q6 (COMMITS commitaux,IDS_ARR author_ids_array,int* array_id_repos,COMMITS commits_table, int size)
{
    KEY id_str= malloc(10); 
    int pos;
    char* author_id;

        for(int i=0;array_id_repos[i]!=-12345; i++)
        {
            sprintf(id_str, "%d", array_id_repos[i]);
            commitaux= findCOMMIT(commits_table,id_str);

            for (int k=0; k<get_p(commitaux) && get_commitline(commitaux,k);k++)
            {

                author_id= extract_author_id(get_commitline(commitaux,k));
                pos = checkID(author_ids_array,author_id, size);

                if (pos==-1)
                {
                add_id_array(author_ids_array,author_id, size);
                }
                else if (pos>=0)
                {
                incrementa_id_array(author_ids_array,pos);
                }
            }
        }
    return author_ids_array; 
}




/**
 * \brief Função aux_q5
 * 
 * Função responsável por auxiliar na resolução da querie 5
 * 
 * @param commits (STRUCT COMMITS)
 * @param author_ids_array (STRUCT IDS_ARR)
 * @param data1  (STRUCT DATA)
 * @param data2 (STRUCT DATA)
 * @param commit_at (STRUCT DATA)
 */
void auxq5(COMMITS commits, IDS_ARR author_ids_array,DATA data1, DATA data2,DATA commit_at, int size)
{
	
    int i,k,pos;
    char *author_id,*data_commit;

        for(i=0; i<MAX_COMMITS; i++) if(get_state2(commits,i))
		{
            for (k=0; k<get_p2(commits,i) && get_commitline2(commits,i,k); k++)
            {


                data_commit=extract_commit_at_date(get_commitline2(commits,i,k));
                commit_at=getdata(data_commit); 
                free(data_commit);
                data_commit=NULL;

                if (compare_DATAS(commit_at,data1,0) && compare_DATAS(commit_at,data2,1)){
                    
                    author_id= extract_author_id(get_commitline2(commits,i,k));
                    pos= checkID(author_ids_array,author_id, size);
                    
                    if (pos==-1){
                        add_id_array(author_ids_array,author_id, size);
                    }
                    else if (pos>=0)
					{
                        incrementa_id_array(author_ids_array,pos);
                    }
                }
            }
        }
}



/**
 * \brief Função print_Q
 * 
 * Função responsável por colocar o resultado das queries em ficheiros
 * 
 * @param author_ids_array (STRUCT DATA)
 * @param users (STRUCT USERS)
 * @param top  
 * @param N correspondente ao número a colocar no ficheiro de output
 */
void print_Q (IDS_ARR author_ids_array,USERS users,int top,int N, int size)
{
    char *exitfile=malloc(100*sizeof(char));    
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w");

    int sizelist;
    IDS_ARR list = hash2list(author_ids_array, size, &sizelist);

    mergeSort_IDS(list,0,sizelist);

    for (int i=0; i<top && i<sizelist; i++)
    {
        fprintf(f,"%s;", list[i].id);
        fprintf (f,"%s;",getlogin(users,list[i].id));
        fprintf(f,"%d\n",list[i].number);
	}

    fclose(f);
    free(exitfile);
}
