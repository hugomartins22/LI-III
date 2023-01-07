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


struct data
{
	int ano;
	int mes;
	int dia; 

};


/**
 * \brief Função initdata
 * 
 * Função responsável por inicializar uma struct do tipo data
 *
 */
DATA initdata ()
{
    DATA data = malloc(sizeof(struct data));
      data->ano=0;
      data->mes=0;
      data->dia=0;

    return data;
}

DATA init_data_wnums(int ano, int mes, int dia)
{
    DATA data = malloc(sizeof(struct data));
    data->ano=ano;
    data->mes=mes;
    data->dia=dia;
    return data;
}


/**
 * \brief Função comparedate2
 * 
 * É a função reponsável por comparar duas datas
 * @param d1 primeira data a comparar
 * @param d2ano ano
 * @param d2mes mes
 * @param d2dia dia
 *
 */
int comparedate2 ( DATA  d1,  int d2ano , int d2mes,int d2dia )
{
    if (d1->ano < d2ano)
        return 1;
    if (d1->ano == d2ano && d1->mes < d2mes)
        return 1;
    if (d1->ano == d2ano && d1->mes == d2mes &&
                              d1->dia < d2dia)
        return 1;
  
    // If none of the above cases satisfy, return false
	
    return 0;
}


/**
 * \brief Função comparedate
 * 
 * É a função reponsável por comparar duas datas
 * @param d1 primeira data a comparar
 * @param d2 segunda data a comparar
 */
int comparedate ( DATA  d1,  DATA  d2)
{
    if (d1->ano < d2->ano)
        return 1;
    if (d1->ano == d2->ano && d1->mes < d2->mes)
        return 1;
    if (d1->ano == d2->ano && d1->mes == d2->mes &&
                              d1->dia < d2->dia)
        return 1;
  
    return 0;
}
/**
 * \brief Função printdata
 * 
 * É a função reponsável por imprimir uma data
 * @param date data a imprimir
 *
 */
void printdata (DATA date) {
    printf ("ano %d|",date->ano );
    printf ("mes %d|",date->mes );
    printf ("dia %d|\n",date->dia );

}


/**
 * \brief Função getdata
 * 
 * É a função reponsável por converter uma string para uma struct data
 * @param date string a converter
 */
DATA getdata (char*date)  
{
	DATA datafinal = malloc(sizeof(struct data));
   // DATA copy = malloc(sizeof(struct data));
	char *secure,*linecopy,*token;
	linecopy= strdup (date);
    secure=linecopy;

	int collumn=0;
	
 	while (token = strsep (&linecopy,"-"))
        {
        if (collumn ==0) datafinal->ano = atoi(token);
        if (collumn ==1) datafinal->mes = atoi(token);
        if (collumn ==2) datafinal->dia = atoi(token);
        collumn++;
        }
       /*copy->ano = datafinal->ano;
        copy->mes = datafinal->mes;
        copy->dia = datafinal->dia;
         */
    free(secure);
	return datafinal;
}


/**
 * \brief Função compare_DATAS
 * 
 * É a função reponsável por comparar a data 1 com a data 2 de acordo com o Parametro N. N define se é para ser data1>=ref(data depois de ref é N=0) ou data1<=ref(data antes de ref é N=1)
 * @param data1 (DATA)
 * @param ref (DATA)
 * @param N (int) 
 */
int compare_DATAS(DATA data1, DATA ref, int N){

	int soma1, somaref,
	data1_ano,
	data1_mes,
	data1_dia,
	dataref_ano,
	dataref_mes,
	dataref_dia;

	data1_ano= data1->ano;
	data1_mes= data1->mes;
	data1_dia= data1->dia;

	dataref_ano= ref->ano;
	dataref_mes= ref->mes;
	dataref_dia= ref->dia;

	soma1 = data1_ano*365 + ((data1_mes-1)*30) + data1_dia;
	somaref= dataref_ano*365 + ((dataref_mes-1)*30) + dataref_dia;

	//data depois de ref é 0) ou <=(data antes de ref é 1)
	if (soma1 >= somaref && N==0) return 1;
	else if (soma1 >= somaref && N==1) return 0;
	else if (soma1 <= somaref && N==0) return 0;
	else if (soma1 <= somaref && N==1) return 1;
}

/**
 * \brief Função insere_Data
 * 
 * Função responsável por preencher uma data .
 * 
 * @param recente (STRUCT DATA)
 * @param date (STRUCT DATA)
 */
DATA insere_Data (DATA recente,DATA date)
{
	recente->ano = date->ano ;
    recente->mes = date->mes ;
    recente->dia = date->dia ;
	return recente;
}


/**
 * \brief Função get_ano
 * 
 * Função responsável por retornar o ano correspondente a uma data.
 * 
 * @param date (STRUCT DATA)
 * @returns int
 */
int get_ano(DATA date)
{
    int a=0;
    a=date->ano;
    return a;
}

/**
 * \brief Função get_mes
 * 
 * Função responsável por retornar o mes correspondente a uma data.
 * 
 * @param date (STRUCT DATA)
 * @returns int
 */
int get_mes(DATA date)
{
    int m=0;
    m = date->mes;
    return m;
}

/**
 * \brief Função get_dia
 * 
 * Função responsável por retornar o dia correspondente a uma data.
 * 
 * @param date (STRUCT DATA)
 * @returns int
 */
int get_dia(DATA date)
{
    int d =0;
    d = date->dia;
    return d;
}