/*----------------------------------------------------------------------------*/
/* memory.c - Rutinas de administracin de memoria para cdigo PGA            */
/*----------------------------------------------------------------------------*/

#include <stdlib.h>

#include "sga.h"

void nomemory(char *string_ok)
// Imprime error de memoria y sale del programa con error
{
	fprintf(outfp,"malloc: out of memory en %s!!\n",string_ok);
  	exit(-1);
}//End nomemory

void initmalloc(void)
// Localiza memoria para estructuras de datos globales
{
   	unsigned nbytes;
   	int j;

   	// Memoria para nueva y antigua poblacin de individuos
   	nbytes = popsize*sizeof(struct individual);
   	if((oldpop = (struct individual *) malloc(nbytes)) == NULL)
    	nomemory("oldpop");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 29, oldpop, %d\n", nbytes);
#endif
   	if((newpop = (struct individual *) malloc(nbytes)) == NULL)
      	nomemory("newpop");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 31, newpop, %d\n", nbytes);
#endif
  
   	// Memoria para cromosoma string de la población
   	nbytes = chromsize*sizeof(unsigned);
   	for(j = 0; j < popsize; j++) {
      	if((oldpop[j].chrom = (unsigned *) malloc(nbytes)) == NULL)
		   	nomemory("oldpop chromosome string");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 37, oldpop[j].chrom, %d\n", nbytes);
#endif
      	if((newpop[j].chrom = (unsigned *) malloc(nbytes)) == NULL)
		   	nomemory("newpop chromosome string");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 39, newpop[j].chrom, %d\n", nbytes);
#endif
   	}//End for
   
   	if((bestfit.chrom = (unsigned *) malloc(nbytes)) == NULL)
	   	nomemory("bestfit chromosome string");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 42, bestfit.chrom, %d\n", nbytes);
#endif
   	if((indcross.chrom = (unsigned *) malloc(nbytes)) == NULL)
      	nomemory("indcross chromosome string");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 44, indcross.chrom, %d\n", nbytes);
#endif

   	// Memoria para cromosoma mutación de la población
   	nbytes = chmutsize*sizeof(unsigned);
   	for(j = 0; j < popsize; j++) {
      	if((oldpop[j].chmut = (unsigned *) malloc(nbytes)) == NULL)
		   	nomemory("oldpop chromosome mutation");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 50, oldpop[j].chmut, %d\n", nbytes);
#endif
      	if((newpop[j].chmut = (unsigned *) malloc(nbytes)) == NULL)
			nomemory("newpop chromosome mutation");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 52,newpop[j].chmut, %d\n", nbytes);
#endif
   	}//End for
   
   	if((bestfit.chmut = (unsigned *) malloc(nbytes)) == NULL)
	   	nomemory("bestfit chromosome mutation");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 55, bestfit.chmut, %d\n", nbytes);
#endif
   	if((indcross.chmut = (unsigned *) malloc(nbytes)) == NULL)
      	nomemory("bestfit chromosome mutation");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 57, indcross.chmut, %d\n", nbytes);
#endif

   	// Memoria para cromosoma Lista de la población
   	nbytes = chlistasize*sizeof(unsigned short);
   	for(j = 0; j < popsize; j++) {
      	if((oldpop[j].pusListaPiezas = (unsigned short *) malloc(nbytes)) == NULL)
		   	nomemory("oldpop chromosome Lista");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 91, oldpop[j].pusListaPiezas, %d\n", nbytes);
#endif
      	if((newpop[j].pusListaPiezas = (unsigned short *) malloc(nbytes)) == NULL)
			nomemory("newpop chromosome Lista");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 98,newpop[j].pusListaPiezas, %d\n", nbytes);
#endif
   	}//End for
   
   	if((bestfit.pusListaPiezas = (unsigned short *) malloc(nbytes)) == NULL)
	   	nomemory("bestfit chromosome Lista");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 105, bestfit.pusListaPiezas, %d\n", nbytes);
#endif
   	if((indcross.pusListaPiezas = (unsigned short *) malloc(nbytes)) == NULL)
      	nomemory("bestfit chromosome Lista");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, memory.c, 110, indcross.pusListaPiezas, %d\n", nbytes);
#endif
}//End initmalloc

void freeall(void)
// Rutina que libera toda el espacio de memoria localizado dinmicamente
{
   	int i;
  
   	//Libera oldpop
   	for(i = 0; i < popsize; i++) {
      	free(oldpop[i].chrom);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 123, oldpop[i].chrom\n");
#endif
		free(oldpop[i].chmut);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 127, oldpop[i].chmut\n");
#endif
		free(oldpop[i].pusListaPiezas);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 131, oldpop[i].pusListaPiezas\n");
#endif
   	}//End for
  
   	free(oldpop);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 98, oldpop\n");
#endif

   	//Libera newpop
   	for(i = 0; i < popsize; i++) {
      	free(newpop[i].chrom);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 94, newpop[i].chrom\n");
#endif
      	free(newpop[i].chmut);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 95, newpop[i].chmut\n");
#endif
      	free(newpop[i].pusListaPiezas);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 152, newpop[i].pusListaPiezas\n");
#endif
   	}//End for
   
   	free(newpop);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 99, newpop\n");
#endif

   	//Libera bestfit
   	free(bestfit.chrom);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 100, bestfit.chrom\n");
#endif
   	free(bestfit.chmut);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 101, bestfit.chmut\n");
#endif
   	free(bestfit.pusListaPiezas);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 172, bestfit.pusListaPiezas\n");
#endif
   
   	//Libera indcross
   	free(indcross.chrom);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 103, indcross.chrom\n");
#endif
   	free(indcross.chmut);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 104, indcross.chmut\n");
#endif
   	free(indcross.pusListaPiezas);
#ifdef _DEBUG_MALLOC_		
   printf("Free, memory.c, 186, indcross.pusListaPiezas\n");
#endif
}//End freeall

