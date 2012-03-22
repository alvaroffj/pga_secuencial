/*----------------------------------------------------------------------------*/
/* generate.c - crea una nueva generacin de individuos                       */
/*----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "sga.h"
#include "random.h"
#include "app.h"

void statistics(struct individual *pop, int corrida)
// Calcula Estadstica de la poblacin
{
   	int i, j, k=-1;

   	min = pop[0].fitness;
   	max = pop[0].fitness;
	med = 0;
	varianza = 0;

   	// Loop para max, min
   	for(j = 0; j < popsize; j++) {
      	if(pop[j].fitness > max) max = pop[j].fitness;         // Nuevo máximo
      	if(pop[j].fitness < min) min = pop[j].fitness;         // nuevo mínimo
      	med = med + pop[j].fitness; //Suma de fitness
		varianza = varianza + (pop[j].fitness * pop[j].fitness); //calculo de la varianza
      	// Nuevo mejor individuo global
      	if(pop[j].fitness < bestfit.fitness) {
		 	bestfit.fitness = pop[j].fitness;
			k = j; //Quiere decir que se actualizó mejor individuo
		}//End if
   	}//End for
	med = (float) (med / (float) popsize);
	varianza = ((float) (varianza / (float) popsize)) - (med * med);
	// Si actualizó mejor individuo se pasan los datos restantes del mejor individuo.
   	if(k >= 0) {
		for(i = 0; i < chlistasize; i++) // Actualiza Cromosoma Lista
            bestfit.pusListaPiezas[i] = pop[k].pusListaPiezas[i];
      	for(i = 0; i < chromsize; i++) // Actualiza Cromosoma String
 		   	bestfit.chrom[i] = pop[k].chrom[i];
      	for(i = 0; i < chmutsize; i++)// Actualiza Cromosoma Mutación
 		   	bestfit.chmut[i] = pop[k].chmut[i];
  		bestfit.fitness    = pop[k].fitness;
  		bestfit.PEval      = pop[k].PEval;
      	bestfit.generation = gen;
   	}//End if

   	// Escribe en archivo de Estaditicas para problema particular
	fprintf(evofp, "%d;%d;%f;%f;%f;%f;%f\n", corrida, gen, min, max, med, varianza,bestfit.generation);
}//End statistics

void initpop(int tipo_problema, int corrida)
// Inicializa la poblacin inicial en forma aleatoria
{
  	int j, j1, k, stop;
   	unsigned mask = 1, numero_gen;
   	
   	//Define por default que cada elemento del cromosoma del Algoritmo Genértico es del tipo binario
	tipo_elemento_cromosoma = BINARIO;
	//*** NEW
	//if(tipo_problema == 2) tipo_elemento_cromosoma = ENTEROS; //Tipo_Problema = Corte de Piezas Irregulares con cromosoma con ENTEROS =: cada elemento del cromosoma es del tipo ENTEROS
   	//***
   	
   	if (tipo_elemento_cromosoma == BINARIO){
		//Para cromosomas binarios (0,1)
		for(j = 0; j < popsize; j++) {
	        //*** NEW
	        for(k = 0; k < chlistasize; k++) {
	            oldpop[j].pusListaPiezas[k] = 0;
	        }//End for
			//numero_gen = rnd( 1, chlistasize ) - 1; //Determina al azar numero_gen entre 0 y largo_cromosoma-1
	        ////El siguiente for coloca en la primera posicion el numero_gen y luego incrementa el valor de numero_gen en 1
	        ////Dado un cromosoma de 10 gen desde el 0 al 9 y el numero_gen = 7 
	        ////pos0 pos1 pos2 pos3 pos4 pos5 pos6 pos7 pos8 pos9
	        ////7	   8    9    0    1    2    3    4    5    6
	        //for(k = 0; k < chlistasize; k++) {
	        //    oldpop[j].pusListaPiezas[ k ] = numero_gen;
			//	numero_gen++;
			//	numero_gen=(numero_gen==chlistasize)?0:numero_gen;
	        //}//End for
	        //***
	        
			// Inicializa cromosoma de string
	      	for(k = 0; k < chromsize; k++) {
	         	oldpop[j].chrom[k] = 0;
	         	if(k == (chromsize-1))
	            	stop = lchrom - (k*UINTSIZE);
	         	else
	            	stop = UINTSIZE;
	         	for(j1 = 1; j1 <= stop; j1++) {
	            	oldpop[j].chrom[k] = oldpop[j].chrom[k]<<1;
	            	if(flip(0.5))
	               		oldpop[j].chrom[k] = oldpop[j].chrom[k]|mask;
	         	}//End for
	      	}//End for
			// Inicializa cromosoma de mutación
	      	for(k = 0; k < chmutsize; k++) {
	        	oldpop[j].chmut[k] = 0;
	         	if(k == (chmutsize-1))
	            	stop = lchmut - (k*UINTSIZE);
	         	else
	            	stop = UINTSIZE;
	         	for(j1 = 1; j1 <= stop; j1++) {
	            	oldpop[j].chmut[k] = oldpop[j].chmut[k]<<1;
	            	if(flip(0.5))
	               		oldpop[j].chmut[k] = oldpop[j].chmut[k]|mask;
	         	}//End for
			}//End for
	      	oldpop[j].parent[0]  = 0; // Initializa información de los padres
	      	oldpop[j].parent[1]  = 0;
	      	oldpop[j].xsite      = 0;
	      	oldpop[j].msite      = 0;
	   	  	app_objfunc(tipo_problema, &(oldpop[j]));  // Evalua fitness inicial
	   	}//End for
   	}//End if
	else if (tipo_elemento_cromosoma == ENTEROS){
		//Para cromosomas donde cada gen puede tener un valor >= 0
	    for(j = 0; j < popsize; j++) {
			numero_gen = rnd( 1, chlistasize ) - 1; //Determina al azar numero_gen entre 0 y largo_cromosoma-1
	        //El siguiente for coloca en la primera posicion el numero_gen y luego incrementa el valor de numero_gen en 1
	        //Dado un cromosoma de 10 gen desde el 0 al 9 y el numero_gen = 7 
	        //pos0 pos1 pos2 pos3 pos4 pos5 pos6 pos7 pos8 pos9
	        //7	   8    9    0    1    2    3    4    5    6
	        for(k = 0; k < chlistasize; k++) {
	            oldpop[j].pusListaPiezas[ k ] = numero_gen;
				numero_gen++;
				numero_gen=(numero_gen==chlistasize)?0:numero_gen;
	        }//End for
			// Inicializa cromosoma de string
	      	for(k = 0; k < chromsize; k++) 
	         	oldpop[j].chrom[k] = 0;
			// Inicializa cromosoma de mutación 
	      	for(k = 0; k < chmutsize; k++)
	        	oldpop[j].chmut[k] = 0;
	        oldpop[j].parent[0] = 0; // Initializa información de los padres
	        oldpop[j].parent[1] = 0;
	        oldpop[j].xsite 	= 0;
	      	oldpop[j].msite     = 0;
	        app_objfunc(tipo_problema, &(oldpop[j]));  // Evalua fitness inicial
	    }//End for
	}//End else
	
   	// Efecta estadsticas sobre población oldpop y obtiene mejor individuo
    statistics(oldpop, corrida);
}//End initpop

void mutation(struct individual *critter)
// Muta cromosoma string con probabilidad pmutation, cuenta # de mutaciones = nrommut
// Muta cromosoma mutacion con probabilidad pmutation, cuenta # de mutaciones = nmutmut
{
   	int j, k, stop;
   	unsigned mask;
   	unsigned short aux;
#define MUTACION_1_BIT		0
#define MUTACION_MASCARA	1
#define TIPO_MUTACION 		MUTACION_MASCARA

	if (tipo_elemento_cromosoma == BINARIO){
	   	if(flip(pmutation)) {
#if (TIPO_MUTACION ==	MUTACION_1_BIT)
			int jrommut, jmutmut, indice;
			unsigned temp;
	    	// Muta cromosoma string con probabilidad pmutation, cuenta # de mutaciones = nrommut
	      	jrommut = rnd(0 ,(lchrom - 1));// Mutacin entre 1 y l-1
			indice = 0;
	    	temp = 1;
	      	for(k = 0; k < chromsize; k++) {
	         	mask = 0;
	         	if(k == (chromsize-1)) 
	            	stop = lchrom - (k*UINTSIZE);
	         	else
	            	stop = UINTSIZE;
	         	for(j = 0; j < stop; j++) {
					if(jrommut == indice) {
	     				mask = mask|(temp<<j);
	     			   	nmutation++;
					   	j = stop;
	            	}//End if
	            	indice++;
	         	}//End for
	         	critter->chrom[k] = critter->chrom[k]^mask;
	      	}//End for
	
	    	// Muta cromosoma mutacion con probabilidad pmutation, cuenta # de mutaciones = nmutmut
	    	// Se muta sin restricciones, no importa si la mutacin sobre la pieza (giro) me da una pieza
	    	// que no cabe en la lmina, esto lo ve la funcin objetivo y no debera tomar esta pieza.
	      	jmutmut = rnd(0 ,(lchmut - 1));// Mutacin entre 1 y l-1
			indice = 0;
	    	temp = 1;
	      	for(k = 0; k < chmutsize; k++) {
	         	mask = 0;
	         	if(k == (chmutsize-1))
	            	stop = lchmut - (k*UINTSIZE);
	         	else
	            	stop = UINTSIZE;
	         	for(j = 0; j < stop; j++) {
					if(jmutmut == indice) {
	    				mask = mask|(temp<<j);
	     		      	mmutation++;
				   		j = stop;
	            	}//End if
					indice++;
	         	}//End for
	         	critter->chmut[k] = critter->chmut[k]^mask;
	      	}//End for
#elif (TIPO_MUTACION ==	MUTACION_MASCARA)
			mask = 1;
			//Mutación Uniforme en cromosoma String
			//Se genera máscara individuo mutación cromosoma string en forma aleatoria
	      	nmutation++;
	      	for(k = 0; k < chromsize; k++) {
	         	indcross.chrom[k] = 0;
	         	if(k == (chromsize-1))
	            	stop = lchrom - (k*UINTSIZE);
	         	else
	            	stop = UINTSIZE;
	         	for(j = 0; j < stop; j++) {
	            	indcross.chrom[k] = indcross.chrom[k]<<1;
	            	if(flip(0.5))
	               		indcross.chrom[k] = indcross.chrom[k]|mask;
	         	}//End for
				critter->chrom[k] = critter->chrom[k]^indcross.chrom[k];
	      	}//End for
	
	    	// Mutación Uniforme Cromosoma Rotación
			//Se genera máscara individuo mutación cromosoma string en forma aleatoria
	      	mmutation++;
	      	for(k = 0; k < chmutsize; k++) {
	         	indcross.chmut[k] = 0;
	         	if(k == (chmutsize-1))
	            	stop = lchmut - (k*UINTSIZE);
	         	else
	            	stop = UINTSIZE;
	         	for(j = 0; j < stop; j++) {
	            	indcross.chmut[k] = indcross.chmut[k]<<1;
	            	if(flip(0.5))
	               		indcross.chmut[k] = indcross.chmut[k]|mask;
	         	}//End for
				critter->chmut[k] = critter->chmut[k]^indcross.chmut[k];
	      	}//End for
#endif
	   	}//End if
	}//End if
   	else if (tipo_elemento_cromosoma == ENTEROS){
   		//Mutación corresponde a realizar una permutación dentro del cromosoma lista.
		for ( j = 0;j < chlistasize; j++){
			if ( flip( pmutation ) ){
				k = rnd( 1, chlistasize ) - 1;
				if ( j != k ){
					aux = critter->pusListaPiezas[ j ];     
					critter->pusListaPiezas[ j ] = critter->pusListaPiezas[ k ];
					critter->pusListaPiezas[ k ] = aux;
					nmutation++;
				}//End if
			}//End if
		}//End for
	}//End else if
}//End mutation


//for ( k = uInicio; k <= uFin; k++ ){
//	IntercambiaPiezas( k, iBuscaPieza( lparent1[k], lchild2 ), lchild2 );
//	IntercambiaPiezas( k, iBuscaPieza( lparent2[k], lchild1 ), lchild1 );
//}//End for
int iBuscaPieza( unsigned short uiPieza, unsigned short *puiPiezas )
//Busca el Valor de una Pieza dentro del cromosoma Lista y retorna la posición de esa Pieza
{
	int i;
	
	for ( i = 0; i < chlistasize; i++)
		if ( puiPiezas[ i ] == uiPieza )
			return( i );
	fprintf(outfp,"Error en iBuscaPieza\n");
  	exit(-1);
}//End iBuscaPieza

void IntercambiaPiezas( int i, int j, unsigned short *puiPiezas )
//Intercambia una pieza por otra dentro del mismo cromosoma
{
	unsigned int uiAuxiliar;

	uiAuxiliar = puiPiezas[ i ];
	puiPiezas[ i ] = puiPiezas[ j ];
	puiPiezas[ j ] = uiAuxiliar;
}//End IntercambiaPiezas

void xcrossover (unsigned *parent1,unsigned *parent2,unsigned *child1,unsigned *child2,
		  		 unsigned *mparent1,unsigned *mparent2,unsigned *mchild1,unsigned *mchild2)
// Función de Cruzamiento de 2 string padres, se transforma en 2 string hijos
// Cruzamiento del Cromosoma String
{
   	int j1, k, stop;
   	unsigned mask=1;

	if(flip(pcross)) {
		// Cruzamiento Cromosoma String
		ncross++;
		//Primero genero máscara individuo cruzamiento cromosoma string en forma aleatorio
      	for(k = 0; k < chromsize; k++) {
         	indcross.chrom[k] = 0;
         	if(k == (chromsize-1))
            	stop = lchrom - (k*UINTSIZE);
         	else
            	stop = UINTSIZE;
         	for(j1 = 1; j1 <= stop; j1++) {
            	indcross.chrom[k] = indcross.chrom[k]<<1;
            	if(flip(0.5))
               		indcross.chrom[k] = indcross.chrom[k]|mask;
         	}//End for
      	}//End for
  		
      	// Cruzamiento Uniforme
  		// Luego con ese individuo voy viendo :
  		// Si bit mscara es cero
  		// 			hijo1 dejo bit padre2 y en hijo2 dejo bit padre1
  		// Si bit mscara es Uno
  		// 			hijo1 dejo bit padre1 y en hijo2 dejo bit padre2
      	for(k = 0; k < chromsize; k++) {
        	child1[k] = (parent1[k]&(~indcross.chrom[k]))|(parent2[k]&( indcross.chrom[k]));
         	child2[k] = (parent1[k]&( indcross.chrom[k]))|(parent2[k]&(~indcross.chrom[k]));
      	}//End for

    	// Cruzamiento Cromosoma Mutación
		mcross++;
		//Primero genero máscara individuo cruzamiento cromosoma mutacin en forma aleatorio
      	for(k = 0; k < chmutsize; k++) {
         	indcross.chmut[k] = 0;
         	if(k == (chmutsize-1))
            	stop = lchmut - (k*UINTSIZE);
         	else
            	stop = UINTSIZE;
         	for(j1 = 1; j1 <= stop; j1++) {
            	indcross.chmut[k] = indcross.chmut[k]<<1;
            	if(flip(0.5))
               		indcross.chmut[k] = indcross.chmut[k]|mask;
         	}//End for
      	}//End for
				
      	// Cruzamiento Uniforme
 		// Luego con ese individuo voy viendo :
 		// Si bit mscara es cero
 		// 			hijo1 dejo bit padre2 y en hijo2 dejo bit padre1
 		// Si bit mscara es Uno
 		// 			hijo1 dejo bit padre1 y en hijo2 dejo bit padre2
      	for(k = 0; k < chmutsize; k++) {
         	mchild1[k] = (mparent1[k]&(~indcross.chmut[k]))|(mparent2[k]&( indcross.chmut[k]));
         	mchild2[k] = (mparent1[k]&( indcross.chmut[k]))|(mparent2[k]&(~indcross.chmut[k]));
      	}//End for
   	}//End if
   	else {
      	for(k = 0; k < chromsize; k++) {
         	child1[k] = parent1[k];
         	child2[k] = parent2[k];
      	}//End for
      	for(k = 0; k < chmutsize; k++) {
         	mchild1[k] = mparent1[k];
         	mchild2[k] = mparent2[k];
      	}//End for
   	}//End else
}//End xcrossover

// Funcin de Cruzamiento de 2 string padres, se transforma en 2 string hijos
// Cruzamiento del String Cromosoma
int crossover (unsigned *parent1,unsigned *parent2,unsigned *child1,unsigned *child2,
               unsigned *mparent1,unsigned *mparent2,unsigned *mchild1,unsigned *mchild2,
               unsigned short *lparent1,unsigned short *lparent2,unsigned short *lchild1,unsigned short *lchild2) {
    int j, jcross, kcross, k;
    int i;
    unsigned mask, temp;
    unsigned uInicio, uFin;
    printf("0.1\n");
/*
    printf("parent1 %d: ",chromsize);
    for(i=0; i<chromsize; i++) {
        printf(" %d", parent1[i]);
    }
    printf("\n");
*/
    if (tipo_elemento_cromosoma == BINARIO) {
        printf("0.2\n");
        //Realiza cruzamiento cromosoma cruzamiento y cromosoma rotación
        //Hace cruzamiento con probabilidad pcross de la población
        if(flip(pcross)) {
                //Cruzamiento Cromosoma String
            jcross = rnd(1 ,(lchrom - 1));// Cruzamiento entre 1 y l-1
/*
            jcross = (int) ceil((float) lchrom/ 2.0);// GMO
*/
            ncross++;
            printf("0.3 %d %d\n", lchrom, jcross);
            for(k = 1; k <= chromsize; k++) {
                printf("0.3.1\n");
                if(jcross >= (k*UINTSIZE)) {
                    printf("0.3.1a: %d\n", k);
                    child1[k-1] = parent1[k-1];
                    printf("*0.3.1a: %d\n", k);
                    child2[k-1] = parent2[k-1];
                    printf("**0.3.1a: %d\n", k);
                } else if((jcross < (k*UINTSIZE)) && (jcross > ((k-1)*UINTSIZE))) {
                    printf("0.3.1b\n");
                    mask = 1;
                    for(j = 1; j <= (jcross-1-((k-1)*UINTSIZE)); j++) {
                        temp = 1;
                        mask = mask<<1;
                        mask = mask|temp;
                    }//End for
                    child1[k-1] = (parent1[k-1]&mask)|(parent2[k-1]&(~mask));
                    child2[k-1] = (parent1[k-1]&(~mask))|(parent2[k-1]&mask);
                } else {
                    printf("0.3.1c\n");
                    child1[k-1] = parent2[k-1];
                    child2[k-1] = parent1[k-1];
                }//End else
                printf("0.3.2\n");
            }//End for
            printf("0.4\n");
            //Cruzamiento Cromosoma Mutacion
            kcross = jcross/bitsxcodigobinario;
            if(jcross%bitsxcodigobinario) kcross++;
            mcross++;
            for(k = 1; k <= chmutsize; k++) {
                if(kcross >= (k*UINTSIZE)) {
                    mchild1[k-1] = mparent1[k-1];
                    mchild2[k-1] = mparent2[k-1];
                } else if((kcross < (k*UINTSIZE)) && (kcross > ((k-1)*UINTSIZE))) {
                    mask = 1;
                    for(j = 1; j <= (kcross-1-((k-1)*UINTSIZE)); j++) {
                        temp = 1;
                        mask = mask<<1;
                        mask = mask|temp;
                    }//End for
                    mchild1[k-1] = (mparent1[k-1]&mask)|(mparent2[k-1]&(~mask));
                    mchild2[k-1] = (mparent1[k-1]&(~mask))|(mparent2[k-1]&mask);
                } else {
                    mchild1[k-1] = mparent2[k-1];
                    mchild2[k-1] = mparent1[k-1];
                }//End else
            }//End for
            printf("0.5\n");
        } else {
            for(k = 0; k < chromsize; k++) {
                child1[k] = parent1[k];
                child2[k] = parent2[k];
            }//End for
            for(k = 0; k < chmutsize; k++) {
                mchild1[k] = mparent1[k];
                mchild2[k] = mparent2[k];
            }//End for
            jcross = 0;
        }//End else
    }//End if
    else if (tipo_elemento_cromosoma == ENTEROS){
            //Realiza cruzamiento de cromosoma Lista
            //Se utiliza cruzamiento PMX (Partially Mapped Crossover)
        if(flip(pcross)){
                    jcross = uInicio = rnd( 1, chlistasize - 1 );
                    uFin = rnd( uInicio--, chlistasize ) - 1;
                    //Copia lparent1 en lchild1 y lparent2 en lchild2	
                    memcpy( lchild1, lparent1, sizeof( unsigned short ) * chlistasize );
                    memcpy( lchild2, lparent2, sizeof( unsigned short ) * chlistasize );
                    //Intercabia las Piezas desde uInicio y uFin
                    if(uFin<jcross){
                            jcross=uInicio;
                    }
                    for ( k = uInicio; k <= uFin; k++ ){
                            IntercambiaPiezas( k, iBuscaPieza( lparent1[k], lchild2 ), lchild2 );
                            IntercambiaPiezas( k, iBuscaPieza( lparent2[k], lchild1 ), lchild1 );
                    }//End for
        }//End if
        else {
            for(k = 0; k < chlistasize; k++) {
                lchild1[k] = lparent1[k];
                lchild2[k] = lparent2[k];
            }//End for
            jcross = 0;
        }//End else
    }//End else if
    return(jcross);
}//End crossover

