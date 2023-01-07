#define MAX_SIZE 200000
typedef char *KEY;


typedef struct data * DATA;

int compare_DATAS(DATA data1, DATA ref, int N);
int comparedate ( DATA  d1,  DATA  d2);
int comparedate2 ( DATA  d1,  int d2ano , int d2mes,int d2dia );
void printdata (DATA date );
DATA getdata (char*date);
DATA initdata ();
DATA insere_Data (DATA recente,DATA date);
int get_ano(DATA date);
int get_mes(DATA date);
int get_dia(DATA date);
DATA init_data_wnums(int ano, int mes, int dia);
