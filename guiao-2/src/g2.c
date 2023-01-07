#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "struct.h"


/**
 * \brief Função query1
 * 
 * É uma função cujo objetivo é retornar o número de bots, orgnizations e users totais 
 * Necessita de estatísticas que estão contempladas na struct STATS (bot,user e organization)
 *
 * @param stats1 (STRUCT STATS)
 * @param N número do ficheiro de output
 */
void query1 (STATS stats1,int N)
{
    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w");

    fprintf(f, "Bot: %d\nOrganization: %d\nUser: %d\n", stats1->bot,stats1->organization,stats1->user);
    fclose(f);
    free(exitfile);
}


/**
 * \brief Função query2
 * 
 * É uma função cujo objetivo é retornar a média entre o número total de colaboradores e o número total de repositórios 
 * Necessita de estatísticas que estão contempladas na struct STATS (colaboradores e repos)
 *
 * @param stats1 (STRUCT STATS)
 * @param N número do ficheiro de output
 */
void query2 (STATS stats1,int N)
{
    float a ;
    a = ((float)stats1->colaboradores/(float)stats1->repos);
    
    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w");

    fprintf(f, "%.2f\n", a);
    fclose(f);
    free(exitfile);
}


/**
 * \brief Função query3
 * 
 * É uma função cujo objetivo é retornar o número de repositórios que possuem bots como colaboradores 
 * Necessita de estatísticas que estão contempladas na struct STATS (bots_colab)
 *
 * @param stats1 (STRUCT STATS)
 * @param N número do ficheiro de output
 */
void query3 (STATS stats1,int N)
{
    
    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w");

    fprintf(f, "%d\n", stats1->bots_colab);
    fclose(f);
    free(exitfile);
}


/**
 * \brief Função query4
 * 
 * É uma função cujo objetivo é retornar uma média entre o número total de commits e o numero total de utilizadores 
 * Necessita de estatísticas que estão contempladas na struct STATS ()
 *
 * @param stats1 (STRUCT STATS)
 * @param N número do ficheiro de output
 */
void query4(STATS stats1,int N)
{
    float a ;
    a = (float)stats1->commits/(float)stats1->utilizadores;

    char *exitfile=malloc(100*sizeof(char));    
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w");

    fprintf(f, "%.2f\n ", a);
    fclose(f);
    free(exitfile);
}


/**
 * \brief Função query5
 * 
 * É uma função cujo objetivo é retornar o top N de utilizadores mais ativos num determinado intervalo de datas
 *
 * @param users (STRUCT USERS)
 * @param commits (STRUCT COMMITS)
 * @param data1 (STRUCT DATA) (data inicio)
 * @param data2 (STRUCT DATA) (data fim)
 * @param top número de utilizadores a retornar
 * @param N número do ficheiro de output
 */
void query5(USERS users, COMMITS commits, DATA data1, DATA data2, int top,int N)
{

    IDS_ARR author_ids_array= malloc(sizeof(struct ids_arr)*(MAX_USERS)); //guardar os ids todos num array
    DATA commit_at=malloc(sizeof(struct data));
    char *author_id, *data_commit;
    int pos,i,k;

    author_ids_array->id = "00000000";
    author_ids_array->number=0;
    author_ids_array->pos=0;
    author_ids_array->size=1;
    
    char *exitfile=malloc(100*sizeof(char));    
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w"); 
    
    if(!compare_DATAS(data1, data2,1)) printf("ERROR MESSAGE \ndata_inicio é depois da data_fim");
    
    else 
    {
        for(i=0; i<MAX_COMMITS; i++) if(commits[i].state){
            for (k=0; k<commits[i].p && commits[i].commit_line[k]; k++)
            {

                data_commit=extract_commit_at_date(commits[i].commit_line[k]);
                commit_at=getdata(data_commit); 
                
                if (compare_DATAS(commit_at,data1,0) && compare_DATAS(commit_at,data2,1)){

                    author_id= extract_author_id(commits[i].commit_line[k]);
                    pos= checkID(author_ids_array, author_ids_array->size,author_id);
            
                    if (pos==-1){
                        add_AUTHORID(author_ids_array,author_id,author_ids_array->size);
                    }
                    else if (pos>=0){
                        author_ids_array[pos].number++;
                    }
                }
            }
        }
    }
    int siz = author_ids_array->size-1;
    mergeSort_IDS(author_ids_array,0,author_ids_array->size-1);
    for (int i=0; i<top && i<siz; i++)
    {
        fprintf(f,"%s;", author_ids_array[i].id);
        fprintf (f,"%s;",getlogin(users,author_ids_array[i].id));
        fprintf(f,"%d\n",author_ids_array[i].number);
	}
    fclose(f);
    free(exitfile);
}


