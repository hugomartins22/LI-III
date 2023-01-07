#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


//definição dos tamanhos máximos

#define MAX_SIZE 1640000
#define MAX_USERS 30000000
#define MAX_REPOS 100000000
#define MAX_COMMITS 100000000


//função que ordena os arrays 
void merge(long arr[], long l, long m, long r)
{
    long i, j, k;
    long n1 = m - l + 1;
    long n2 = r - m;
  
    long *L=malloc(sizeof(long)*(n1)), *R=malloc(sizeof(long)*(n2));
  
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
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
  

//função que ordena os arrays 

void mergeSort(long arr[], long l, long r)
{
    if (l < r)
    {
        long m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int is_num (char string[]){
    for (int i=0;i<strlen (string); i++) 
        if (!isdigit(string[i])) return 0 ;

    return 1 ;
}

//função que preenche o array com os users id 

long carryUsers(char* Filepath,long users[])
{
    
    char line[MAX_SIZE];
    long i=0;
    char *parameter;
    int collumn=0;
    FILE *file=fopen(Filepath, "r");

 
     while (fgets(line,MAX_SIZE,file) && i<MAX_USERS)
     {
        collumn=0;
        parameter = strtok(line, ";");
        while(parameter!=NULL)
        {
            if(collumn==0)  
            {
                users[i]=atol(parameter);
            }
            collumn++;
            parameter = strtok(NULL,";");
        }
        i++;
        
    }
    mergeSort (users,0,i);
    fclose (file);
return i;
}


//função que preenche o array com os repos id 

long carryRepos(char* Filepath,long repos[])
{
    
    char line[MAX_SIZE];
    long i=0;
    char *parameter;
    int collumn=0;

    FILE *file=fopen(Filepath, "r");

 
     while (fgets(line,MAX_SIZE,file) && i<MAX_REPOS)
     {
        collumn=0;
        parameter = strtok(line, ";");
        while(parameter!=NULL)
        {
            if(collumn==0)  
            {
                repos[i]=atol(parameter);
            }
            collumn++;
            parameter = strtok(NULL,";");
        }
        i++;
    }

    fclose (file);
    mergeSort (repos,0,i);
return i;
}


//função que preenche o array com os commits id 

long carryCommits(char* Filepath,long commits[])
{
    
    char line[MAX_SIZE];
    long i=0;
    char *parameter;
    int collumn=0;

    FILE *file=fopen(Filepath, "r");

 
     while (fgets(line,MAX_SIZE,file) && i<MAX_COMMITS)
     {
        collumn=0;
        parameter = strtok(line, ";");
        while(parameter!=NULL)
        {
            if(collumn==0)  
            {
                commits[i]=atol(parameter);
            }
            collumn++;
            parameter = strtok(NULL,";");
        }
        i++;
    }

    fclose (file);
    mergeSort (commits,0,i);

return i;
}


//função que faz a procura binária num array

int binarySearch(long arr[], long l, long r, long x)
{
    if (r >= l) {
        long mid = l + (r - l) / 2;
 
        if (arr[mid] == x)
            return 1;
 
        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);

        return binarySearch(arr, mid + 1, r, x);
    }
    return 0;
}

//função que verifica se o commiter id e o author id pertencem ao array de users 

int user_com(long users[],long contUsers,char* line)
{

    int collumn=0,valido =1;
    long author_id=0,commiter_id=0;
    char *parameter;

    parameter = strtok(line, ";");

    while(parameter!=NULL)
    {
        if (collumn==1) 
        {
            author_id=atol(parameter);
            valido = binarySearch(users,0,contUsers,author_id);       
        }
        else if (collumn==2 && valido ==1)
        {
            commiter_id=atol(parameter);
            valido = binarySearch(users,0,contUsers,commiter_id);
        }
        collumn ++; 
        parameter = strtok(NULL,";");
    }
    if (valido == 1) return 1 ;
    else return 0; 
}


//função que verifica se o repos.owner.id pertence ao array de users 

int user_repos(long users[],long contUsers,char* line)
{
    int collumn=0,valido =1;
    long repos_owner=0;
    char *parameter;


    parameter = strtok(line, ";");

    while(parameter!=NULL)
    {
        if (collumn==1) 
        {
            repos_owner=atol(parameter);
            valido = binarySearch(users,0,contUsers,repos_owner);            
        }
        collumn ++; 
        parameter = strtok(NULL,";");
    }
    if (valido == 1) return 1 ;
    else return 0; 
}


//função que verifica se o commits.repo.id e o author id pertencem ao array de repos

int repos_commit(long repos[],long contRepos,char* line)
{

    int collumn=0,valido =1;
    long comRepos_id=0;
    char *parameter;


    parameter = strtok(line, ";");


    while(parameter!=NULL)
    {
        if (collumn==0) 
        {
            comRepos_id=atol(parameter);
            valido = binarySearch(repos,0,contRepos,comRepos_id);            
        }
        collumn ++; 
        parameter = strtok(NULL,";");
    }
    if (valido == 1) return 1 ;
    else return 0; 
}


//função que verifica se o repositorio tem algum commit se tiver passa-o senao elimina

int commit_repos(long commits[],long contCommits,char* line)
{

    int collumn=0,valido =1;
    long Repos_id=0;
    char *parameter;


    parameter = strtok(line, ";");

    while(parameter!=NULL)
    {
        if (collumn==0) 
        {
            Repos_id=atol(parameter);
            valido = binarySearch(commits,0,contCommits,Repos_id);
            
        }
        collumn ++; 
        parameter = strtok(NULL,";");
    }
    if (valido == 1) return 1 ;
    else return 0; 
}


//funçao que faz tudo , desde correr ficheiros e colocar as linhas validas e elimar

int main()
{
    FILE *commits = fopen("./saida/commits-ok.csv", "r");
    FILE *commits_ok = fopen("commits-final2.csv", "w");

    FILE *repos = fopen("./saida/repos-ok.csv", "r");
    FILE *repos_ok = fopen("repos-final2.csv", "w");

    FILE *commits2 = fopen("commits-final2.csv", "r");
    FILE *commits2_ok = fopen("./saida/commits-final.csv", "w");

    FILE *repos2 = fopen("repos-final2.csv", "r");
    FILE *repos2_ok = fopen("./saida/repos-final.csv", "w");

    FILE *users = fopen("./saida/users-ok.csv", "r");
    FILE *users_ok = fopen("./saida/users-final.csv", "w");
    
    long row=0; 
    
    char *line=malloc(MAX_SIZE),*line_copy=malloc(MAX_SIZE);
    
    static long ArrUsers[MAX_USERS];
    static long ArrRepos[MAX_REPOS];
    static long ArrCommits[MAX_COMMITS];

    if (!users || !repos || !commits) 
        printf("Can't open file\n"); 

    long contUsers = carryUsers("./saida/users-ok.csv",ArrUsers);


    while (fgets(line,MAX_SIZE,users)) fprintf(users_ok, "%s", line);
        fclose(users);
        fclose(users_ok);


    while (fgets(line,MAX_SIZE, commits)) 
    { 
        strcpy(line_copy,line);

        if (row==0)
            fputs(line, commits_ok);

        if (row!=0 && (user_com(ArrUsers,contUsers,line_copy)==1)) 
        { 
            fputs(line, commits_ok);
        }
        else if (row!=0) 
        {  
        }
        row++;
    }
    fclose(commits);


    while (fgets(line,MAX_SIZE, repos))
    { 

        strcpy(line_copy,line);

        if (row==0)
            fputs(line, repos_ok);

        if (row!=0 && (user_repos(ArrUsers,contUsers,line_copy)==1)) 
        { 
            fputs(line, repos_ok);
        }
        else if (row!=0) 
        {  
        }
        row++;
    }
    fclose(repos);

    long contRepos = carryRepos("repos-final2.csv",ArrRepos);

    while (fgets(line,MAX_SIZE, commits2))
    { 

        strcpy(line_copy,line);

        if (row==0)
            fputs(line, commits2_ok);

        if (row!=0 && (repos_commit(ArrRepos,contRepos,line_copy)==1)) 
        { 
            fputs(line, commits2_ok);
        }
        else if (row!=0) 
        {  
        }
        row++;
    }

    fclose(commits2);

    long contCommits = carryCommits("commits-final2.csv",ArrCommits);

    while (fgets(line,MAX_SIZE, repos2))
    { 

        strcpy(line_copy,line);

        if (row==0)
            fputs(line, repos2_ok);

        if (row!=0 && (commit_repos(ArrCommits,contCommits,line_copy)==1)) 
        { 
            fputs(line, repos2_ok);
        }
        else if (row!=0) 
        {  
        }
        row++;
    }

    fclose(repos2);

return 0;
}
