#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define BUFFER_SIZE 1684000


//função que dada uma lista retorna o tamanho da mesma 

int list_length(char* parameter){

    if (strlen(parameter)== 2) return 0;
    else{
        int i,
            cont = 1;

        for (i=0; parameter[i]!= ']'; i++)
            if (parameter[i]==',') cont++; 

        return cont;
    }
}


//função para verificar a data no formato certo e cumpre o a) do enunciado

int verify_date (char *created_at){

    int list[6], 
        value, 
        i=0;

    char *value_str;

    value_str = strtok(created_at, "-");

    while (value_str!=NULL){

        if(i==0 && strlen(value_str)!=4) return 0;
        else if (i!=0 && strlen(value_str)!=2) return 0;
        else {  value = atoi(value_str);
                list[i] = value;
                value_str = strtok(NULL, "- :");
                i++;
        }
    }

    int YY,MM,DD, hh, mm, ss, date, timer;
    int current_year, current_month, current_day, current_hour, current_minutes, current_secunds;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    YY=list[0];
    MM=list[1];
    DD=list[2];
    hh=list[3];
    mm=list[4];
    ss=list[5];

    current_year = tm.tm_year+ 1900;
    current_month = tm.tm_mon+1;
    current_day = tm.tm_mday;
    current_hour = tm.tm_hour;
    current_minutes = tm.tm_min;
    current_secunds = tm.tm_sec;


    if(YY>=2005 && YY<=current_year){

        if(MM>=1 && MM<=12){
            
            if((DD>=1 && DD<=31) && (MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10 || MM==12))
                date=1;
            else if((DD>=1 && DD<=30) && (MM==4 || MM==6 || MM==9 || MM==11))
                date=1;
            else if((DD>=1 && DD<=28) && (MM==2))
                date=1;
            else if(DD==29 && MM==2 && (YY%400==0 ||(YY%4==0 && YY%100!=0)))
                date=1;
            else
                date=0;
        }
        else date=0;

        if (YY==2005 && MM<=4){
         
            if (MM<4) date=0;
            else if (MM==4 && DD<7) date=0;
            else date=1;
        }

        else if (YY==current_year && MM>=current_month){
        
            if (MM>current_month) date=0;
            else if (MM==current_month && DD>current_day) date=0;
            else date=1;
        }
        else date=1;  
    }
    else date=0;

       

    if (hh>=0 && hh<=24 && mm>=0 && mm<=60 && ss>=0 && ss<=60) {
        
        if(YY==current_year && MM==current_month && DD==current_day && hh>=current_hour){
            if(hh>current_hour) timer=0;
            else if (hh==current_hour){
                if (mm>=current_minutes){
                    if (mm>current_minutes) timer=0;
                    else if (mm==current_minutes){
                        if (ss<current_secunds) timer=1;
                        else timer=0;
                    }  
                    else timer=1;
                }
                else timer=1; 
            }
            else timer=1;
        }
        else timer=1;
    }
    else timer=0;

    if (date && timer) return 1;
    else return 0;

}


//verifica se a lista é válida

int verify_list(char* list){
    
    int j=strlen(list)-1;


    if (list[0]!='[' || list[j]!=']') return 0;

    if (list[0]=='[' && list[1]==']') return 1;
    else {
        char *value_str;
        int value;

        list++;
        value_str=strtok(list,",]");

        while (value_str!=NULL){

            value = atoi(value_str);

            if (value<=0) return 0;
            else value_str = strtok(NULL, ",]");
        }
    }
    return 1; 
}




// verifica se o campo type dos users é um dos 3 (BOT,Organization,User)

int verify_type(char* type){

    if( !(strcmp(type,"User")) || !(strcmp(type,"Organization")) || !(strcmp(type,"Bot"))) return 1;
    else return 0;
}