/**
 * \brief Função query6
 * 
 * É uma função cujo objetivo é retornar o Top N (indicado por parâmetro) de utilizadores presentes em
 * commits de uma determinada linguagem (indicada por parâmetro)
 *
 * @param users (STRUCT USERS)
 * @param repos (STRUCT REPOS)
 * @param commits_table (STRUCT COMMITS)
 * @param lang (linguagem a ser tratada)
 * @param n (número de users a retornar)
 * @param N número do ficheiro de output
 */
void query6(USERS users, REPOS repos, COMMITS commits_table, int n, char* lang,int N)
{
    
    int *array_id_repos=malloc(sizeof(int)*MAX_REPOS/8);
    IDS_ARR author_ids_array= malloc(sizeof(struct ids_arr)*(20000));
    COMMITS commitaux=malloc(sizeof(struct commits));

    char *exitfile=malloc(100*sizeof(char));    
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w"); 
    
    searchIDREPOSlang(repos,array_id_repos,lang); //lista de ids de repos com a linguage
    

    author_ids_array->id = "00000000";
    author_ids_array->number=0;
    author_ids_array->pos=0;
    author_ids_array->size=1;
    
    int pos;
    KEY id_str= malloc(10); //possivel problema com memoria
    char* linecopy, *author_id;

    for(int i=0;array_id_repos[i]!=-12345; i++)
    {
        sprintf(id_str, "%d", array_id_repos[i]);
        commitaux= findCOMMIT(commits_table,id_str);

        for (int k=0; k<commitaux->p && commitaux->commit_line[k];k++)
        {

            author_id= extract_author_id(commitaux->commit_line[k]);
            pos= checkID(author_ids_array, author_ids_array->size,author_id);//ta errado , não é id_repo mas sim o id_owner em cda commitline

            if (pos==-1)
            {
                add_AUTHORID(author_ids_array,author_id,author_ids_array->size);
            }
            else if (pos>=0)
            {
                author_ids_array[pos].number++;
            }
        }
    }
    int siz = author_ids_array->size;
    mergeSort_IDS(author_ids_array,0,author_ids_array->size);
    
     for (int i=0; i<n && i<siz; i++)
    {
        fprintf(f,"%s;", author_ids_array[i].id);
        fprintf (f,"%s;",getlogin(users,author_ids_array[i].id));
        fprintf(f,"%d\n",author_ids_array[i].number);
	}
    fclose(f);
    free(exitfile);
}


/**
 * \brief Função query7
 * 
 * É uma função cujo objetivo é retornar a lista de repositórios sem commits a partir de uma determinada
 *data (indicada por parâmetro).
 *
 * @param Tcommits (STRUCT COMMITS)
 * @param Trepos (STRUCT REPOS)
 * @param cAno (Ano a comparar)
 * @param cMes (Mes a comparar)
 * @param cDia (Dia a comparar)
 * @param N número do ficheiro de output
 */