void preselect(void)
// Obtiene suma total del fitness para oldpop
{
   	int j;

   	sumfitness = 0.0;
   	for(j = 0; j < popsize; j++) sumfitness += (((double) 1.0) /(double) oldpop[j].fitness);
}//End preselect

int xselect(void)
// roulette-wheel selection que considera variables del tipo float
{
   	double sum, pick;
   	int i;

   	pick = (double) randomperc();
   	sum = 0.0;

   	if(sumfitness != 0.0) {
      	for(i = 0; (sum < pick) && (i < popsize); i++)
         	sum += (double) (((double) 1.0) /(double) oldpop[i].fitness)/((double) sumfitness);
   	}//End if
   	else
      	i = rnd(1,popsize);
   	return(i-1);
}//End xselect

// Rutina que establece la generación de nuevos individuos
void generation(int tipo_problema, int corrida) {
    printf("generation\n");
    int mate1, mate2, jcross=0, jmcross=0, j = 0;
    // Obtiene suma total del fitness para oldpop
    preselect();
    printf("-1\n");

   	// Efecta selección, cruzamiento y mutación
    do {
      	// obtiene índices de individuos en oldpop a efectuar cruzamiento
      	mate1 = xselect();
      	mate2 = xselect();
        printf("0\n");
        printf("chromsize %d: \n",chromsize);
//-----------------------------------------------------------------                
        int i, stop, tp, rt, k, bitpos, mask = 1;
        printf("oldpop %d: ", mate1);
        for (i = 0; i < chromsize; i++) {
            if (i == (chromsize - 1)) //ultimo bloque
                stop = lchrom - (i * UINTSIZE);
            else
                stop = UINTSIZE;

            rt = oldpop[mate1].chrom[i];
            for (k = 0; k < stop; k++) {
                bitpos = k + UINTSIZE*i;
                if (rt & mask) printf("1");
                else printf("0");
                rt = rt >> 1;
            }
        }
        printf("\n");
        printf("newpop %d: ", j);
        for (i = 0; i < chromsize; i++) {
            if (i == (chromsize - 1)) //ultimo bloque
                stop = lchrom - (i * UINTSIZE);
            else
                stop = UINTSIZE;

            tp = newpop[j].chrom[i];
            for (k = 0; k < stop; k++) {
                bitpos = k + UINTSIZE*i;
                if (tp & mask) printf("1");
                else printf("0");
                tp = tp >> 1;
            }
        }
        printf("\n");
//-----------------------------------------------------------------
		//Cruzamiento en un solo punto para cromosoma String, cromosoma Mutación, cromosoma Lista
      	jcross = crossover(oldpop[mate1].chrom, oldpop[mate2].chrom, newpop[j].chrom, newpop[j+1].chrom,
                            oldpop[mate1].chmut, oldpop[mate2].chmut, newpop[j].chmut, newpop[j+1].chmut,
                            oldpop[mate1].pusListaPiezas, oldpop[mate2].pusListaPiezas, newpop[j].pusListaPiezas, newpop[j+1].pusListaPiezas);

	printf("1\n");
      	mutation(&(newpop[j]));
        mutation(&(newpop[j+1]));
	printf("2\n");
      	// Decodifica string, evalúa fitness y guarda parentesco de ambos hijos
        app_objfunc(tipo_problema, &(newpop[j]));
        printf("3\n");
      	newpop[j].parent[0] = mate1+1;
      	newpop[j].xsite = jcross;
        jmcross = jcross/bitsxcodigobinario;
        if(jcross%bitsxcodigobinario) jmcross++;
        newpop[j].msite = jmcross;
      	newpop[j].parent[1] = mate2+1;
        printf("4\n");
      	app_objfunc(tipo_problema, &(newpop[j+1]));
      	newpop[j+1].parent[0] = newpop[j].parent[0];
      	newpop[j+1].xsite = newpop[j].xsite;
      	newpop[j+1].msite = newpop[j].msite;
      	newpop[j+1].parent[1] = newpop[j].parent[1];
        printf("5\n");
      	// Incrementa índice de la población
      	j = j + 2;
    } while(j < (popsize-1));
    // Efecta estadísticas sobre nueva población y obtiene mejor individuo
    printf("6\n");
    statistics(newpop, corrida);
    printf("*generation\n");
}//End generation


