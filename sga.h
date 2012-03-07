/*----------------------------------------------------------------------------*/
/* sga.h - global declarations for main(), all variable declared herein must  */
/*         also be defined as extern variables in external.h !!!              */
/*----------------------------------------------------------------------------*/

#include <sys/times.h>
#include <stdio.h>

#define LINELENGTH 80                                    /* width of printout */
#define BITS_PER_BYTE 8            /* number of bits per byte on this machine */
#define UINTSIZE (BITS_PER_BYTE*sizeof(unsigned))    /* # of bits in unsigned */
//#define _DEBUG_MALLOC_   /* SI ESTA DEFIIDO SE MUESTRA MENSAJES MALLOC Y FREE */

/* file pointers */
FILE 			*outfp, *infp, *evofp, *rprofp, *ralgfp;

char		 	ruta_instancias[100]; 			/* Ruta completa del directorio donde estn los archivos instancias */
char		 	ruta_resultados[100]; 			/* Ruta completa del directorio donde quedarán los archivos resultados */
char		 	arch_reporte_pro[100]; 			/* Archivo de reporte de variables importantes por corrida, relacionada por cada problema a resolver */
char			arch_reporte_alg[100]; 			/* Archivo de reporte de variables importantes por corrida, relacionada con el algoritmo genético a resolver */
char 			arch_evolucion[100]; 			/* Archivo con evolución del Algoritmo Genético por cada corrida */
char 			ruta_salida[100]; 				/* Ruta completa del archivo resultante de salida */
int    			numfiles; 						/* number of open files */
unsigned long 	r250_buffer[250]; 				/* Variable usada en la generacion de la semilla aleatoria */
unsigned long 	r250_index; 					/* Variable usada en la generacion de la semilla aleatoria */
unsigned long 	seed_val; 						/* Variable usada en la generacion de la semilla aleatoria */
int    			tipo_problema;		    		/* variable que indica qué tipo de problema se está considerando */
							  					/* TIPO_PROBLEMA = 0 => PCPG  : Problema de Corte de Pieza Guillotina */
							  					/* TIPO_PROBLEMA = 1 => PCPNG : Problema de Corte de Pieza No Guillotina */      
							  					/* TIPO_PROBLEMA = 2 => PCPI  : Problema de Corte de Pieza Irregulares */      
char 		 	nomarch[100];	  				/* variable donde se almacena nombre de archivo a usar */
int    			popsize;                		/* population size */
int    			maxgen;                 		/* maximum generation number */
float  			pcross;                 		/* probability of crossover */
float  			pmutation;              		/* probability of mutation */
int    			runmax;                 		/* cuenta cantidad de archivos maximos a usar */
int    			run;                    		/* cuenta cantidad de archivos a usar */
int 			largo_cromosoma;				/* Define el largo del cromosoma */
float			fitness_inicial;				/* Define fitness_inicial con el que parte el bestfit */ 
int 		    encabezado_resultado_algoritmo;	/* Variable que determina si encabezado del archivo de resultado del algortmo está impreso o no */
int 		    encabezado_resultado_problema;	/* Variable que determina si encabezado del archivo de resultado del problema está impreso o no */

enum { BINARIO = 0, ENTEROS = 1};
int tipo_elemento_cromosoma;					/* Define el Tipo de elemento de cada cromosoma */

/*Estructura que almacena las pérdidas del individuo */
typedef struct eval
{
   	float  perdida;
   	float  areaocupada;
   	float  calidad;
   	int    piezas;
	float  c_distancia; 		//Componente Distancia
	float  c_perdidareal;	//Componente Perdida
	float  c_digregacion;	//Componente Digregacion
	int    n_perdidas;     	//Número de Perdidas generadas
	double var1;			//Variable Nro.1
}TEval;

/* Global structures and variables */
struct individual 
{
   	unsigned *chrom;    /* cromosoma string for the individual */
   	float    fitness;   /* fitness of the individual */
	TEval 	 PEval;  	/* Estructura con montos de prdida total, calidad, etc..*/
   	int      xsite;     /* crossover site at mating chromosome string */
   	int      msite;     /* crossover site at mating chromosome  mutation*/
  	int      parent[2]; /* who the parents of offspring were */
   	unsigned *chmut;   	/* cromosoma que representa la mutación en las piezas */
    unsigned short *pusListaPiezas; /* cromosoma donde cada gen puede tener un valor mayor o igual a cero */
};
struct bestever
{
   	unsigned *chrom;        /* cromosome string for the best-ever individual */
   	float    fitness;       /* fitness of the best-ever individual */
	TEval 	 PEval;  		/* Estructura con montos de pérdida total, calidad, etc..*/
   	int      generation;    /* generation which produced it */
   	unsigned *chmut;        /* cromosoma mutation que representa las piezas */
    unsigned short *pusListaPiezas; /* cromosoma donde cada gen puede tener un valor mayor o igual a cero */
};

struct indcruz
{
   	unsigned *chrom; /* cromosoma string=>individuo usado para el cruzamiento */
   	unsigned *chmut; /* cromosoma mutac.=>individuo usado para el cruzamiento */
    unsigned short *pusListaPiezas; /* cromosoma donde cada gen puede tener un valor mayor o igual a cero */
};

struct individual  *oldpop;                 /* last generation of individuals */
struct individual  *newpop;                 /* next generation of individuals */
struct individual *tempold;                         /* individuos temporales */
struct bestever bestfit;                         /* fittest individual so far */
struct indcruz indcross;         /* Individuo usado para el operador de cruza */
float  sumfitness;                    /* summed fitness for entire population */
float  max;                                  /* maximum fitness of population */
float  min;                                  /* minumum fitness of population */
float  med;                                  /* medium  fitness of population */
float  varianza;                                  /* varianza de la poblacion */
int    lchrom;                     /* length of the chromosome per individual */
int    lchmut;			   /* length of the mutatin chromosome per individual */
int    chromsize;            /* number of bytes needed to store lchrom string */
int    chmutsize;            /* number of bytes needed to store NumPie string */
int    chlistasize;			         /* number of bytes needed to store lista */
int    gen;                                      /* current generation number */
int    nmutation;          		  /* number of mutations to chromosome string */
int    ncross;            		 /* number of crossovers to chromosome string */
int    mmutation;       		/* number of mutations to chromosome mutation */
int    mcross;          	   /* number of crossovers to chromosome mutation */
float  randomseed;									      /* seed probability */
int bitsxcodigobinario; 	  /* variable donde se almacena cantidad de bits  */
							  /* del Codigo Binario necesarios para almacenar */
				 			  /* los tipos de piezas del problema.            */

// Variables Utilizadas para funcin Elapsed_time
typedef struct {
   	float elapsed_time;		/*Tiempo total transcurrido*/	
	float parent_user;		/*Parent user cpu time consumed*/
	float parent_sys;		/*Parent sys cpu time consumed*/
	float child_user; 		/*Child_user cpu time_consumed*/
	float child_sys; 		/*Child_sys_cpu_time_consumed*/
	float user; 			/*User cpu time*/
	float sys;				/*Sys cpu time*/
	float cpu_time; 		/*Cpu time*/
	float io_time; 			/*IO time*/
} cpu_consummed_time_t;

//Variables para determinar el tiempo transcurrido
struct tms time_start, time_end;
clock_t ticks_start, ticks_end;
cpu_consummed_time_t time_consumtion;