// verifica se o campo wiki dos users é um dos 3 (BOT,Organization,User)
int verify_wiki (char* line) {
    if( !(strcmp(line,"True")) || !(strcmp(line,"False")) || !(strcasecmp(line,"None"))) return 1;
    else return 0;
}


// verifica se é uma string 

int is_string (char string[] ) {
int count =0;
    for (int i=0;i<strlen (string); i++) {
        if (!isdigit(string[i])) count ++; 
        if (count ==1) return 1;
    }
    return 0 ;
}

// verifica se uma string é um número

int is_num (char string[]){
    for (int i=0;i<strlen (string); i++) 
        if (!isdigit(string[i])) return 0 ;

    return 1 ;
}

// verifica se algum parametro é vazio 

int empty_parameters(char *line){
    int i,j;

    for (i=0; i<strlen(line)-1; i++){
        j=i+1;
        if (line[0]==';')  return 1;
        if (line[i]==';' && line[j]==';') return 1;
        if (i==(strlen(line)-2) && line[j]==';') return 1;
    }
    return 0;
}


//função que faz todos os testes a cada um  dos campos relativos ao ficheiro user

int verify_user (char *line){
    
    int collumn=0,id, follower_list_length, following_list_length,j, a,b,c,d,e,f,g,h,k,l,m,n,o,p,z=0;
    char *created_at, *parameter, *type, *login, *follower_list, *following_list, *public_repos,*public_gists,*followers, *following;


    if (empty_parameters(line)) return 0; 
    else {
        
        parameter = strtok(line, ";"); 

        while(parameter!=NULL){ 

            if      (collumn==0) {id = atoi(parameter);}
            else if (collumn==1) {login = parameter;}
            else if (collumn==2) {type = parameter;}
            else if (collumn==3) {created_at = parameter;}
            else if (collumn==4) {followers = parameter;}
            else if (collumn==5) {follower_list = parameter;follower_list_length = list_length(parameter);}
            else if (collumn==6) {following = parameter;}
            else if (collumn==7) {following_list = parameter;following_list_length = list_length(parameter);}
            else if (collumn==8) {public_gists = parameter;}
            else if (collumn==9) {public_repos = (parameter) ;}
            else return 0;

            collumn ++;
            parameter = strtok(NULL, ";\n");
        }

        a=atoi(followers); 
        b=atoi(following);
        c=atoi(public_gists);
        d=atoi(public_repos);
        e=verify_type(type);
        f=verify_date(created_at);
        g=verify_list(follower_list);
        h=verify_list(following_list);
        k= is_num(followers);
        l= is_num(following);
        m= is_num(public_gists); 
        n= is_num((public_repos));

        if (id>0 && a>=0 && b>=0 && c>=0 && d>=0 && e && f && g && h && k && l && m && n &&  (a==follower_list_length) && (b==following_list_length) ) return 1;
        else return 0; 
    }
}


//função que faz todos os testes a cada um  dos campos relativos ao ficheiro de repositórios

int verify_repos(char* line){

    int collumn=0,j,id,owner_id, a,b,c,d,e,f,g,h,k,l,m,n,o,z=0;
    char *description, *forks_count, *open_isues, *stargazers_count, *size,*parameter, *created_at, *updated_at, *full_name, *license, *has_wiki, *language, *default_branch;

    if (empty_parameters(line)) return 0; 
    else {

        parameter = strtok(line, ";");

            while(parameter!=NULL)
            {

                if (collumn==0) id=atoi(parameter);
                else if (collumn==1) owner_id = atoi(parameter);
                else if (collumn==2) full_name = parameter;
                else if (collumn==3) license = parameter;
                else if (collumn==4) has_wiki = parameter;
                else if (collumn==5) description = parameter;
                else if (collumn==6) language = parameter;
                else if (collumn==7) default_branch = parameter;
                else if (collumn==8) created_at = parameter;
                else if (collumn==9) updated_at = parameter;
                else if (collumn==10) forks_count = parameter;
                else if (collumn==11) open_isues = parameter;
                else if (collumn==12) stargazers_count = parameter;
                else if (collumn==13)  size = (parameter); 
                
                if (collumn>13) return 0;

                collumn ++; 
                parameter = strtok(NULL,";");
            }

            a=atoi(forks_count);
            b=atoi(open_isues);
            c=is_num(size);
            d=atoi(stargazers_count);
            e=is_string(language);
            f=verify_date(created_at);
            g=verify_date(updated_at);
            k=!is_num(full_name);
            l=is_num(forks_count);
            m=is_num(open_isues);
            n=is_num(stargazers_count);
        
            if (id>0 && owner_id>0 && a>=0 && b>=0 && c>=0 && d>=0 && f && g  && k && l && m && n  ) return 1;
            else return 0; 
    }
}


