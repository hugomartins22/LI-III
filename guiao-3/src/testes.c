#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
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
 * \brief Função readfiletestes
 * 
 * É uma função cujo objetivo é imprimir a media do resultado das queries
 *
 * @param stats1 (STRUCT STATS)
 * @param tabelacommit (STRUCT COMMITS)
 * @param tabelarepo (STRUCT REPOS);
 * @param tabelauser (STRUCT USER);
 */
void readfiletestes (STATS stats1,COMMITS tabelacommit ,REPOS tabelarepo ,USERS tabelauser ) 
{
    int n;
    char*line=malloc(MAX_SIZE);
    char*linecopy=malloc(MAX_SIZE);
    char*linecopy2=malloc(MAX_SIZE);
    long collumn=0;
    char *parameter,*a,*b,*c,*d,*token; 
    clock_t start, end;
    double cpu_time_usedq1=0.0 ,cpu_time_usedq2=0.0,cpu_time_usedq3=0.0,cpu_time_usedq4=0.0,cpu_time_usedq5=0.0,cpu_time_usedq6=0.0,cpu_time_usedq7=0.0,cpu_time_usedq8=0.0,cpu_time_usedq9=0.0,cpu_time_usedq10=0.0 ;

 
        for (int i=0;i<1;i++) {
                  
                    n=1;
                    start = clock();
                    query1(stats1,n);
                    end = clock();
                    cpu_time_usedq1+= ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;
                    
                    start = clock();
                    query2(stats1,n);
                    end = clock();
                    cpu_time_usedq2 += ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;

                    start = clock();        
                    query3(stats1,n); 
                    end = clock();
                    cpu_time_usedq3 += ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;

                    start = clock();         
                    query4(stats1,n);
                    end = clock();
                    cpu_time_usedq4 += ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;             
              
                    b="100";
                    c="2010-01-01";
                    d="2015-01-01";
                    start = clock(); 
                    query5(tabelauser,tabelacommit,getdata(c),getdata(d),atoi(b),n);
                    end = clock();
                    cpu_time_usedq5 += ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;

                    c="Python";
                    b ="5";
                    start = clock(); 
                    query6(tabelauser,tabelarepo,tabelacommit,atoi(b),c,n);
                    end = clock();
                    cpu_time_usedq6 += ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;

                    DATA date =initdata();
                    date =getdata("2014-04-25");
                    start = clock(); 
                    query7(tabelacommit,tabelarepo,get_ano(date),get_mes(date),get_dia(date),n);
                    end = clock();
                    cpu_time_usedq7 += ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;

                    c="2016-10-05";
                    b ="3";
                    start = clock(); 
                    query8(tabelarepo,tabelacommit,atoi(b),getdata(c),n);
                    end = clock();
                    cpu_time_usedq8 += ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;


                        
                   /*
                    b="4";
                    start = clock(); 
                    query9(tabelauser,tabelacommit,tabelarepo,atoi(b),(get_utilizadores(stats1)),n);
                    end = clock();
                    cpu_time_usedq9 += ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;
                    */

                    b="50";
                    start = clock(); 
                    query10 (tabelacommit,atoi(b),tabelauser,n);
                    end = clock();
                    cpu_time_usedq10 += ((double) (end - start)) / CLOCKS_PER_SEC;
                    n++;
        }
                    printf("QUERIE 1 Average CPU Time:%f\n", cpu_time_usedq1);
                    printf("QUERIE 2 Average CPU Time:%f\n", cpu_time_usedq2);
                    printf("QUERIE 3 Average CPU Time:%f\n", cpu_time_usedq3);
                    printf("QUERIE 4 Average CPU Time:%f\n", cpu_time_usedq4);  
                    printf("QUERIE 5 Average CPU Time:%f\n", cpu_time_usedq5);
                    printf("QUERIE 6 Average CPU Time:%f\n", cpu_time_usedq6);  
                    printf("QUERIE 7 Average CPU Time:%f\n", cpu_time_usedq7);   
                    printf("QUERIE 8 Average CPU Time:%f\n", cpu_time_usedq8);
                    // printf("QUERIE 9 Average CPU Time:%f\n", cpu_time_usedq9/3);  
                    printf("QUERIE 10 Average  CPU Time:%f\n", cpu_time_usedq10);
}

int compareFiles (FILE *e, FILE *c){
  char c1 = getc(e);
  char c2 = getc(c);
  int r=1;
  while (c1 != EOF && c2 != EOF && r){
    if (c1!=c2) r=0;
    c1 = getc(e);
    c2 = getc(c);
  }
  return r;
}



/**
 * \brief Função main 
 * 
 * É a função reponsável pelo catálogo dos dados 
 * É responsável pela leitura dos dados de cada um dos ficheiros csv e pelo catálogo deles em hash tables
 *
 *
 */
int main () 
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

    readfiletestes (stats1, tabelacommit , tabelarepo , tabelauser ); 

    
/*
    FILE *commands1_1 = fopen("./saidas/commands1_output.txt", "r");
    FILE *commands1_2 = fopen("./expectedfiles/commands1_output.txt", "r");
    printf("commands1=%d\n",compareFiles(commands1_1, commands1_2));
    
    FILE *commands2_1 = fopen("./saidas/commands2_output.txt", "r");
    FILE *commands2_2 = fopen("./expectedfiles/commands2_output.txt", "r");
    printf("commands2=%d\n",compareFiles(commands2_1, commands2_2));
    
    FILE *commands3_1 = fopen("./saidas/commands3_output.txt", "r");
    FILE *commands3_2 = fopen("./expectedfiles/commands3_output.txt", "r");
    printf("commands3=%d\n",compareFiles(commands3_1, commands3_2));
    
    FILE *commands4_1 = fopen("./saidas/commands4_output.txt", "r");
    FILE *commands4_2 = fopen("./expectedfiles/commands4_output.txt", "r");
    printf("commands4=%d\n",compareFiles(commands4_1, commands4_2));
    
    FILE *commands5_1 = fopen("./saidas/commands5_output.txt", "r");
    FILE *commands5_2 = fopen("./expectedfiles/commands5_output.txt", "r");
    printf("commands5=%d\n",compareFiles(commands5_1, commands5_2));
    
    FILE *commands6_1 = fopen("./saidas/commands6_output.txt", "r");
    FILE *commands6_2 = fopen("./expectedfiles/commands6_output.txt", "r");
    printf("commands6=%d\n",compareFiles(commands6_1, commands6_2));
    
    FILE *commands7_1 = fopen("./saidas/commands7_output.txt", "r");
    FILE *commands7_2 = fopen("./expectedfiles/commands7_output.txt", "r");
    printf("commands7=%d\n",compareFiles(commands7_1, commands7_2));
    
    FILE *commands8_1 = fopen("./saidas/commands8_output.txt", "r");
    FILE *commands8_2 = fopen("./expectedfiles/commands8_output.txt", "r");
    printf("commands8=%d\n",compareFiles(commands8_1, commands8_2));
    
    FILE *commands9_1 = fopen("./saidas/commands9_output.txt", "r");
    FILE *commands9_2 = fopen("./expectedfiles/commands9_output.txt", "r");
    printf("commands9=%d\n",compareFiles(commands9_1, commands9_2));
*/
    return 0;
}


