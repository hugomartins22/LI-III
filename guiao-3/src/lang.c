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


struct lang_arr
{
	char* language;
	int number;
	int pos;
	int size;

};



/**
 * \brief Função add_language
 * 
 * É a função reponsável por adicionar uma linguagem a um array de linguagens. 
 * @param arr (LANG_ARR)
 * @param lang (char*)
 * @param pos (int)
 * 
 */
void add_language(LANG_ARR arr, char* lang)
{
    int pos=arr->size;
		arr->size++;
		arr[pos].language= lang;
        arr[pos].number=0;
		arr[pos].pos=pos;
        arr[pos].size= pos+1;
        
}	

//verifica se a linguagem existe no array. (retorna a posição ou -1 se nao tiver)
/**
 * \brief Função checkLANG
 * 
 * É a função reponsável por verificar se uma linguagem pertence a um array de linguagens. Caso pertença retorna a sua posição, caso não pertença retorna -1.
 * @param arr (LANG_ARR)
 * @param size (int)
 * @param char* (lang)
 * 
 */
int checkLANG(LANG_ARR arr,char* lang)
{
    int size=arr->size,k=0;
	for(int i=0; i<size; i++){
		if (!strcmp(arr[i].language, lang)) 
        {
            k=i;
            return k;
        }
	}
	return -1;
}



/**
 * \brief Função merge_LANG
 * 
 * Função auxiliar da mergeSORT_Lang.
 * @param arr (LANG_ARR)
 * @param l  (int)
 * @param m (int)
 * @param r (int)
 */
void merge_Lang(LANG_ARR arr, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    LANG_ARR L=malloc(sizeof(struct lang_arr)*(n1)), R=malloc(sizeof(struct lang_arr)*(n2));
  
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0;
    j = 0; 
    k = l; 

    while (i < n1 && j < n2) {
        if (L[i].number >= R[j].number) {
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

/**
 * \brief Função mergeSort_Lang
 * 
 * Função responsável pela organização de uma lista de linguagens de modo decrescente.
 * 
 * @param arr (LANG_ARR)
 * @param l (int)
 * @param r (int)
 */
void mergeSort_Lang(LANG_ARR arr, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
  
        mergeSort_Lang(arr, l, m);
        mergeSort_Lang(arr, m + 1, r);
  
        merge_Lang(arr, l, m, r);
    }
}

/**
 * \brief Função init_lang
 * 
 * Função responsável por inicializar o array de linguagens
 * @param size (int)
 */
LANG_ARR init_lang(int size)
{
    LANG_ARR lang_array = malloc(sizeof(struct lang_arr)*(size));

    lang_array->language = "Python";
    lang_array->number=0;
    lang_array->pos=0;
    lang_array->size=1;

    return lang_array;
}

/**
 * \brief Função print_lang
 * 
 * Função responsável por inicializar o array de linguagens
 * @param lang_array STRUCT LANG_ARR
 * @param N inteiro correspondente ao número do ficheiro de saída
 * @param top inteiro correspondente ao número de vezes a imprimir
 */
void print_lang (LANG_ARR lang_array,int N,int top)
{
    char *exitfile=malloc(100*sizeof(char));
    sprintf(exitfile,"saidas/commands%d_output.txt",N);
    FILE *f = fopen (exitfile,"w");

    int siz = lang_array->size-1;
    mergeSort_Lang(lang_array,0,lang_array->size-1);
    
    for (int i=0; i<top && i<siz; i++)
    {
        fprintf(f,"%s\n", lang_array[i].language);
	}
    fclose(f);
}

/**
 * \brief Função incrementa2
 * 
 * Função responsável por incrementar o campo number 
 * @param lang STRUCT LANG_ARR
 * @param pos inteiro
 * @param vezes número de vezes 
 */
void incrementa2(LANG_ARR lang,int pos,int vezes)
{
    lang[pos].number+=vezes;
}
