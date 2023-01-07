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
#include "generic.h"


struct repos 
{
	KEY id; 
	char state; // 0 = free; 1 = used

	int owner_id;
	char* language;
	DATA updated_at; 
	char* description;

};


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
	char *copy;
	unsigned long p = hash (id)%MAX_REPOS;
	while (Trepos[p].state && strcmp (Trepos[p].id,id))
	    p = (p+1)%MAX_REPOS;
	if (Trepos[p].state) 
	{
		copy = strdup(Trepos[p].description);
	}
	return copy;
	

}



/**
 * \brief Função initREPOS
 * 
 * É a função reponsável por iniciar o parametro da taebla de hash "state" a 0 
 * @param repo_table (Tabela de repositórios
 * 
 */
REPOS initREPOS () 
{
	REPOS tabelarepo=malloc(MAX_REPOS*sizeof(struct repos));
	int i;
	for (i=0; i<MAX_REPOS; tabelarepo[i++].state = 0);
	return tabelarepo;
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

for (i=0; i<MAX_REPOS; i++) if (r[i].state)
{
	printf("ID:%s \n",r[i].id);
	printdata(r[i].updated_at);
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
		printdata(r.updated_at);

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
    char *aux;
    unsigned long p = hash (id_repo)%MAX_REPOS;
    while (repos[p].state){
        if (!strcmp (repos[p].id,id_repo))
		{
			aux=repos[p].language;
            return aux;
        }
        else p = (p+1)%MAX_REPOS;	
    }
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
