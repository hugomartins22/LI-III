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
#include "queries.h"
#include "generic.h"
#include "g3.h"




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
                            DATA date =initdata();
                            date =getdata(b);
                            query7(tabelacommit,tabelarepo,get_ano(date),get_mes(date),get_dia(date),n);
                            break;
                         }
                    if(strcmp (a,"9")==0) { query9(tabelauser,tabelacommit,tabelarepo,atoi(b),(get_utilizadores(stats1)),n);break;}
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
    USERS tabelauser=initUsers();
    COMMITS tabelacommit=initCOMMITS();
    REPOS tabelarepo=initREPOS();
    COMMITS_AUX tabelaCommitAux =initCOMMITSAUX();

    STATS stats1 = init_stats();
    
   FILE *users = fopen("./entrada/users-g3.csv", "r");  
   // FILE *users = fopen("./entrada/users-g2.csv", "r");  


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
    
    stats1=insere_stats( stats1, bot, user, organ, totalusers);
    fclose(users);

    row =0;
    DATA updated ;
    updated = initdata();

    FILE *repos = fopen("./entrada/repos-g3.csv", "r");
    //FILE *repos = fopen("./entrada/repos-g2.csv", "r");

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
    stats1=insere_stats_repos(stats1,contrepos);
    fclose(repos);

    row =0;


    FILE *commits = fopen("./entrada/commits-g3.csv", "r");
    //FILE *commits = fopen("./entrada/commits-g2.csv", "r");

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

    contbot=contabots(tabelacommit);

    stats1=insere_stats_commits_bots(stats1,totalcommits,contbot);
    fclose (commits);

    FILE *commits2 = fopen("./entrada/commits-g3.csv", "r");
    //FILE *commits2 = fopen("./entrada/commits-g2.csv", "r");

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
    stats1= insere_stats_colabs(stats1,colaboradores);
    fclose (commits2);

    char file[500];
    if (filepath[1] != NULL) {
    sprintf(file,"entrada/%s",filepath[1]);
    readfile (file,stats1,tabelacommit,tabelarepo,tabelauser);
    }
    else  
    {
        printf ("invalida");
        menu(stats1,tabelacommit,tabelarepo,tabelauser);
    }   
    return 0;
}
