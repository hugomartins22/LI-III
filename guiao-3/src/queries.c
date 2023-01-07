#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include <math.h>
#include "data.h"
#include "lang.h"
#include "users.h"
#include "repos.h"
#include "commits.h"
#include "queries.h"
#include "generic.h"

struct stats
{
	int user;
	int bot;
	int organization;
	int repos ;
	int colaboradores ;
	int bots_colab;
	int utilizadores;
	int commits;
};

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
    int size = MAX_USERS/2;
    IDS_ARR author_ids_array= initIDS_ARR(size); //guardar os ids todos num array
    DATA commit_at=initdata();

    if(!compare_DATAS(data1, data2,1)) printf("ERROR MESSAGE \ndata_inicio é depois da data_fim");
    else auxq5(commits, author_ids_array,data1,data2,commit_at, size);

    print_Q(author_ids_array,users, top, N, size);
    freeIDS_ARR(author_ids_array, size);
    free(commit_at);
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
    int size= MAX_REPOS/8;
    int *array_id_repos=malloc(sizeof(int)*size);
    IDS_ARR author_ids_array= initIDS_ARR(size*2);
    COMMITS commitaux=initcommit();
    
    searchIDREPOSlang(repos,array_id_repos,lang); //lista de ids de repos com a linguage

    author_ids_array= aux_q6 (commitaux,author_ids_array,array_id_repos,commits_table, size);
    print_Q(author_ids_array,users, n, N, size);
    freeIDS_ARR(author_ids_array, size*2);
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
 
    aux_q7(cAno ,cMes, cDia, Trepos, N, Tcommits);
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
    LANG_ARR lang_array = init_lang(1000000); 
    int i, j=0;
    char *lang;
    int count;

    aux_q8( data, repos,  commits, lang_array);

    print_lang(lang_array, N, top);
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
    int h,cont=0,j;
    char *repo_id,*friends,*token,*linecopy,*friend_repos,*token2,*linecopy2;

    AUX10 arr = init_aux10(totalusers);
 
    arr=aux_q9(tUser,tCom,arr,&j);
    print_q9( N, tUser, arr,q,j);
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
    AUX10 arr = init_aux10(MAX_COMMITS); 

    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",N );
    FILE *f = fopen (exitfile,"w");

    aux_q10( tabela, top, tabelauser, f, arr);


    fclose(f);
    free(exitfile);
    free_arr(arr);
}

/**
 * \brief Função get_utilizadores
 * 
 * É uma função cujo objetivo é retornar o total de users
 *
 * @param stats (STRUCT STATS)
 */
int get_utilizadores(STATS stats)
{
    int p = stats->utilizadores;
    return p;
}

/**
 * \brief Função init_stats
 * 
 * É uma função cujo objetivo é inicializar o array de stats
 *
 */
STATS init_stats ()
{
    STATS stats1 = malloc(sizeof (struct stats));
    return stats1;
}

/**
 * \brief Função insere_stats
 * 
 * É uma função cujo objetivo é preencher os campos da struct stats
 * @param stats1 (STRUCT STATS)
 * @param bot inteiro correspondente ao total de bots
 * @param user inteiro correspondente aos users
 * @param organ inteiro correspondente ao total de organizações
 * @param totalusers inteiro correspondente ao total de users
 */
STATS insere_stats(STATS stats1,int bot,int user,int organ,int totalusers)
{
    stats1->bot=bot;
    stats1->user=user;
    stats1->organization =organ;
    stats1->utilizadores = totalusers;
    return stats1;
}


/**
 * \brief Função insere_stats_repos
 * 
 * É uma função cujo objetivo é preencher um campo da struct stats
 * @param stats1 (STRUCT STATS)
 * @param contrepos inteiro correspondente ao total de repositórios
 */
STATS insere_stats_repos (STATS stats1,int contrepos)
{
    stats1->repos = contrepos;
    return stats1;
}

/**
 * \brief Função insere_stats_commits_bots
 * 
 * É uma função cujo objetivo é preencher um campo da struct stats
 * @param stats1 (STRUCT STATS)
 * @param totalcommits inteiro correspondente ao total de commits
 * @param contbot inteiro correspondente aos bots
 */
STATS insere_stats_commits_bots (STATS stats1,int totalcommits,int contbot)
{
    stats1->commits = totalcommits;
    stats1->bots_colab = contbot;
    return stats1;
}

/**
 * \brief Função insere_stats_colabs
 * 
 * É uma função cujo objetivo é preencher um campo da struct stats
 * @param stats1 (STRUCT STATS)
 * @param colaboradores inteiro correspondente ao total de colaboradores
 */
STATS insere_stats_colabs (STATS stats1,int colaboradores)
{
    stats1->colaboradores = colaboradores;
    return stats1;
}
