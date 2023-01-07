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


#define MAX_LINE  10

/**
 * \brief Função auxwin
 * 
 * É a função que ajuda na construção do menu 
 *
 * @param win apontador para a janela 
 * @param py inteiro correspondente à posição inicial de linhas
 * @param px inteiro correspondente à posição inicial de colunas
 */
int auxwin(WINDOW *win , int py,int px) 
{

    box(win,py,px);
    wattron(win,A_STANDOUT);
    mvwprintw(win,0,2,"Menu");
    mvwprintw(win,0,7,"Guiao3");
    wattroff(win,A_STANDOUT);
    refresh();
    wrefresh(win);

}

/**
 * \brief Função file_lines
 * 
 * É a função que nos diz quantas linhas possui o ficheiro 
 *
 * @param f apontador para ficheiro que pretendemos ler
 *
 * @returns um inteiro correspondente ao número total de linhas
 */
int file_lines (FILE  *f)
{
    int k;
    char aux [1000000];
    for (k=0;fgets(aux,1000000,f);k++)
    printf("%d\n",k );
    if (k%MAX_LINE !=0 ) return (k/MAX_LINE +1);
    else return (k/MAX_LINE);

}

/**
 * \brief Função go_line
 * 
 * É a função que nos leva até uma posição no ficheiro .
 *
 * @param f apontador para ficheiro que pretendemos ler
 * @param pos posição correspontedte à pagina em que nos encontramos
 */
void go_line (FILE *f , int pos ) 
{
    char aux [1000000];
    int k ;
    fseek (f,0,SEEK_SET);
    k=0 ;
    while(k<pos && fgets(aux,1000000,f)) 
    {
    k++;
    }
}


/**
 * \brief Função printnew
 * 
 * É a função que organiza os ouputs dos ficheiros e permite a navegação pelos resultados.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param file apontador para ficheiro que pretendemos ler
 * @param pos posição correspontedte à pagina em que nos encontramos
 * @param pages número total de páginas 
 */
void printnew(WINDOW *win,int i,int j,FILE *file ,int pos,int pages) 
{
    
    fseek(file,0,SEEK_SET);

    wclear (win);
    wrefresh(win);
   
int line = 0 ,k=0;
char aux [20000],ch,p,str[1000];


 ch = 'o';
while (ch!='q' && ch!='Q') 
{
    wclear(win);
    while ((p = fgetc(file)) !=EOF && line < 10)  
    {
        if (p=='\n') line++;
         aux[k] = p;
         k++;
    }
    aux[k]='\0';
    wprintw(win,"%s\n" ,aux);
    wrefresh(win);
    wprintw(win, "------------------------Página %d-de-%d------------------------------------------------\n",pos+1,pages);
    wprintw(win ,"P-> Proximo\n");
    wprintw(win, "A-> Anterior\n");
    wprintw(win, "Q-> Voltar ao Menu \n");
    wprintw(win, "J-> Ir para a página \n");
    wrefresh(win);

    ch = getch();
    wrefresh(win);
    
    switch (ch ) 
    {
        case 'p':
        case 'P':  
        if (pos == pages-1)
        {
            fseek(file,-k,SEEK_CUR);
        } 
        else 
        {
        pos ++ ;
        fseek(file,-1,SEEK_CUR);
        wclear(win);
        wrefresh(win);
        }
        break;

        case 'a':
        case 'A':

        if (pos == 0) 
        {
            wclear(win);
            fseek(file,0,SEEK_SET);
        }
        else 
        {
        pos -- ;
        go_line(file,pos*(10));
        wclear(win);
        wrefresh(win);
        }
        break;
        
        case 'j':
        case 'J':
            wprintw(win,"Insira a página que pretende visualizar :\n");
            move(20, 2);
            i+=4;
            wrefresh(win);
            getstr(str);
            wrefresh(win);

            if (atoi(str)<0 ||atoi(str) > pages )
            {
                fseek(file,-k-1,SEEK_CUR);
            }
        else 
        {
        pos = atoi(str)-1;
        go_line(file,(atoi(str)-1)*(10));
        wclear(win);
        wrefresh(win);
        }
        break;
        default: 
         if (pos == 0) 
            {
            wclear(win);
            fseek(file,0,SEEK_SET);
            }
        else 
            {
                fseek(file,-k-1,SEEK_CUR);
            }
            
    }
    k=0;
    line =0;
    }
}



