/*----------------------------------------------------------------------------*/
/* app2_g.c - rutinas para Problema de Corte de Piezas Guillotina             */
/*----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <sys/times.h>
#include <string.h>

#include "sga.h"
#include "memory.h"
#include "var_pcp.h"

void app2_liberamemlistaperdidas_g(void)
// Libera memoria de la lista de prdidas y/o ganancias
{
	TListaPE2 *LAux2;
   	LAux2 = LPer2;

   	if(LAux2 == NULL) return;
   	while(LAux2 != NULL) {
      	LPer2 = LAux2->prox;
      	free(LAux2);
#ifdef _DEBUG_MALLOC_		
   	printf("Free, app2_g.c, 24, LAux2\n");
#endif
    LAux2 = LPer2;
   	}//End while
}//End app2_liberamemlistaperdidas_g

void app2_agregalistap_g(int x, int y, int anc, int alt, char tipo, int t_pieza)
// Agrega un elemento al final de la lista de prdidas, su parmetro
// inicial es un puntero apuntando al nodo final de la lista
{
   	TListaPE2 *LAux2;

   	if((LAux2 = (TListaPE2*) malloc(sizeof(TListaPE2))) == NULL)
   		nomemory("Laux2 en app2_agregalistap_g");
#ifdef _DEBUG_MALLOC_		
   	printf("Malloc, App2_g.c, 35, LAux2, %d\n", sizeof(TListaPE2));
#endif

   	LAux2->ancho = anc;
   	LAux2->alto = alt;
   	LAux2->xini = x;
   	LAux2->yini = y;
   	LAux2->tipo = tipo; 		// 'E'= Prdida Externa,'P'=Prdida Interna, 'G'=Ganacia
   	LAux2->pieza = t_pieza; // t_pieza = 0 => Identifica prdida,
   	LAux2->prox = LPer2; 		// t_pieza >0  => Id tipo de pieza con orientacin normal de la pieza,
   	LPer2 = LAux2;					// t_pieza <0  => Id tipo de pieza orientacin invertida de la pieza.
	TotalPie++;		// Incrementa el nmero de piezas (ganancias + prididas) totales.
}//End app2_agregalistap_g

TNodoRE2 app2_pieza2perdida_g(int anc,int alt)
// Politica de Encaje
{
    TListaPE2 *LAux2, *LBusca2;
    TNodoRE2 Ret2;
    int d_ancho_min = AnchoPl, d_alto_min = AltoPl, d_ancho_actual, d_alto_actual;
    int pos_nodo_best_fit = -1, j = 0;

   	LAux2 = LPer2;
  	LBusca2 = LPer2;

    if(LBusca2 == NULL) {
    	Ret2.ancho = -1;
      	return Ret2;
   	}//End if
   	while(LAux2 != NULL) {  // Mientras LAux.prox no apunte a NULL
      	if((anc <= LAux2->ancho)&&(alt <= LAux2->alto)&&(LAux2->tipo == 'P')) {
         	d_ancho_actual = LAux2->ancho - anc;
        	d_alto_actual = LAux2->alto - alt;
    		if((d_ancho_actual == 0) && (d_alto_actual == 0)){
    			pos_nodo_best_fit = j;				
				break;
         	}//End if
    		else if(d_ancho_actual == 0){
        	   	if(d_alto_actual <= d_alto_min){
               		d_ancho_min = d_ancho_actual;
               		d_alto_min = d_alto_actual;
    				pos_nodo_best_fit = j;				
            	}//End if
	        	else if((d_ancho_min != 0) && (d_alto_min != 0)){
	        		//Me interesa no perder la pieza actual ya que por lo menos
	        		//esta calza justo por un lado, pero la d_min NO...
	            	d_ancho_min = d_ancho_actual;
	               	d_alto_min = d_alto_actual;
	    			pos_nodo_best_fit = j;				
	            }//End else if
	        }//End else if
    		else if(d_alto_actual == 0){
        		if(d_ancho_actual <= d_ancho_min){
           	   		d_ancho_min = d_ancho_actual;
               		d_alto_min = d_alto_actual;
    				pos_nodo_best_fit = j;				
            	}//End if
        		else if((d_ancho_min != 0) && (d_alto_min != 0)){
        			//Me interesa no perder la pieza actual ya que por lo menos
        			//esta calza justo por un lado, pero la d_min NO...
            		d_ancho_min = d_ancho_actual;
            		d_alto_min = d_alto_actual;
    				pos_nodo_best_fit = j;				
        		}//End else if
         	}//End else if
    		else if((d_ancho_actual < d_ancho_min) || (d_alto_actual < d_alto_min)){
         		d_ancho_min = d_ancho_actual;
         		d_alto_min = d_alto_actual;
				pos_nodo_best_fit = j;				
    		}//End else if
	   	}//End if
		j++;
		LAux2 = LAux2->prox;
   	}//End while
   	if(pos_nodo_best_fit == 0){
 	   	//Trata de calzar nueva pieza en el nodo cabeza de la lista de Prdidas
      	Ret2.ancho = LBusca2->ancho; // Nueva pieza calza en primer nodo
      	Ret2.alto = LBusca2->alto;   // Asigna nuevo nodo
      	Ret2.xini = LBusca2->xini;
      	Ret2.yini = LBusca2->yini;
      	Ret2.tipo = LBusca2->tipo;   // 'P'=Prdida, 'G'=Ganacia
      	Ret2.pieza = LBusca2->pieza; // -1=nmero que identifica prdida,>0=Id del tipo de pieza utilizado
      	LPer2 = LBusca2->prox;       // LPer queda apuntandoa al prximo
        free(LBusca2);
#ifdef _DEBUG_MALLOC_	
   	printf("Free, app2_g.c, 119, LBusca2\n");              // Libera LBusca
#endif
   	    TotalPie--;										// Decrementa el nmero de piezas (ganancias + prididas) totales.
      	return Ret2;
   	}//End if
   	else if(pos_nodo_best_fit > 0){
    	LAux2 = LPer2;
      	LBusca2 = LPer2;
		j = 1;
      	while(LAux2 != NULL) {  // Mientras LAux.prox no apunte a NULL
         	LAux2 = LBusca2->prox;
         	if(j == pos_nodo_best_fit) {
            	Ret2.ancho = LAux2->ancho;
            	Ret2.alto = LAux2->alto;
            	Ret2.xini = LAux2->xini;
            	Ret2.yini = LAux2->yini;
            	Ret2.tipo = LAux2->tipo; 	// 'P'=Prdida, 'G'=Ganacia
            	Ret2.pieza = LAux2->pieza;// -1=nmero que identifica prdida,>0=Id del tipo de pieza utilizado
            	LBusca2->prox = LAux2->prox;
	          	free(LAux2);
#ifdef _DEBUG_MALLOC_		
   	printf("Free, app2_g.c, 137, LAux2\n");
#endif
        		TotalPie--;							  // Decrementa el nmero de piezas (ganancias + prdidas) totales.
            	return Ret2;
         	}//End if
         	LBusca2 = LBusca2->prox;
    		j++;
      	}//End while
   	}//End elseif
   	Ret2.ancho = -1; // Sale por aqui cuando hay una lista de piezas
   	return Ret2;     // y la nueva pieza no calza en ninguna de las prdidas.
}//End app2_pieza2perdida_g

void app2_genera_layout_g(int tipo_problema, char *nombrearchivo, cpu_consummed_time_t *t, TListaPE2 *Bus2)
//Función que genera el layout de salida
{
	char strtot[32768], straux[100];
	
	//Primero coloca parámetros importantes 
	fprintf(outfp,"29 %d %d %d\n", TotalPie, AnchoPl, AltoPl); //29 línea => 28 parámetros + 1 (fin de parámetros).
  	strtot[0] = 0;
    if(tipo_problema == 0) 
    	sprintf(straux,"TIPO_PROBLEMA                  : %s\n", "P.C.P Guillotina");
    else if(tipo_problema == 1) 
    	sprintf(straux,"TIPO_PROBLEMA                  : %s\n", "P.C.P No Guillotina");
    else if(tipo_problema == 2) 
    	sprintf(straux,"TIPO_PROBLEMA                  : %s\n", "P.C.P Irregulares");
    else if(tipo_problema == 4) 
    	sprintf(straux,"TIPO_PROBLEMA                  : %s\n", "Stock Cutting No Guillotina");
    else 
    	sprintf(straux,"TIPO_PROBLEMA                  : %s\n", "NO DEFINIDO...");
    strcat(strtot, straux);
	sprintf(straux,"ARCHIVO_PROBLEMA               : %s\n", nombrearchivo);
	strcat(strtot, straux);
	sprintf(straux,"CANTIDAD_PIEZAS                : %d\n", NumPie);
	strcat(strtot, straux);
	sprintf(straux,"CANTIDAD_TIPOS_PIEZAS          : %d\n", cantidadtipospiezas);
	strcat(strtot, straux);
	sprintf(straux,"ANCHO_LAMINA (cm)              : %d\n", AnchoPl);
	strcat(strtot, straux);
	sprintf(straux,"LARGO_LAMINA (cm)              : %d\n", AltoPl);
	strcat(strtot, straux);
	sprintf(straux,"MAXIMO_GENERACIONES            : %d\n", maxgen);
	strcat(strtot, straux);
	sprintf(straux,"PESO_PERDIDA  (%%)              : %f\n", 100.0 * peso_perdida);
	strcat(strtot, straux);
	sprintf(straux,"PESO_DISTANCIA  (%%)            : %f\n", 100.0 * peso_distancia);
	strcat(strtot, straux);
	sprintf(straux,"PESO_DIGREGACION (%%)           : %f\n", 100.0 * peso_digregacion);
	strcat(strtot, straux);
	sprintf(straux,"TAMANO_POBLACION (ind.)        : %d\n", popsize);
	strcat(strtot, straux);
	sprintf(straux,"PROBABILIDAD_CRUZAMIENTO (%%)   : %f\n", 100.0 * pcross);
	strcat(strtot, straux);
	sprintf(straux,"PROBABILIDAD_MUTACION (%%)      : %f\n", 100.0 * pmutation);
	strcat(strtot, straux);
	sprintf(straux,"SEMILLA (%%)                    : %f\n", 100.0 * randomseed);
	strcat(strtot, straux);
	sprintf(straux,"MS_CALIDAD (%%)                 : %f\n", 100.0 * bestfit.PEval.calidad);
	strcat(strtot, straux);
	sprintf(straux,"MS_FITNESS                     : %f\n", bestfit.fitness);
	strcat(strtot, straux);
	sprintf(straux,"MS_PERDIDA (%%)                 : %f\n", 100.0 * bestfit.PEval.c_perdidareal/(AnchoPl * AltoPl));
	strcat(strtot, straux);
	sprintf(straux,"MS_DISTANCIA (%%)               : %f\n", 100.0 * bestfit.PEval.c_distancia);
	strcat(strtot, straux);
	sprintf(straux,"MS_DIGREGACION (%%)             : %f\n", 100.0 * bestfit.PEval.c_digregacion);
	strcat(strtot, straux);
	sprintf(straux,"MS_AREA_OCUPADA (%%)            : %f\n", 100.0 * bestfit.PEval.areaocupada);
	strcat(strtot, straux);
	sprintf(straux,"MS_NUMERO_PIEZAS_COLOCADAS     : %d\n", bestfit.PEval.piezas);
	strcat(strtot, straux);
	sprintf(straux,"MS_NUMERO_PERDIDAS_GENERADAS   : %d\n", bestfit.PEval.n_perdidas);
	strcat(strtot, straux);
	sprintf(straux,"GENERACION_OCURRENCIA          : %d\n", bestfit.generation);
	strcat(strtot, straux);
	sprintf(straux,"TIEMPO_CPU (Segundos)          : %f\n", t->cpu_time);
	strcat(strtot, straux);
	sprintf(straux,"TIEMPO_IO (Segundos)           : %f\n", t->io_time);
	strcat(strtot, straux);
	sprintf(straux,"TIEMPO_TOTAL (Segundos)        : %f\n", t->elapsed_time);
	strcat(strtot, straux);
	sprintf(straux,"TOTAL PIEZAS+PERDIDAS HECHAS   : %d\n", TotalPie);
	strcat(strtot, straux);
	fprintf(outfp,"%s@\n", strtot);

	//Finalmente detalla pieza a pieza
	fprintf(outfp,"  TIPO_PIEZA\t   POS_X_INI\t   POS_Y_INI\t   POS_X_FIN\t   POS_Y_FIN\tESTADO_PIEZA\tAREA_OCUPADA\t\n");
	while(Bus2 != NULL) {
   		fprintf(outfp,"%12d\t%12d\t%12d\t%12d\t%12d\t%12c\t%12d\t\n",
	   	Bus2->pieza, Bus2->xini, Bus2->yini, Bus2->xini+Bus2->ancho, Bus2->yini+Bus2->alto,
		Bus2->tipo, Bus2->ancho * Bus2->alto);
  		Bus2 = Bus2->prox;
 	}//End while	
  	fprintf(outfp,"\n");
  	
}//End app2_genera_layout_g

void app2_funceval_g(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion, TNodoAP *piezas)
//funcin evaluacin para desplegar al mejor individuo
{
  	int 		i, siguiente=1;
   	int 		Acan = 0, Acal = 0, Acpi = 0, Nuan, Nual, Nupi;
   	int 		PieInc = NumPie;
   	TNodoRE2 	Perd2;
   	TListaPE2 	*Bus2;

	// Inicializa la Lista de Prdidas o Ganacias
	LPer2    = NULL;
	TotalPie = 0;

	//Me estoy saltando las piezas de ancho y largo = 0
   	for(i=0;i<NumPie;i++){
    	Acan = piezas[i].ancho;
      	Acal = piezas[i].alto;
      	Acpi = piezas[i].numero;
      	if((Acan != 0) && (Acal != 0) && (Acan <= AnchoPl) && (Acal <= AltoPl)) {
		   	app2_agregalistap_g(0, 0, Acan, Acal, 'G', Acpi);
			siguiente = i+1;
			i = NumPie;
	   	}//End if
      	else
			PieInc--;
	}//End for
	if(siguiente<NumPie){
    	for(i=siguiente;i<NumPie;i++) {
        	Nuan = piezas[i].ancho;
         	Nual = piezas[i].alto;
         	Nupi = piezas[i].numero;
         	if((Nuan != 0) && (Nual != 0) && (Nuan <= AnchoPl) && (Nual <= AltoPl)){
				Perd2 = app2_pieza2perdida_g(Nuan,Nual);
				// Se puedo calzar pieza nueva en alguna prdida dentro de la lista de prdidas
            	if(Perd2.ancho != -1) {
					// La nueva pieza est dentro de una prdida => se generan 2 nuevas prdidas
					// y se agrega la pieza que cabe
               		if((Nuan < Perd2.ancho) && (Nual < Perd2.alto)) {
						// Agrega la nueva pieza dentro de la pieza que acaba de sacar
						app2_agregalistap_g(Perd2.xini, Perd2.yini, Nuan, Nual, 'G', Nupi);
						// Define fragmento de prdida inferior (dentro del patrn y debajo de la nueva pieza)
                  		app2_agregalistap_g(Perd2.xini, Perd2.yini+Nual, Perd2.ancho, Perd2.alto-Nual, 'P', 0);
						// Define fragmento de prdida derecho (dentro del patrn y al lado de la nueva pieza)
                  		app2_agregalistap_g(Perd2.xini+Nuan, Perd2.yini, Perd2.ancho - Nuan, Nual, 'P', 0);
               		}//End if
					// La nueva pieza calza exacto a lo ancho de la pridida
					// y queda una prdida inferior (por debajo de la nueva pieza),
					// se agrega la nueva pieza y la prdida inferior
               		else if(Nual < Perd2.alto) {
						// Agrega la nueva pieza que cabe a lo ancho exacto
						app2_agregalistap_g(Perd2.xini, Perd2.yini, Nuan, Nual, 'G', Nupi);
						// Agrega nueva prdida del mismo ancho de la perdida anterior pero de menor alto.
                  		app2_agregalistap_g(Perd2.xini, Perd2.yini+Nual, Nuan, Perd2.alto - Nual, 'P', 0);
               		}//End else if
					// La nueva pieza calza exacto a lo largo de la pridida y queda
					// una prdida derecha (por al lado de la nueva pieza)
               		else if(Nuan < Perd2.ancho) {
					 	// Agrega la nueva pieza que calza a lo alto exacto
						app2_agregalistap_g(Perd2.xini, Perd2.yini, Nuan, Nual, 'G', Nupi);
						// Agrega nueva prdida del mismo alto de la perdida anterior pero de menor ancho.
                  		app2_agregalistap_g(Perd2.xini+Nuan, Perd2.yini, Perd2.ancho - Nuan, Nual, 'P', 0);
               		}//End else if
					// La nueva pieza calza justo con la prdida => No hay perdida y se agrega la pieza 'G'
               		else if((Nuan = Perd2.ancho) && (Nual = Perd2.alto))
						//Agrega la nueva pieza que calza a lo alto exacto
						app2_agregalistap_g(Perd2.xini, Perd2.yini, Nuan, Nual, 'G', Nupi);
            	}//End if
				// Nueva pieza no calz en las prdidas o no hay prdidas.
            	else {
				   	// Determina corte vertical, pieza la pone al lado derecho del patrn de corte
               		if((Acal + Nual) <= AltoPl) {								
						// Genera Prdida al lado derecho de la nueva pieza
						// y por debajo del patrn de corte actual
                  		if(Acan > Nuan) {
							// Agrega la nueva pieza debajo del patrn y a la izquierda
							app2_agregalistap_g(0, Acal, Nuan, Nual, 'G', Nupi);
							// Ingresa nueva Prdida de dimensiones (crecimiento a lo alto
							// (hacia abajo) del patrn) x (Alto de la nueva pieza)
                     		app2_agregalistap_g(Nuan, Acal, Acan - Nuan, Nual, 'P', 0);
							// Dimensiona nuevo alto del patrn de corte actual
                     		Acal = Acal + Nual;
                  		}//End if
						// Genera Prdida al lado derecho el patrn de corte y por encima de la nueva pieza.
                  		else if(Nuan > Acan) {
							// Agrega la nueva pieza debajo del patrn y a la izquierda
						   	app2_agregalistap_g(0, Acal, Nuan, Nual, 'G', Nupi);
							// Ingresa nueva Prdida de dimensiones (crecimiento a lo ancho
							// (hacia lado derecho) del patrn) x (Alto del patrn de corte antiguo)
                     		app2_agregalistap_g(Acan, 0, Nuan - Acan, Acal, 'P', 0);
							// Dimensiona nuevo ancho del patrn de corte actual
                     		Acan = Nuan;
							// Dimensiona nuevo alto del patrn de corte actual
                     		Acal = Acal + Nual;
                  		}//End else if
						// No hay prdida,el ancho de la nueva pieza coincide exacto con el del patrn
						// de corte pero hay que actualizar el alto del nuevo patrn de corte
                  		else {
						   	// Agrega la nueva pieza debajo del patrn y a la izquierda
							app2_agregalistap_g(0, Acal, Nuan, Nual, 'G', Nupi);
							// Dimensiona nuevo alto del patrn de corte actual
                     		Acal = Acal + Nual;
						}//End else
               		}//End if
					// Determina corte horizontal, pieza la pone por debajo del patrn de corte
               		else if((Acan + Nuan) <= AnchoPl) {
					   	// Genera Prdida bajo la nueva pieza y a la derecha del patrn de corte actual
                  		if(Acal > Nual) {
							// Agrega la nueva pieza al lado derecho del patrn
							app2_agregalistap_g(Acan, 0, Nuan, Nual, 'G', Nupi);
							// Ingresa nueva Prdida de dimensiones (Ancho nueva pieza) x (crecimiento
							// a lo ancho (derecho) del patrn)
                     		app2_agregalistap_g(Acan, Nual, Nuan, Acal - Nual, 'P', 0);
							// Dimensiona nuevo ancho del patrn de corte actual
                     		Acan = Acan + Nuan;
                  		}//End if
						// Genera Prdida bajo el patrn de corte y a la izquierda de la nueva pieza.
                  		else if(Nual > Acal) {
							// Agrega la nueva pieza al lado derecho del patrn
							app2_agregalistap_g(Acan, 0, Nuan, Nual, 'G', Nupi);
							// Ingresa nueva Prdida de dimensiones (Ancho del patrn antes de insertar
						   	// nueva pieza) x (crecimiento a lo alto (hacia abajo) del patrn)
                     		app2_agregalistap_g(0, Acal, Acan, Nual - Acal, 'P', 0);
							// Dimensiona nuevo ancho del patrn de corte actual
                     		Acan = Acan + Nuan;
							// Dimensiona nuevo alto del patrn de corte actual
                     		Acal = Nual;
                  		}//End else if
						// No hay prdida,el alto de la nueva pieza coincide con el del patrn de corte
						// pero hay que actualizar el ancho del nuevo patrn de corte
                  		else {
							// Agrega la nueva pieza al lado izquierdo del patrn
							app2_agregalistap_g(Acan, 0, Nuan, Nual, 'G', Nupi);
							// Dimensiona nuevo ancho del patrn de corte actual
                     		Acan = Acan + Nuan;
						}//End else
               		}//End else if
               		else PieInc--; // Pieza excede el area disponible en la lmina.
            	}//End else
 			}//End if
			else PieInc--;         // Pieza excede el area disponible en la lmina.
      	}//End for
	}//End if
   	//Existe patrn de corte
	if(Acan != 0 && Acal != 0) { 	
	  	// El patrón está completamente dentro de la lámina => se generan 2 pérdidas externas
      	if(Acan < AnchoPl && Acal < AltoPl)	{ 	
			// Ingresa nueva Prdida de dimensiones (Ancho:diferencia entre
			// ancho lmina y ancho patrn) x (alto del patrn)
         	app2_agregalistap_g(Acan, 0, AnchoPl - Acan, Acal, 'E', 0);
			// Ingresa nueva Prdida de dimensiones (Ancho de la
			// lmina) x (alto:diferencia entre alto lmina y alto patrn)
         	app2_agregalistap_g(0, Acal, AnchoPl, AltoPl - Acal, 'E', 0);
      	}//End if
		// El patrón cubre totalmente el alto de la lámina pero es más angosto que la lámina
		// => se genera 1 pérdida externa al lado del patrón
      	else if(Acan < AnchoPl) 								
			// Ingresa nueva Pérdida de dimensiones (Ancho:diferencia entre ancho lámina
			// y ancho patrón) x (alto de la lámina)
         	app2_agregalistap_g(Acan, 0, AnchoPl - Acan, AltoPl, 'E', 0);
		   	// El patrón cubre totalmente el ancho de la lámina pero es menos alto que la lámina
			// => se genera 1 pérdida externa debajo del patrón
      	else if(Acal < AltoPl)									
			// Ingresa nueva Prdida de dimensiones (Ancho de la
			// lmina) x (alto:diferencia entre alto lmina y alto patrn)
         	app2_agregalistap_g(0, Acal, AnchoPl, AltoPl - Acal, 'E', 0);
	}//End if
	// No existe patrn de corte	=> la prdida es de toda la lmina	
	else	
		// Ingresa Prdida de dimensiones (Ancho de la lmina) x (alto de la lmina)
    	app2_agregalistap_g(0, 0, AnchoPl, AltoPl, 'E', 0);
   		
	Bus2 = LPer2;

	// LLENA archivo de layout para el problema particular
	app2_genera_layout_g(tipo_problema, nomarch, time_consumtion, Bus2);
	
	app2_liberamemlistaperdidas_g();
}//End app2_funceval_g

void app2_objfunc_g(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion, struct bestever *bestcritter)
// Funcin Objetivo, transforma el string cromosoma en un arreglo de piezas
{
   	unsigned mask=1;
   	unsigned tp, tp1;
   	int      j, k, stop;
	unsigned bitpos;
	TNodoAP  *piezaschromo; // variable que apunta a todas las piezas del cromosoma generado

   	if((piezaschromo = (TNodoAP*) malloc(NumPie*sizeof(TNodoAP))) == NULL)
		nomemory("piezaschromo en bestobjfunc");
#ifdef _DEBUG_MALLOC_		
   	printf("Malloc, App2_g.c, 365, piezaschromo, %d\n", NumPie*sizeof(TNodoAP));
#endif

   	//Asigna cada pieza del arreglo piezasproblema en arregla piezaschromo dependiendo del string chrom
   	for(k = 0; k < chromsize; k++) {
    	if(k == (chromsize-1))
        	stop = lchrom-(k*UINTSIZE);
      	else
         	stop = UINTSIZE;
      	tp  = bestcritter->chrom[k];
		tp1 = bestcritter->chmut[k];
      	for(j = 0; j < stop; j++) {
         	bitpos = j + UINTSIZE*k;
         	// Entra al if si el bit actual es 1
         	if(tp&mask) {
				//Asigna pieza desde arreglo piezasproblema
		     	if(tp1&mask) {
    				// Efecta rotacin de piezas
         			piezaschromo[bitpos].ancho 			= piezasproblema[bitpos].alto;
               		piezaschromo[bitpos].alto  			= piezasproblema[bitpos].ancho;
		    		piezaschromo[bitpos].numero			= -1 * piezasproblema[bitpos].numero;
				}//End if
				else {								
					// No efectua rotacion de piezas
    				piezaschromo[bitpos].ancho 			= piezasproblema[bitpos].ancho;
               		piezaschromo[bitpos].alto  			= piezasproblema[bitpos].alto;
		      		piezaschromo[bitpos].numero         = piezasproblema[bitpos].numero;
				}//End else
            	piezaschromo[bitpos].cantidadpiezas     = 1;
		   	}//End if
         	else {
				//Asigna solo ceros => pieza no se considera
				piezaschromo[bitpos].ancho 				= 0;
            	piezaschromo[bitpos].alto  				= 0;
         		piezaschromo[bitpos].numero				= 0;
        	   	piezaschromo[bitpos].cantidadpiezas     = 1;
		   	}//End else
         	tp = tp>>1;
         	tp1 = tp1>>1;
      	}//End for
   }//End for
	
   app2_funceval_g(tipo_problema, nomarch, time_consumtion, piezaschromo);
	
   free(piezaschromo);
#ifdef _DEBUG_MALLOC_		
   printf("Free, app2_g.c, 417, piezaschromo\n");
#endif
}//End app2_objfunc_g

