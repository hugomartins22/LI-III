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


struct users
{
	KEY id;
	char state; // 0 = free; 1 = used
	int followers;
	int following;
	char* login;
	char* type;
	char* follower_list;
    char* following_list;
	char* repos;
};




/**
 * \brief Função initUsers
 * 
 * É a função reponsável por inicializar a tabela de users
 * @param table tabela de users
 */
USERS initUsers ()
{
    USERS tabelauser=malloc(MAX_USERS*sizeof(struct users));
	int i;
	for (i=0; i<MAX_USERS; tabelauser[i++].state = 0);

    return tabelauser;
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

    while (table[p].state && strcmp(table[p].id,i)){
        p = (p+1)%MAX_USERS;
    }

    if (table[p].state)
    {   
        return strdup(table[p].repos);
    }
    return NULL;
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
 * \brief Função get_state
 * 
 * É a função reponsável por retornar um state 
 * @param tUser tabela de users
 * @param i posição que se prentende procurar
 */
char get_state(USERS tUser,int i)
{
    char aux ;
    aux = tUser[i].state;
    return aux ;
}

/**
 * \brief Função get_id
 * 
 * É a função reponsável por retornar a chave de uma posição na tabela
 *  
 * @param tUser tabela de users
 * @param i posição que se prentende procurar
 */
KEY get_id (USERS tUser,int i)
{
    KEY aux = tUser[i].id;
    return aux;
}