/**
 * \brief Função case1
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param stats1 (Struct STATS)
 */
int case1(WINDOW *win,int i,int j,STATS stats1) {
int p =0;
char aux[100];
        
        query1(stats1,1);
       
        FILE *f = fopen("./saidas/commands1_output.txt", "r");       
        wclear(win);
        wrefresh(win);    
        auxwin(win,0,0);
        wrefresh(win);    
        i=2;
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"O resultado da query 1 é :");
        wattroff(win,A_STANDOUT); 
        i++;
    
        while(fgets(aux,1000,f))
        {
            
        mvwprintw(win,i,j ,"%s",aux);
        wrefresh(win);
        i++;
        }
        i++;
        mvwprintw(win,i,j,"Prima qualquer tecla para regressar ao menu!");
        i++;
        wrefresh(win);
        fclose(f);
        getch();
}

/**
 * \brief Função case2
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param stats1 (Struct STATS)
 */
int case2(WINDOW *win,int i,int j,STATS stats1) 
{
    char aux [1000];
        
        query2(stats1,1);

    FILE *f = fopen("./saidas/commands1_output.txt", "r");           
        wclear(win);
        wrefresh(win);    
        auxwin(win,0,0);
        wrefresh(win);   
        i=2; 
        wattron(win,A_STANDOUT); 
        mvwprintw(win,i,j,"O resultado da query 2 é :");
        wattroff(win,A_STANDOUT); 
        i++;
        while(fgets(aux,1000,f))
        {
        mvwprintw(win,i,j ,"%s",aux);
        wrefresh(win);
        i++;
        }
        i++;
        mvwprintw(win,i,j,"Prima qualquer tecla para regressar ao menu!");
        i++;
        wrefresh(win);

        fclose(f);  

        getch();
}

/**
 * \brief Função case3
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param stats1 (Struct STATS)
 */
int case3(WINDOW *win,int i,int j,STATS stats1) 
{
    char aux[1000];
        
        query3(stats1,1);

        FILE *f = fopen("./saidas/commands1_output.txt", "r");           
        
        wclear(win);
        wrefresh(win);    
        auxwin(win,0,0);
        wrefresh(win);    
        i=2;
        wattron(win,A_STANDOUT); 
        mvwprintw(win,i,j,"O resultado da query 3 é :");
        wattroff(win,A_STANDOUT); 
        i++;
        while(fgets(aux,1000,f))
        {
        mvwprintw(win,i,j ,"%s",aux);
        wrefresh(win);
        i++;
        }
        i++;
        mvwprintw(win,i,j,"Prima qualquer tecla para regressar ao menu!");
        i++;
        wrefresh(win);
        fclose(f);
        getch();


}

/**
 * \brief Função case4
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param stats1 (Struct STATS)
 */
int case4(WINDOW *win,int i,int j,STATS stats1) 
{
    char aux[1000];
    int p =0;
         

        query4(stats1,1);
        FILE *f = fopen("./saidas/commands1_output.txt", "r");           
        wclear(win);
        wrefresh(win);    
        auxwin(win,0,0);
        wrefresh(win);    
       
        i=2;
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"O resultado da query 4 é :");
        wattroff(win,A_STANDOUT); 
        i++;

        while(fgets(aux,1000,f))
        {
        mvwprintw(win,i,j ,"%s",aux);
        wrefresh(win);
        i++;
        }
        i++;
        mvwprintw(win,i,j,"Prima qualquer tecla para regressar ao menu!");
        i++;
        wrefresh(win);
        fclose(f);
        getch();
}