//função que faz todos os testes a cada um  dos campos relativos ao ficheiro commits

int verify_commits(char* line){
    
    if (empty_parameters(line)) return 0;
    else {

        int collumn=0,repo_id, author_id, committer_id;
        char *parameter, *commit_at;

        parameter = strtok(line, ";");

        while(parameter!=NULL){

            if (collumn==0) repo_id=atoi(parameter); 
            else if (collumn==1) {author_id=atoi(parameter);}
            else if (collumn==2)  {committer_id=atoi(parameter);}
            else if (collumn==3) commit_at=parameter;
            collumn ++;
            parameter = strtok(NULL, ";\n\0");
        }


        if (repo_id>0 && author_id>0 && committer_id>0 && verify_date(commit_at)) return 1;
        else return 0;
    } 
} 


//principal que percorre os ficheiros e coloca as linhas válidas de acordo com as funções acima

int main() {

    
    FILE *users = fopen("./entrada/users.csv", "r"); 
    FILE *repos = fopen("./entrada/repos.csv", "r");
    FILE *commits = fopen("./entrada/commits.csv", "r");
    
    FILE *users_ok = fopen("./saida/users-ok.csv", "a");
    FILE *repos_ok = fopen("./saida/repos-ok.csv", "a");
    FILE *commits_ok = fopen("./saida/commits-ok.csv", "a");
    


    int  row=0;    
    char* line=malloc(BUFFER_SIZE),*line_copy=malloc(BUFFER_SIZE);

    if (!users || !repos || !commits) 
        printf("Can't open file\n"); 

    //users filter
    while (fgets(line,BUFFER_SIZE, users)) {
        
        for (int i=0; i<strlen(line); i++) line_copy[i]=line[i]; 
        line_copy[strlen(line)]='\0';


        if (row==0) fputs(line, users_ok);

        if (row!=0 && verify_user(line_copy)) { 
            fputs(line, users_ok);
        }
        else if (row!=0) {
        }
        
        row++;  
    }
    fclose(users);
    fclose(users_ok);



    //Repository filter
    row=0;
    while (fgets(line,BUFFER_SIZE, repos)) 
    { 


        for (int i=0; i<strlen(line); i++) line_copy[i]=line[i];
        line_copy[strlen(line)]='\0';


        if (row==0) fputs(line, users_ok);
        line_copy[strlen(line)]='\0';

        if (row!=0 && verify_repos(line_copy)) { 
            fputs(line, repos_ok);
        }
        else if (row!=0) {  
        }
        row++; 
    }

    fclose(repos);
    fclose(repos_ok);


    //commits filter
    row=0;
    
    while (fgets(line,BUFFER_SIZE, commits)) { 

        for (int i=0; i<strlen(line); i++) line_copy[i]=line[i]; 
        line_copy[strlen(line)]='\0';

        if (row==0) fputs(line, users_ok);

        if (row!=0 && verify_commits(line_copy)) { 
            fputs(line, commits_ok);
        }
        else if (row!=0) {  

        }
        
        row++;
    }
    fclose(commits);
    fclose(commits_ok);
    free (line);
    free(line_copy);
}