void query7(COMMITS Tcommits,REPOS Trepos,int cAno ,int cMes,int cDia,int N)
{
    int i,j,c=0,collumn;
    char*token , *linecopy;

    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w");
 
    for (i=0; i<MAX_COMMITS; i++) if (Tcommits[i].state) 
    { 
    DATA recente = malloc (sizeof(struct data));
    DATA date = malloc (sizeof(struct data));

        recente->ano=0;
        recente->mes=0;
        recente->dia=0;
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
                recente->ano = date->ano ;
                recente->mes = date->mes ;
                recente->dia = date->dia ;
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
 * \brief Função query8
 * 
 * É uma função cujo objetivo é retornar o top N de linguagens presentes com mais frequência em
 * repositórios a partir de uma determinada data.
 *
 * @param repos (STRUCT REPOS)
 * @param commits (STRUCT COMMITS)
 * @param top (Número de linguagens
 * @param data (Data a comparar)
 * @param N (número do ficheiro de output)
 */
void query8 (REPOS repos, COMMITS commits, int top, DATA data,int N)
{
    LANG_ARR lang_array = malloc(sizeof(struct lang_arr)*(300)); 
    int i, j=0;
    char *lang;

    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w");

    lang_array->language = "Python";
    lang_array->number=0;
    lang_array->pos=0;
    lang_array->size=1;

    int count;
    for (i=0; i<MAX_COMMITS; i++)
    {
        if (commits[i].state )  
        {   
            
            lang = searchlang(repos, commits[i].id_repo); //procura nos repos a linguagem do id_repo
            int pos= checkLANG(lang_array, lang_array->size, lang); //pos=posição da lang ou -1 se não estiver
            
            if (pos==-1) 
            {
                add_language(lang_array, lang,lang_array->size);
                count = ncommitsvalid(data, commits[i], commits[i].id_repo);
                pos= checkLANG(lang_array, lang_array->size, lang);
                lang_array[pos].number+=count;
            }
            else if (pos>=0) 
            {
                count = ncommitsvalid(data, commits[i], commits[i].id_repo);
                lang_array[pos].number+=count;
            }
        }
    }
    int siz = lang_array->size-1;
    mergeSort_Lang(lang_array,0,lang_array->size-1);
    
    for (int i=0; i<top && i<siz; i++)
    {
        fprintf(f,"%s\n", lang_array[i].language);
	}
    fclose(f);
}



/**
 * \brief Função query9
 * 
 * É uma função cujo objetivo é retornar o top  N (indicado por parâmetro) de utilizadores com mais commits
em repositórios cujo owner está contido na sua lista de followers e following.
 *
 * @param tUser (STRUCT REPOS)
 * @param tCom (STRUCT COMMITS)
 * @param tRepo (STRUCT REPOS)
 * @param N (número do ficheiro de output)
 * @param totalusers (Número total de users)
 */
void query9(USERS tUser,COMMITS tCom,REPOS tRepo,int N,int totalusers,int q)
{
    int i,j=0,h,cont=0,c;
    char *repo_id,*friends,*token,*linecopy,*friend_repos,*token2,*linecopy2;

    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",q );
    FILE *f = fopen (exitfile,"w");

    AUX10 arr = malloc(sizeof(struct aux10)*totalusers);

    for (i=0; i<MAX_USERS; i++) 
    {
        if (tUser[i].state)
        {
            friends=getFriends(tUser[i].id,tUser); 
            linecopy= strdup (friends);

            while (token = strsep (&linecopy,","))
            {
                friend_repos=getFriendRepo(token,tUser);
                if(friend_repos)
                {
                    linecopy2= strdup (friend_repos);

                    while(token2 = strsep (&linecopy2,","))
                    {

                        if (tUser[i].id)
                        {
                            c=cont_commits(tCom,token2 ,tUser[i].id);
                            arr[j].author_id=tUser[i].id;
                            arr[j].size+=c;
                        }
                    }

                }
            }
            j++;
        }
    }

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
 * \brief Função query10
 * 
 * É uma função cujo objetivo é retornar o top N de mensagens maiores de cada repositório
 *
 * @param tabela (STRUCT COMMITS)
 * @param top n de mensagens
 * @param tabelauser (STRUCT USERS)
 * @param N numero do ficheiro de output
 */
void query10(COMMITS tabela,int top,USERS tabelauser,int N)
{
    char*token , *linecopy ,*login ,*id,*msg;
    int collumn=0,i,size=0,h=0;
    AUX10 arr = malloc(sizeof(struct aux10)*MAX_COMMITS); 

    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",N );
    FILE *f = fopen (exitfile,"w");

    for (i=0; i<MAX_COMMITS; i++) if (tabela[i].state) 
    { 
        h= tabela[i].p;
        for (int j=0 ;j<tabela[i].p && tabela[i].commit_line[j];j++)
        {
            collumn=0;
            linecopy= strdup (tabela[i].commit_line[j]);
            while (token = strsep (&linecopy,";"))
            {
                if (collumn == 1) {id = strdup(token);}
                if (collumn == 4) {msg = strdup(token);}
                collumn++;
            }

            arr[j].author_id= id; 
            arr[j].size=strlen(msg) -1;         
        }
        mergeSort (arr,0,h-1);
        reverseArray(arr,0,h-1);   

        for (int k=0;k<top&& k<tabela[i].p;k++) 
        {
         fprintf(f, "%s;%s;%d;%s\n", arr[k].author_id,getlogin (tabelauser,arr[k].author_id ),arr[k].size,tabela[i].id_repo);
        }
    }
    fclose(f);
    free(exitfile);
    for (int k=0;k<MAX_COMMITS;k++)
    {
        free(arr[k].author_id);
    }
    free(arr);
}

/**
 * \brief Função removenewlineFromString
 * 
 * É uma função cujo objetivo é remover os "\n" da string
 *
 * @param str (apontador para uma string )
 * @returns um apontador para a string 
 */
const char *removenewlineFromString(char *str)
{
    int i,j;
    i = 0;
    while(i<strlen(str))
    {
        if (str[i]=='\n')
        {
            for (j=i; j<strlen(str); j++)
                str[j]=str[j+1];
        }
        else i++;
    }
    return str;
}



/**
 * \brief Função readfile
 * 
 * É uma função cujo objetivo é ler as instruções do ficheiro objeto e direcionar para cada uma das querys
 
 * @param filepath caminho para o ficheiro objeto
 * @param stats1 (STRUCT STATS)
 * @param tabelacommit (STRUCT COMMITS)
 * @param tabelarepo (STRUCT REPOS);
 * @param tabelauser (STRUCT USER);
 */
void readfile (char* filepath,STATS stats1,COMMITS tabelacommit ,REPOS tabelarepo ,USERS tabelauser ) 
{

    char*line=malloc(MAX_SIZE);
    char*linecopy=malloc(MAX_SIZE);
    char*linecopy2=malloc(MAX_SIZE);
    long collumn=0;
    char *parameter,*a,*b,*c,*d,*token; 

    FILE *file = fopen(filepath, "r");
   
    for (int n=1;(fgets(line,MAX_SIZE,file));n++)
    {
        collumn =0;
        linecopy = strdup (line);
        linecopy2 = strdup (line);
        {
            while (token = strsep (&linecopy," "))
            {
                if (collumn ==0) {
                    a=strdup (removenewlineFromString(token));
                    if (strcmp (a,"1")==0){query1(stats1,n); break;}
                    if (strcmp (a,"2")==0){query2(stats1,n); break;}
                    if (strcmp (a,"3")==0){query3(stats1,n); break;}
                    if (strcmp (a,"4")==0){query4(stats1,n); break;}
                }
                if (collumn==1) 
                {
                    b=strdup (removenewlineFromString(token));
                    if(strcmp (a,"7")==0) 
                        {
                            DATA date =malloc(sizeof(struct data));
                            date =getdata(b);
                            query7(tabelacommit,tabelarepo,date->ano,date->mes,date->dia,n);
                            break;
                         }
                    if(strcmp (a,"9")==0) { query9(tabelauser,tabelacommit,tabelarepo,atoi(b),(stats1->utilizadores),n);break;}
                    if(strcmp (a,"10")==0) { query10 (tabelacommit,atoi(b),tabelauser,n);break;}     
                }
                if (collumn==2) 
                {
                    c=strdup (removenewlineFromString(token));
                    if(strcmp (a,"6")==0) {query6(tabelauser,tabelarepo,tabelacommit,atoi(b),c,n);break;}
                    if(strcmp (a,"8")==0) { query8(tabelarepo,tabelacommit,atoi(b),getdata(c),n);break;}
                }
                if (collumn==3)
                {
                    d=strdup (removenewlineFromString(token));
                    if(strcmp (a,"5")==0) { query5(tabelauser,tabelacommit,getdata(c),getdata(d),atoi(b),n);break;}
                }
                collumn++;
            }
         } 
    }
}

/**
 * \brief Função main 
 * 
 * É a função reponsável pelo catálogo dos dados 
 * É responsável pela leitura dos dados de cada um dos ficheiros csv e pelo catálogo deles em hash tables
 *
 *
 */
int main (int n,char**filepath) 
{
    char *linecopy,*line,*token,*user_login,*user_type,*user_id,*repo_id ,*linecopy2,*author_id,*linecopy3,*token2,*autorcommit,*description,*com_id,*commiter,*user_follower_list,*user_following_list,*user_followers,*user_following;
    char *language,*message ,*commiter_id;
    int collumn ,owner_id,followers,following;
    int colaboradores=0,c=0,k=0,contrepos=0,a=0,b=0,m=0 ,l=0,bot=0,organ=0,user=0,contbot=0;
    int row =0,totalcommits=0,totalusers=0,aux=0,result_bot1 =0,N=2,result_bot2=0,result_bot=0;


    line=malloc(MAX_SIZE);
    description = malloc(MAX_SIZE);


    //cria as tabelas
    USERS tabelauser=malloc(MAX_USERS*sizeof(struct users));
    COMMITS tabelacommit=malloc(MAX_COMMITS*sizeof(struct commits));
    REPOS tabelarepo=malloc(MAX_REPOS*sizeof(struct repos));
    COMMITS_AUX tabelaCommitAux =malloc(MAX_COMMITS*sizeof(struct commits_aux));
   
    initUsers(tabelauser); 
    initCOMMITS (tabelacommit);
    initREPOS (tabelarepo);
    initCOMMITSAUX(tabelaCommitAux);

    STATS stats1 = malloc(sizeof (struct stats));
    
    FILE *users = fopen("./entrada/users-g2.csv", "r");  

    while (fgets(line,MAX_SIZE, users)) 
    {
        collumn =0;
        linecopy = strdup(line);
        if (row != 0) 
        {
            while (token = strsep (&linecopy,";"))
            {
                if (collumn ==0) user_id = strdup(token);
                if (collumn ==1) user_login = strdup(token);
                if (collumn ==2) 
                {
                    user_type= strdup(token);
                    if (strcmp(user_type,"Bot")==0) bot++;
                    if (strcmp(user_type,"User")==0) user++;
                    if (strcmp(user_type,"Organization")==0) organ++;
                    
                }
                if (collumn ==4) followers= atoi(token);
                if (collumn ==6) following= atoi(token);
                if (collumn ==5) user_follower_list= strdup(token);
                if (collumn ==7) user_following_list= strdup(token);
                collumn++;

            }
        add_users(user_id,user_login,user_type,user_follower_list,user_following_list,tabelauser,followers,following);
        totalusers++;
        }
        row++;
    }
    
    stats1->bot=bot;
    stats1->user=user;
    stats1->organization =organ;
    stats1->utilizadores = totalusers;
    fclose(users);

    row =0;
    DATA updated ;
    updated = malloc(sizeof(struct data));

    FILE *repos = fopen("./entrada/repos-g2.csv", "r");

    while (fgets(line, MAX_SIZE, repos)) 
    {
        collumn =0;
        linecopy = strdup (line);
        if (row !=0)
        {
            while (token = strsep (&linecopy,";"))
            {
                if (collumn ==0) repo_id = token;
                if (collumn ==1) owner_id = atoi(token);
                if (collumn ==5) description = token;
                if (collumn ==6) language = token;
                if (collumn ==9) updated = getdata (token); 
                collumn++;
        }
        addREPOS(repo_id,tabelarepo, owner_id, language,updated,description );
        addRepoToUser(repo_id,owner_id,tabelauser);

        contrepos++;
        }
        row ++;
    }
    stats1->repos = contrepos;
    fclose(repos);

    row =0;


    FILE *commits = fopen("./entrada/commits-g2.csv", "r");
    while (fgets(line,MAX_SIZE, commits)) 
    {
        collumn =0;
        aux=0;
        linecopy = strdup (line);
        linecopy2 = strdup (line);
        linecopy3 =strdup (line);


        if (row !=0)
        {
         
            while (token2 = strsep (&linecopy3,";"))
            {
                if (aux ==1) autorcommit = token2;
                if (aux ==2) commiter = token2;
                aux++;
            }
            
            result_bot1 = checkbot(autorcommit,tabelauser);
            result_bot2 = checkbot(commiter,tabelauser);
             

            while (token = strsep (&linecopy,";"))
            {
                if (collumn ==0) repo_id = token;
                collumn++;
            }

            if ((result_bot1 == 1) ||(result_bot2 == 1) ) result_bot =1;
            else result_bot=0;

            addCOMMITSM(repo_id,tabelacommit,linecopy2,result_bot);
            totalcommits++;
        }      
            row++;
    }

    for (int z=0;z<MAX_COMMITS;z++) if (tabelacommit[z].state) 
    {
        if (tabelacommit[z].bot) contbot++;
    }

    stats1->commits = totalcommits;
    stats1->bots_colab = contbot;
    fclose (commits);

    FILE *commits2 = fopen("./entrada/commits-g2.csv", "r");

    row=0;
    while (fgets(line,MAX_SIZE, commits2)) 
    {
       
        collumn =0;
        linecopy = strdup (line);
        linecopy2 = strdup (line);

        if (row !=0)
        {
            while (token = strsep (&linecopy,";"))
            {
                if (collumn == 1) author_id = strdup(token);
                if (collumn == 2) com_id = strdup(token);
                collumn++;
            }
            k=addCOMMITS_Aux(author_id,tabelaCommitAux,linecopy2);
            l=addCOMMITS_Aux(com_id,tabelaCommitAux,linecopy2);
            if (k==0) colaboradores++;
            if (l==0) colaboradores++;
        }      
            row++;
    }
    stats1->colaboradores = colaboradores;
    fclose (commits2);

    char file[500];
    sprintf(file,"entrada/%s",filepath[1]);
    readfile (file,stats1,tabelacommit,tabelarepo,tabelauser);
    return 0;
}