/**
 * \brief Função case5
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param users (STRUCT USERS)
 * @param commitS (STRUCT COMMITS)
 */
int case5 (WINDOW *win,int i,int j,USERS users,COMMITS commits) 
{
    int p =0;
    char nr[10],d1[20],d2[20],lang[20];

    

        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"Querie Introduzida: 5");
        wattroff(win,A_STANDOUT);  
        i+=2; 
        mvwprintw(win,i,j,"Introduza o número :");
        i++;
        wrefresh(win);
        mvwgetstr(win,i,j,nr);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"O Número introduzido foi : %s ",nr);
        wrefresh(win);
        i++;
        wattroff(win,A_STANDOUT);  
        auxwin(win,0,0);
        wrefresh(win);
        mvwprintw(win,i,j,"Introduza a primeira data ");
        wrefresh(win);
        i+=2;
        mvwgetstr(win,i,j,d1);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"A data introduzida foi : %s ",d1);
        i++;
        wattroff(win,A_STANDOUT);  
        wrefresh(win);

        mvwprintw(win,i,j,"Introduza a segunda data ");
        i++;
        wrefresh(win);
        mvwgetstr(win,i,j,d2);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"A data introduzida foi : %s ",d2);
        i++;
        wattroff(win,A_STANDOUT);  
        wrefresh(win);

    
        query5(users,commits,getdata(d1),getdata(d2),atoi(nr),1);


     FILE *f = fopen("./saidas/commands1_output.txt", "r");           
        p=file_lines(f);
        printnew (win,i,j,f ,0,p);  
        fclose(f);

}


/**
 * \brief Função case6
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param tabelauser (STRUCT COMMITS)
 * @param tabelarepo (STRUCT REPOS)
 * @param tabelacommit (STRUCT COMMITS)
 */
void case6(WINDOW *win,int i,int j,USERS tabelauser,REPOS tabelarepo,COMMITS tabelacommit) 
{
int p =0;
    char nr[10],lang[20];
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"Querie introuduzida : 6");
        wattroff(win,A_STANDOUT);  
        i+=2; 
        mvwprintw(win,i,j,"Introduza o número  :");
        i++;
        wrefresh(win);
        mvwgetstr(win,i,j,nr);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"O Número introduzido foi : %s ",nr);
        wattroff(win,A_STANDOUT);
        wrefresh(win);
        i++;

        mvwprintw(win,i,j,"Introduza a linguagem ");
        i++;
        wrefresh(win);
        mvwgetstr(win,i,j,lang);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"A Linguagem introduzida foi : %s ",lang);
        i++;
        wattroff(win,A_STANDOUT);  
        wrefresh(win);


        i++;
        wattroff(win,A_STANDOUT);  
        auxwin(win,0,0);
        wrefresh(win);

     query6(tabelauser,tabelarepo,tabelacommit,atoi(nr),lang,1);
    
     FILE *f = fopen("./saidas/commands1_output.txt", "r");           
        p=file_lines(f);
        printnew (win,i,j,f ,0,p);  
        fclose(f);

}


/**
 * \brief Função case7
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param tabelacommit (STRUCT COMMITS)
 * @param tabelarepo (STRUCT REPOS)
 */
void case7(WINDOW *win,int i,int j,COMMITS tabelacommit,REPOS tabelarepo) 
{
    int p =0;
    char d1[20];
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"Querie introuduzida : 7");
        wattroff(win,A_STANDOUT);  
        i+=2; 
        mvwprintw(win,i,j,"Introduza a data :");
        i++;
        wrefresh(win);
        mvwgetstr(win,i,j,d1);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"A data introduzidA foi : %s ",d1);
        wattroff(win,A_STANDOUT);
        wrefresh(win);
        i+=2;
        DATA aux=getdata(d1);
        query7(tabelacommit,tabelarepo,get_ano(aux),get_mes(aux),get_dia(aux),1);
        
        FILE *f = fopen("./saidas/commands1_output.txt", "r");           
        p=file_lines(f);
        printnew (win,i,j,f ,0,p);  
        fclose(f);

}

/**
 * \brief Função case8
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param tabelarepo (STRUCT REPOS)
 * @param tabelacommit (STRUCT COMMITS)
 */
void case8(WINDOW *win,int i,int j,REPOS tabelarepo,COMMITS tabelacommit) 
{
    int p =0;
    char nr[10],d1[20];
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"Querie introuduzida : 8");
        wattroff(win,A_STANDOUT);  
        i+=2; 
        mvwprintw(win,i,j,"Introduza o número  :");
        i++;
        wrefresh(win);
        mvwgetstr(win,i,j,nr);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"O número introduzido foi : %s ",nr);
        wattroff(win,A_STANDOUT);
        wrefresh(win);
        i++;
         mvwprintw(win,i,j,"Introduza a data :");
        i++;
        wrefresh(win);
        mvwgetstr(win,i,j,d1);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"A data introduzida foi : %s ",d1);
        wattroff(win,A_STANDOUT);
        wrefresh(win);
        i+=2;
        
       query8(tabelarepo,tabelacommit,atoi(nr),getdata(d1),1);

        FILE *f = fopen("./saidas/commands1_output.txt", "r");           
        p=file_lines(f);
        printnew (win,i,j,f ,0,p); 
        fclose(f);
}


/**
 * \brief Função case9
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param tuser (STRUCT USER);
 * @param tabelacommit (STRUCT COMMITS)
 * @param tabelarepo (STRUCT REPOS)
 * @param stats1 (STRUCT stats1)
 */
void case9(WINDOW *win,int i,int j,USERS tuser,COMMITS tabelacommit,REPOS tabelarepo,STATS stats1) 
{
    int p =0;
    char nr[10];
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"Querie introuduzida : 9");
        wattroff(win,A_STANDOUT);  
        i+=2; 
        mvwprintw(win,i,j,"Introduza o número  :");
        i++;
        wrefresh(win);
        mvwgetstr(win,i,j,nr);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"O número introduzido foi : %s ",nr);
        wattroff(win,A_STANDOUT);
        wrefresh(win);
        i++;
        
        query9(tuser,tabelacommit,tabelarepo,atoi(nr),get_utilizadores(stats1),1);

        mvwprintw(win,i,j,"entrou ");    
          wrefresh(win);     
        i++; 
        FILE *f = fopen("./saidas/commands1_output.txt", "r"); 
        p=file_lines(f);
        printnew (win,i,j,f ,0,p);  
        fclose(f);

       
 
}

/**
 * \brief Função case10
 * 
 * É uma função auxiliar que ajuda na produção do menu para a querie correspondente.
 
 * @param win (apontador para o ficheiro)
 * @param i auxiliar na construção da janela
 * @param j auxiliar na construção da janela
 * @param tabelacommit (STRUCT COMMITS)
 * @param tabelauser (STRUCT USER);
 */
void case10(WINDOW *win,int i,int j,COMMITS tabelacommit,USERS tabelauser) 
{
    int p =0;
    char nr[10];
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"Querie introuduzida : 10");
        wattroff(win,A_STANDOUT);  
        i+=2; 
        mvwprintw(win,i,j,"Introduza o número  :");
        i++;
        wrefresh(win);
        mvwgetstr(win,i,j,nr);
        wrefresh(win);
        wattron(win,A_STANDOUT);
        mvwprintw(win,i,j,"O número introduzido foi : %s ",nr);
        wattroff(win,A_STANDOUT);
        wrefresh(win);
        i++;
        
       query10(tabelacommit,atoi(nr),tabelauser,1);

       FILE *f = fopen("./saidas/commands1_output.txt", "r");           
        p=file_lines(f);
        printnew (win,i,j,f ,0,p);  
        fclose(f);
        
}


/**
 * \brief Função menu
 * 
 * É uma função principal que produz o menu e ajuda na sepração por queries; 
 *
 * @param stats1 (STRUCT STATS)
 * @param tabelacommit (STRUCT COMMITS)
 * @param tabelarepo (STRUCT REPOS);
 * @param tabelauser (STRUCT USER);
 */
int menu (STATS stats1,COMMITS tabelacommit ,REPOS tabelarepo ,USERS tabelauser) 
{
    int altura ,largura ,sx,sy;
    char str[100],nr[10],d1[20],d2[20],lang[20];


do 
    {
       
        initscr();
        raw();
        sx=sy=0;
        altura = LINES ;
        largura = COLS ;

        keypad(stdscr, TRUE);

        WINDOW *win = newwin (altura,largura ,sx,sy);
        wrefresh(win);


        int i =2;
        int j =1;
        

            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"| Introduza o número correspondente à querie que prentende executar                   |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|1| Quantidade de bots, organizações e utilizadores                                   |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|2| Número médio de colaboradores por repositório                                    |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|3| Quantidade de repositórios com bots                                                |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|4| Quantidade média de commits por utilizador                                         |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|5| Top N de utilizadores mais ativos num determinado intervalo de datas                |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|6| Top N de utilizadores com mais commits em repositórios de uma determinada linguagem |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|7| Repositórios inativos a partir de uma determinada data                             |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|8| Top N de linguagens mais utilizadas a partir de uma determinada data                |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|9| Top N de utilizadores com mais commits em repositórios cujo owner é um amigo seu    |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|10| Top N de utilizadores com as maiores mensagens de commit por repositório          |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
             mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            mvwprintw(win,i,j,"|11| Sair do programa                                                                   |\n");
            i++;
            mvwprintw(win,i,j,"-----------------------------------------------------------------------------------------\n");
            i++;
            wrefresh(win);	
            i++;

        box(win,0,0);
        wattron(win,A_STANDOUT);
        mvwprintw(win,0,2,"Menu");
        mvwprintw(win,0,7,"Guiao3");
        wattroff(win,A_STANDOUT);
        refresh();
        wrefresh(win);

        mvwgetstr(win,i,j,str);
       
        wrefresh(win);

            if (strcmp (str ,"1")==0) 
            {
                case1(win,i,j,stats1);
            }
            if (strcmp (str ,"2")==0) 
            {
                case2(win,i,j,stats1);
            }
            if (strcmp (str ,"3")==0) 
            {
                case3(win,i,j,stats1);
            }
            if (strcmp (str ,"4")==0) 
            {
                case4(win,i,j,stats1);
            }
            if (strcmp (str ,"5")==0) 
            {
                 case5 (win,i, j, tabelauser,tabelacommit) ;
            }
            if (strcmp (str ,"6")==0) 
            {
                case6(win,i,j,tabelauser,tabelarepo,tabelacommit);
            }
            if (strcmp (str ,"7")==0) 
            {
                case7(win,i,j,tabelacommit,tabelarepo);
            }
            else if (strcmp (str ,"8")==0) 
            {
                case8(win,i,j,tabelarepo,tabelacommit);
            }
           else  if (strcmp (str ,"9")==0) 
            {
                case9(win,i,j,tabelauser,tabelacommit,tabelarepo,stats1);
            }
            else if (strcmp (str ,"10")==0) 
            {
                case10(win,i,j,tabelacommit,tabelauser);
            }
           else  
            {
                
                wattron(win,A_STANDOUT);
                mvwprintw(win,i,j, "Prima novamente qualquer tecla para sair !");
                wrefresh(win);
                wattroff(win,A_STANDOUT);
                i++;
            }
    }
    while ( strcmp ((str) ,"11") !=0) ;
        getch();
        endwin();
        return 0 ;

} 