/*----------------------------------------------------------------------------*/
/* app2_g.c - rutinas para Problema Corte de Piezas guillotina = PCPG         */
/*----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "sga.h"
#include "var_g.h" 

void app2_liberamemlistaperdidas_g(void)
// Libera memoria de la lista de pérdidas y/o ganancias
{
   	TListaPE2 *LAux2;
   	LAux2 = LPer2;

   	if(LAux2 == NULL)	return;
   	while(LAux2 != NULL) {
      	LPer2 = LAux2->prox;
      	free(LAux2);
#ifdef _DEBUG_MALLOC_		
   	printf("Free, app2_g.c, 24, LAux2\n");
#endif
      	LAux2 = LPer2;
   	}//End while
}//End app2_liberamemlistaperdidas

void app2_agregalistap_g(int x, int y, int anc, int alt, char tipo, int t_pieza)
// Agrega un elemento al final de la lista de pérdidas, su parámetro
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
   	LAux2->tipo = tipo; 	// 'E'= Pérdida Externa,'P'=Pérdida Interna, 'G'=Ganacia
   	LAux2->pieza = t_pieza; // t_pieza = 0 => Identifica pérdida,
   	LAux2->prox = LPer2;    // t_pieza > 0 => Id tipo de pieza con orientación normal de la pieza,
   	LPer2 = LAux2;	   		// t_pieza < 0 => Id tipo de pieza orientación invertida de la pieza.
   	TotalPie++;             // Incrementa el número de piezas (ganancias + pérdidas) totales.
}//End app2_agregalistap

TNodoRE2 app2_pieza2perdida_g(int anc,int alt)
// Politica de Encaje
{
   	TListaPE2 	*LAux2, *LBusca2;
   	TNodoRE2 	Ret2;
   	int 		d_ancho_min = AnchoPl, d_alto_min = AltoPl, d_ancho_actual, d_alto_actual;
   	int 		pos_nodo_best_fit = -1, j = 0;
   	
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
      	//Trata de calzar nueva pieza en el nodo cabeza de la lista de Pérdidas
      	Ret2.ancho = LBusca2->ancho; // Nueva pieza calza en primer nodo
      	Ret2.alto = LBusca2->alto;   // Asigna nuevo nodo
      	Ret2.xini = LBusca2->xini;
      	Ret2.yini = LBusca2->yini;
      	Ret2.tipo = LBusca2->tipo;   // 'P'=Pérdida, 'G'=Ganacia
      	Ret2.pieza = LBusca2->pieza; // -1=número que identifica pérdida,>0=Id del tipo de pieza utilizado
      	LPer2 = LBusca2->prox;       // LPer queda apuntando al próximo
      	free(LBusca2);
#ifdef _DEBUG_MALLOC_	
   	printf("Free, app2_g.c, 119, LBusca2\n"); // Libera LBusca
#endif
   		TotalPie--; // Decrementa el número de piezas (ganancias + pérididas) totales.
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
            	Ret2.tipo = LAux2->tipo;   // 'P'=Pérdida, 'G'=Ganacia
            	Ret2.pieza = LAux2->pieza; // -1=número que identifica pérdida,>0=Id del tipo de pieza utilizado
            	LBusca2->prox = LAux2->prox;
            	free(LAux2);
#ifdef _DEBUG_MALLOC_		
   	printf("Free, app2_g.c, 137, LAux2\n");
#endif
            	TotalPie--; // Decrementa el número de piezas (ganancias + pérdidas) totales.
            	return Ret2;
         	}//End if
         	LBusca2 = LBusca2->prox;
         	j++;
      	}//End while
   	}//End elseif
   	Ret2.ancho = -1; // Sale por aqui cuando hay una lista de piezas
   	return Ret2;     // y la nueva pieza no calza en ninguna de las pérdidas.
}//End app2_pieza2perdida

void app2_genera_layout_g(TListaPE2 *Bus2)
//Función que genera el layout de salida
{
	char strtot[32768], straux[100];
	
	//Primero coloca parámetros importantes 
	fprintf(outfp,"32 %d %d %d\n", TotalPie, AnchoPl, AltoPl); //32 parmetros => 31 + 1 que indica Total de piezas + perdidas generadas... 
  	strtot[0] = 0;
  	strcat(strtot, straux);
	sprintf(straux,"CANTIDAD_PIEZAS                : %d\n", NumPie);
	strcat(strtot, straux);
                      sprintf(straux,"INSTANCIA                      : %s\n", nomarch);
	strcat(strtot, straux);
	sprintf(straux,"CANTIDAD_TIPOS_PIEZAS          : %d\n", cantidadtipospiezas);
	strcat(strtot, straux);
	sprintf(straux,"ANCHO_LAMINA (unidades)        : %d\n", AnchoPl);
	strcat(strtot, straux);
	sprintf(straux,"LARGO_LAMINA (unidades)        : %d\n", AltoPl);
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
	sprintf(straux,"MS_CALIDAD (%%)                 : %f\n", 100.0 * bestfit.PEval.calidad);
	strcat(strtot, straux);
	sprintf(straux,"MS_FITNESS                     : %f\n", bestfit.fitness);
	strcat(strtot, straux);
	sprintf(straux,"MS_PERDIDA (%%)                 : %f\n", 100.0 * bestfit.PEval.c_perdidareal);
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
	sprintf(straux,"WORKER_MEJOR_INDIVIDUO         : %d\n", bestfit.generation);
  	strcat(strtot, straux);
 	sprintf(straux,"TOTAL PIEZAS+PERDIDAS HECHAS   : %d\n", TotalPie);
	strcat(strtot, straux);
	fprintf(outfp,"%s\n", strtot);

	//Finalmente detalla pieza a pieza
/*
	fprintf(outfp,"  TIPO_PIEZA\t   POS_X_INI\t   POS_Y_INI\t   POS_X_FIN\t   POS_Y_FIN\tESTADO_PIEZA\tAREA_OCUPADA\t\n");
*/
                        fprintf(outfp,"PIEZAS: ");
	while(Bus2 != NULL) {
/*
   		fprintf(outfp,"%12d\t%12d\t%12d\t%12d\t%12d\t%12c\t%12d\t\n",
	   	Bus2->pieza, Bus2->xini, Bus2->yini, Bus2->xini+Bus2->ancho, Bus2->yini+Bus2->alto,
		Bus2->tipo, Bus2->ancho * Bus2->alto);
  		Bus2 = Bus2->prox;
*/
                
/*
                                if(Bus2->tipo == 'G') {
*/
                                        fprintf(outfp, "%c,%d,%d,%d,%d;", Bus2->tipo, Bus2->xini, Bus2->yini, Bus2->ancho, Bus2->alto);
/*
                                }
*/
                                Bus2 = Bus2->prox;
 	}//End while	
  	fprintf(outfp,"\n---------------------------------------------------------------------------------------------------\n");
}//End app2_genera_layout_g

void app2_funceval_g(TNodoAP *piezas)
//función evaluación para desplegar al mejor individuo
{
   	int         i, siguiente=1, acum, cont;
   	int         Acan = 0, Acal = 0, Acpi = 0, Nuan, Nual, Nupi;
   	int         PieInc = NumPie;
        int PerdExt;
   	int AreaPlaca = AnchoPl * AltoPl;
   	float AreaOcup,UnifPerd, TAU, CONSTANTE;
        
        TEval Eval;
   	TNodoRE2    Perd2;
   	TListaPE2   *Bus2;
   
   	// Inicializa la Lista de Pérdidas o Ganacias
   	LPer2 = NULL;
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
            	// Se puede calzar pieza nueva en alguna pérdida dentro de la lista de pérdidas
                
            	if(Perd2.ancho != -1) {
               		// La nueva pieza está dentro de una pérdida => se generan 2 nuevas pérdidas
               		// y se agrega la pieza que cabe
               		if((Nuan < Perd2.ancho) && (Nual < Perd2.alto)) {
                  		// Agrega la nueva pieza dentro de la pieza que acaba de sacar
                  		app2_agregalistap_g(Perd2.xini, Perd2.yini, Nuan, Nual, 'G', Nupi);
                  		// Define fragmento de pérdida inferior (dentro del patrón y debajo de la nueva pieza)
                  		app2_agregalistap_g(Perd2.xini, Perd2.yini+Nual, Perd2.ancho, Perd2.alto-Nual, 'P', 0);
                  		// Define fragmento de pérdida derecho (dentro del patrón y al lado de la nueva pieza)
                  		app2_agregalistap_g(Perd2.xini+Nuan, Perd2.yini, Perd2.ancho - Nuan, Nual, 'P', 0);
               		}//End if
               		// La nueva pieza calza exacto a lo ancho de la pérdida
               		// y queda una pérdida inferior (por debajo de la nueva pieza),
               		// se agrega la nueva pieza y la pérdida inferior
               		else if(Nual < Perd2.alto) {
                  		// Agrega la nueva pieza que cabe a lo ancho exacto
                  		app2_agregalistap_g(Perd2.xini, Perd2.yini, Nuan, Nual, 'G', Nupi);
                  		// Agrega nueva pérdida del mismo ancho de la perdida anterior pero de menor alto.
                  		app2_agregalistap_g(Perd2.xini, Perd2.yini+Nual, Nuan, Perd2.alto - Nual, 'P', 0);
               		}//End else if
               		// La nueva pieza calza exacto a lo largo de la pérdida y queda
               		// una pérdida derecha (por al lado de la nueva pieza)
               		else if(Nuan < Perd2.ancho) {
                  		// Agrega la nueva pieza que calza a lo alto exacto
                  		app2_agregalistap_g(Perd2.xini, Perd2.yini, Nuan, Nual, 'G', Nupi);
                  		// Agrega nueva pérdida del mismo alto de la perdida anterior pero de menor ancho.
                  		app2_agregalistap_g(Perd2.xini+Nuan, Perd2.yini, Perd2.ancho - Nuan, Nual, 'P', 0);
               		}//End else if
               		// La nueva pieza calza justo con la pérdida => No hay perdida y se agrega la pieza 'G'
               		else if((Nuan = Perd2.ancho) && (Nual = Perd2.alto))
                  		//Agrega la nueva pieza que calza a lo alto exacto
                  		app2_agregalistap_g(Perd2.xini, Perd2.yini, Nuan, Nual, 'G', Nupi);
            	}//End if
            	// Nueva pieza no calzó en las pérdidas o no hay pérdidas.
            	else {
               		// Determina corte vertical, pieza la pone al lado derecho del patrón de corte
               		if((Acal + Nual) <= AltoPl) {
                  		// Genera Pérdida al lado derecho de la nueva pieza
                  		// y por debajo del patrón de corte actual
                  		if(Acan > Nuan) {
                     		// Agrega la nueva pieza debajo del patrón y a la izquierda
                     		app2_agregalistap_g(0, Acal, Nuan, Nual, 'G', Nupi);
                     		// Ingresa nueva Pérdida de dimensiones (crecimiento a lo alto
                     		// (hacia abajo) del patrón) x (Alto de la nueva pieza)
                     		app2_agregalistap_g(Nuan, Acal, Acan - Nuan, Nual, 'P', 0);
                     		// Dimensiona nuevo alto del patrón de corte actual
                     		Acal = Acal + Nual;
                  		}//End if
                  		// Genera Pérdida al lado derecho el patrón de corte y por encima de la nueva pieza.
                  		else if(Nuan > Acan) {
                     		// Agrega la nueva pieza debajo del patrón y a la izquierda
		                    app2_agregalistap_g(0, Acal, Nuan, Nual, 'G', Nupi);
		                    // Ingresa nueva Pérdida de dimensiones (crecimiento a lo ancho
		                    // (hacia lado derecho) del patrón) x (Alto del patrón de corte antiguo)
		                    app2_agregalistap_g(Acan, 0, Nuan - Acan, Acal, 'P', 0);
		                    // Dimensiona nuevo ancho del patrón de corte actual
		                    Acan = Nuan;
		                    // Dimensiona nuevo alto del patrón de corte actual
		                    Acal = Acal + Nual;
		              	}//End else if
                  		// No hay pérdida,el ancho de la nueva pieza coincide exacto con el del patrón
                  		// de corte pero hay que actualizar el alto del nuevo patrón de corte
                  		else	{
                     		// Agrega la nueva pieza debajo del patrón y a la izquierda
		                    app2_agregalistap_g(0, Acal, Nuan, Nual, 'G', Nupi);
		                    // Dimensiona nuevo alto del patrón de corte actual
		                    Acal = Acal + Nual;
		               	}//End else
		         	}//End if
               		// Determina corte horizontal, pieza la pone por debajo del patrón de corte
               		else if((Acan + Nuan) <= AnchoPl) {
                  		// Genera Pérdida bajo la nueva pieza y a la derecha del patrón de corte actual
                  		if(Acal > Nual) {
		                    // Agrega la nueva pieza al lado derecho del patrón
		                    app2_agregalistap_g(Acan, 0, Nuan, Nual, 'G', Nupi);
		                    // Ingresa nueva Pérdida de dimensiones (Ancho nueva pieza) x (crecimiento
		                    // a lo ancho (derecho) del patrón)
		                    app2_agregalistap_g(Acan, Nual, Nuan, Acal - Nual, 'P', 0);
		                    // Dimensiona nuevo ancho del patrón de corte actual
		                    Acan = Acan + Nuan;
	             		}//End if
                  		// Genera Pérdida bajo el patrón de corte y a la izquierda de la nueva pieza.
		                else if(Nual > Acal) {
		                  	// Agrega la nueva pieza al lado derecho del patrón
		                    app2_agregalistap_g(Acan, 0, Nuan, Nual, 'G', Nupi);
		                    // Ingresa nueva Pérdida de dimensiones (Ancho del patrón antes de insertar
		                    // nueva pieza) x (crecimiento a lo alto (hacia abajo) del patrón)
		                    app2_agregalistap_g(0, Acal, Acan, Nual - Acal, 'P', 0);
		                    // Dimensiona nuevo ancho del patrón de corte actual
		                    Acan = Acan + Nuan;
		                    // Dimensiona nuevo alto del patrón de corte actual
		                    Acal = Nual;
		             	}//End else if
                  		// No hay pérdida,el alto de la nueva pieza coincide con el del patrón de corte
                  		// pero hay que actualizar el ancho del nuevo patrón de corte
                  		else {
                     		// Agrega la nueva pieza al lado izquierdo del patrón
                     		app2_agregalistap_g(Acan, 0, Nuan, Nual, 'G', Nupi);
                     		// Dimensiona nuevo ancho del patrón de corte actual
                     		Acan = Acan + Nuan;
                  		}//End else
               		}//End else if
               		else PieInc--; // Pieza excede el area disponible en la lámina.
            	}//End else
         	}//End if
         	else PieInc--;         // Pieza excede el area disponible en la lámina.
      	}//End for
   	}//End if
   	//Existe patrón de corte
   	if(Acan != 0 && Acal != 0) { 	
      	// El patrón está completamente dentro de la lámina => se generan 2 pérdidas externas
      	if(Acan < AnchoPl && Acal < AltoPl){
         	// Ingresa nueva Pérdida de dimensiones (Ancho:diferencia entre
         	// ancho lámina y ancho patrón) x (alto del patrón)
         	app2_agregalistap_g(Acan, 0, AnchoPl - Acan, Acal, 'E', 0);
         	// Ingresa nueva Pérdida de dimensiones (Ancho de la
         	// lámina) x (alto:diferencia entre alto lámina y alto patrón)
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
         	// Ingresa nueva Pérdida de dimensiones (Ancho de la
         	// lámina) x (alto:diferencia entre alto lámina y alto patrón)
         	app2_agregalistap_g(0, Acal, AnchoPl, AltoPl - Acal, 'E', 0);
   	}//End if
   	// No existe patrón de corte	=> la pérdida es de toda la lámina	
   	else
      	// Ingresa Pérdida de dimensiones (Ancho de la lámina) x (alto de la lámina)
      	app2_agregalistap_g(0, 0, AnchoPl, AltoPl, 'E', 0);
   
   	Bus2 = LPer2;

	// LLENA archivo de layout para el problema particular
        app2_genera_layout_g(Bus2);
        
}//End app2_funceval

void app2_objfuncfinal_g(struct bestever *critter) {
    unsigned mask = 1, tp, rt, bitpos, dir, salto, go = 1, m, b;
    int j, k, stop, i, ini, vueltas = 0, pIni = 0, valPos, nAsig = 0;
    int chrom[NumPie];
    TNodoAP *piezaschromo; // variable que apunta a todas las piezas del cromosoma generado
    
    if ((piezaschromo = (TNodoAP*) malloc(NumPie * sizeof (TNodoAP))) == NULL)
        nomemory("piezaschromo en objfunc");
#ifdef _DEBUG_MALLOC_		
    printf("Malloc, App_g.c, 440, piezaschromo, %d\n", NumPie * sizeof (TNodoAP));
#endif
    for(i=0; i<NumPie; i++) {
        arreglo_orden_g[i] = -1;
        arreglo_ocupado_g[i] = 0;
        chrom[i] = 0;
        arreglo_rotar_g[i] = 0;
    }
    for (i = 0; i < chromsize; i++) {
        if (i == (chromsize - 1)) //ultimo bloque
            stop = lchrom - (i * UINTSIZE);
        else
            stop = UINTSIZE;
        
        tp = critter->chrom[i];
        rt = critter->chmut[i];
        for (j = 0; j < stop; j++) {
            bitpos = j + UINTSIZE*i;
            if((i==0 && j >=bit_reservados_g) || i>0) {
                /*
                * Extrae segmento del cromosoma correspondiente a las piezas
                */
                if (tp & mask) chrom[bitpos - bit_reservados_g] = 1;
                else chrom[bitpos - bit_reservados_g] = 0;
/*
                printf("%i ", chrom[bitpos - bit_reservados_g]);
*/
                /*
                * Extrae segmento del cromosoma correspondiente a la orientacion de la pieza
                */
                if (rt & mask) arreglo_rotar_g[bitpos - bit_reservados_g] = 1; //Rotada
                else arreglo_rotar_g[bitpos - bit_reservados_g] = 0; //No rotada
            }
            tp = tp >> 1;
            rt = rt >> 1;
        }
    }
/*
    printf("\n");
*/
    /*
     * ini: Valor que se considera primero, 0 o 1, al recorrer el cromosoma
     */
    ini = 1;
    /*
     * dir: Direccion inicial en que se recorre el cromosoma
     * 0: der -> izq
     * 1: izq -> der
     */
    tp = critter->chrom[0];
    dir = (tp & mask)?1:0; 
    //Obtiene el valor del salto para recorrer el cromosoma
    salto = 1;
    for(i=0; i<3; i++) {
        tp = tp >> 1;
        for(b = 1, m = 2; m > i; m--) { b *= 2;}
        if(tp & mask) salto = salto + b;
    }
    
    
    vueltas = 0;
    while(go) { //mientras queden piezas por ordenar
        if(dir == 1) {//izq->der
            pIni = 0; //posicion inicial
            while(arreglo_ocupado_g[pIni] != 0) { //si ya se ordeno la pieza pasa a la de la derecha
                pIni++;
            }
            for(i=pIni; i<NumPie; i = i+salto) { //desde la posicion inicial hasta el final, con (i + salto) como valor de incremento
                if(arreglo_ocupado_g[i] == 0) { // si no ha sido agregado
                    valPos = chrom[i]; //obtiene el valor en esa posicion
                    arreglo_ocupado_g[i] = 1; //se marca como leida
                    if(valPos==ini) { //si es igual al valor buscado se agrega al listado de piezas ordenado
                        arreglo_orden_g[nAsig] = i; //se agrega al listado de piezas ordenado
                        if(arreglo_rotar_g[i]==1) {
                            piezaschromo[nAsig].ancho = piezasproblema[i].alto;
                            piezaschromo[nAsig].alto = piezasproblema[i].ancho;
                        } else {
                            piezaschromo[nAsig].alto = piezasproblema[i].alto;
                            piezaschromo[nAsig].ancho = piezasproblema[i].ancho;
                        }
                    } else {
                        piezaschromo[nAsig].ancho = 0;
                        piezaschromo[nAsig].alto = 0;
                        piezaschromo[nAsig].numero = 0;
                        piezaschromo[nAsig].cantidadpiezas = 1;
                    }
                    nAsig++;
                    if(nAsig == NumPie) go = 0; //si se ordenaron todas las piezas termina
                }
            }
        } else {//der->izq
            pIni = NumPie-1; //se ubica en la ultima posicion del string
            while(arreglo_ocupado_g[pIni] != 0) { //si ya se ordeno la pieza pasa a la de la izquierda
                pIni--;
            }
            for(i=pIni; i>=0; i = i-salto) { //desde la posicion inicial hasta el inicio del string, con (i + salto) como valor de retroceso
                if(arreglo_ocupado_g[i] == 0) { // si no ha sido agregado
                    valPos = chrom[i]; //obtiene el valor en esa posicion
                    arreglo_ocupado_g[i] = 1; //se marca como leida
                    if(valPos==ini) { //si es igual al valor buscado se agrega al listado de piezas ordenado
                        arreglo_orden_g[nAsig] = i; //se agrega al listado de piezas ordenado
                        if(arreglo_rotar_g[i]==1) {
                            piezaschromo[nAsig].ancho = piezasproblema[i].alto;
                            piezaschromo[nAsig].alto = piezasproblema[i].ancho;
                        } else {
                            piezaschromo[nAsig].alto = piezasproblema[i].alto;
                            piezaschromo[nAsig].ancho = piezasproblema[i].ancho;
                        }
                        piezaschromo[nAsig].numero = piezasproblema[i].numero;
                        piezaschromo[nAsig].cantidadpiezas = 1;
                    } else {
                        piezaschromo[nAsig].ancho = 0;
                        piezaschromo[nAsig].alto = 0;
                        piezaschromo[nAsig].numero = 0;
                        piezaschromo[nAsig].cantidadpiezas = 1;
                    }
                    nAsig++;
                    if(nAsig == NumPie) go = 0;
                }
            }
        }
        vueltas++;
        
        dir = (dir==0)?1:0; //se cambia la direccion en que se recorre el string
/*
        if(vueltas%2 == 0) ini = (ini==0)?1:0; //si ya se dio una vuelta completa, entonces se invierte el valor a buscar
*/
    }
    
/*
    for (k = 0; k < chromsize; k++) {
        if (k == (chromsize - 1)) //ultima
            stop = lchrom - (k * UINTSIZE);
        else
            stop = UINTSIZE;
        tp = critter->chrom[k];
        rt = critter->chmut[k];
        for (j = 0; j < stop; j++) {
            bitpos = j + UINTSIZE*k;
            // Entra al if si el bit actual es 1
            if (tp & mask) {
                //Asigna pieza desde arreglo piezasproblema
                if (rt & mask) {
                    // Efecta rotacin de piezas
                    piezaschromo[bitpos].ancho = piezasproblema[bitpos].alto;
                    piezaschromo[bitpos].alto = piezasproblema[bitpos].ancho;
                } else {
                    // No efectua rotacion de piezas
                    piezaschromo[bitpos].ancho = piezasproblema[bitpos].ancho;
                    piezaschromo[bitpos].alto = piezasproblema[bitpos].alto;
                }//End else
                piezaschromo[bitpos].numero = piezasproblema[bitpos].numero;
                piezaschromo[bitpos].cantidadpiezas = 1;
            } else {
                //Asigna solo ceros => pieza no se considera
                piezaschromo[bitpos].ancho = 0;
                piezaschromo[bitpos].alto = 0;
                piezaschromo[bitpos].numero = 0;
                piezaschromo[bitpos].cantidadpiezas = 1;
            }//End else
            tp = tp >> 1;
            rt = rt >> 1;
        }//End for
    }//End for
*/

/*
    critter->PEval = app2_funceval_g(piezaschromo, 1);
    critter->fitness = (float) critter->PEval.perdida;
*/

    free(piezaschromo);
#ifdef _DEBUG_MALLOC_		
    printf("Free, app_g.c, 512, piezaschromo\n");
#endif
}

void app2_objfunc_g(struct individual *critter)
// Función Objetivo, transforma el string cromosoma en un arreglo de piezas
{
   	unsigned mask = 1, tp, rt, bitpos, dir, salto, go = 1, m, b;
    int j, k, stop, i, ini, vueltas = 0, pIni = 0, valPos, nAsig = 0;
    int chrom[NumPie];
    TNodoAP *piezaschromo; // variable que apunta a todas las piezas del cromosoma generado
    if ((piezaschromo = (TNodoAP*) malloc(NumPie * sizeof (TNodoAP))) == NULL)
        nomemory("piezaschromo en objfunc");
#ifdef _DEBUG_MALLOC_		
    printf("Malloc, App_g.c, 440, piezaschromo, %d\n", NumPie * sizeof (TNodoAP));
#endif
    for(i=0; i<NumPie; i++) {
        arreglo_orden_g[i] = -1;
        arreglo_ocupado_g[i] = 0;
        chrom[i] = 0;
        arreglo_rotar_g[i] = 0;
    }
    for (i = 0; i < chromsize; i++) {
        if (i == (chromsize - 1)) //ultimo bloque
            stop = lchrom - (i * UINTSIZE);
        else
            stop = UINTSIZE;
        
        tp = critter->chrom[i];
        rt = critter->chmut[i];
        for (j = 0; j < stop; j++) {
            bitpos = j + UINTSIZE*i;
            if((i==0 && j >=bit_reservados_g) || i>0) {
                /*
                * Extrae segmento del cromosoma correspondiente a las piezas
                */
                if (tp & mask) chrom[bitpos - bit_reservados_g] = 1;
                else chrom[bitpos - bit_reservados_g] = 0;
/*
                printf("%i ", chrom[bitpos - bit_reservados_g]);
*/
                /*
                * Extrae segmento del cromosoma correspondiente a la orientacion de la pieza
                */
                if (rt & mask) arreglo_rotar_g[bitpos - bit_reservados_g] = 1; //Rotada
                else arreglo_rotar_g[bitpos - bit_reservados_g] = 0; //No rotada
            }
            tp = tp >> 1;
            rt = rt >> 1;
        }
    }
/*
    printf("\n");
*/
    /*
     * ini: Valor que se considera primero, 0 o 1, al recorrer el cromosoma
     */
    ini = 1;
    /*
     * dir: Direccion inicial en que se recorre el cromosoma
     * 0: der -> izq
     * 1: izq -> der
     */
    tp = critter->chrom[0];
    dir = (tp & mask)?1:0; 
    //Obtiene el valor del salto para recorrer el cromosoma
    salto = 1;
    for(i=0; i<3; i++) {
        tp = tp >> 1;
        for(b = 1, m = 2; m > i; m--) { b *= 2;}
        if(tp & mask) salto = salto + b;
    }
    
    vueltas = 0;
    while(go) { //mientras queden piezas por ordenar
        if(dir == 1) {//izq->der
            pIni = 0; //posicion inicial
            while(arreglo_ocupado_g[pIni] != 0) { //si ya se ordeno la pieza pasa a la de la derecha
                pIni++;
            }
            for(i=pIni; i<NumPie; i = i+salto) { //desde la posicion inicial hasta el final, con (i + salto) como valor de incremento
                if(arreglo_ocupado_g[i] == 0) { // si no ha sido agregado
                    valPos = chrom[i]; //obtiene el valor en esa posicion
                    arreglo_ocupado_g[i] = 1; //se marca como leida
                    if(valPos==ini) { //si es igual al valor buscado se agrega al listado de piezas ordenado
                        arreglo_orden_g[nAsig] = i; //se agrega al listado de piezas ordenado
                        if(arreglo_rotar_g[i]==1) {
                            piezaschromo[nAsig].ancho = piezasproblema[i].alto;
                            piezaschromo[nAsig].alto = piezasproblema[i].ancho;
                        } else {
                            piezaschromo[nAsig].alto = piezasproblema[i].alto;
                            piezaschromo[nAsig].ancho = piezasproblema[i].ancho;
                        }
                    } else {
                        piezaschromo[nAsig].ancho = 0;
                        piezaschromo[nAsig].alto = 0;
                        piezaschromo[nAsig].numero = 0;
                        piezaschromo[nAsig].cantidadpiezas = 1;
                    }
                    nAsig++;
                    if(nAsig == NumPie) go = 0; //si se ordenaron todas las piezas termina
                }
            }
        } else {//der->izq
            pIni = NumPie-1; //se ubica en la ultima posicion del string
            while(arreglo_ocupado_g[pIni] != 0) { //si ya se ordeno la pieza pasa a la de la izquierda
                pIni--;
            }
            for(i=pIni; i>=0; i = i-salto) { //desde la posicion inicial hasta el inicio del string, con (i + salto) como valor de retroceso
                if(arreglo_ocupado_g[i] == 0) { // si no ha sido agregado
                    valPos = chrom[i]; //obtiene el valor en esa posicion
                    arreglo_ocupado_g[i] = 1; //se marca como leida
                    if(valPos==ini) { //si es igual al valor buscado se agrega al listado de piezas ordenado
                        arreglo_orden_g[nAsig] = i; //se agrega al listado de piezas ordenado
                        if(arreglo_rotar_g[i]==1) {
                            piezaschromo[nAsig].ancho = piezasproblema[i].alto;
                            piezaschromo[nAsig].alto = piezasproblema[i].ancho;
                        } else {
                            piezaschromo[nAsig].alto = piezasproblema[i].alto;
                            piezaschromo[nAsig].ancho = piezasproblema[i].ancho;
                        }
                        piezaschromo[nAsig].numero = piezasproblema[i].numero;
                        piezaschromo[nAsig].cantidadpiezas = 1;
                    } else {
                        piezaschromo[nAsig].ancho = 0;
                        piezaschromo[nAsig].alto = 0;
                        piezaschromo[nAsig].numero = 0;
                        piezaschromo[nAsig].cantidadpiezas = 1;
                    }
                    nAsig++;
                    if(nAsig == NumPie) go = 0;
                }
            }
        }
        
        vueltas++;
        dir = (dir==0)?1:0; //se cambia la direccion en que se recorre el string
/*
        if(vueltas%2 == 0) ini = (ini==0)?1:0; //si ya se dio una vuelta completa, entonces se invierte el valor a buscar
*/
    }
    
/*
    for (k = 0; k < chromsize; k++) {
        if (k == (chromsize - 1)) //ultima
            stop = lchrom - (k * UINTSIZE);
        else
            stop = UINTSIZE;
        tp = critter->chrom[k];
        rt = critter->chmut[k];
        for (j = 0; j < stop; j++) {
            bitpos = j + UINTSIZE*k;
            // Entra al if si el bit actual es 1
            if (tp & mask) {
                //Asigna pieza desde arreglo piezasproblema
                if (rt & mask) {
                    // Efecta rotacin de piezas
                    piezaschromo[bitpos].ancho = piezasproblema[bitpos].alto;
                    piezaschromo[bitpos].alto = piezasproblema[bitpos].ancho;
                } else {
                    // No efectua rotacion de piezas
                    piezaschromo[bitpos].ancho = piezasproblema[bitpos].ancho;
                    piezaschromo[bitpos].alto = piezasproblema[bitpos].alto;
                }//End else
                piezaschromo[bitpos].numero = piezasproblema[bitpos].numero;
                piezaschromo[bitpos].cantidadpiezas = 1;
            } else {
                //Asigna solo ceros => pieza no se considera
                piezaschromo[bitpos].ancho = 0;
                piezaschromo[bitpos].alto = 0;
                piezaschromo[bitpos].numero = 0;
                piezaschromo[bitpos].cantidadpiezas = 1;
            }//End else
            tp = tp >> 1;
            rt = rt >> 1;
        }//End for
    }//End for
*/
/*
    critter->PEval = app2_funceval_g(piezaschromo, 0);
    critter->fitness = (float) critter->PEval.perdida;
*/

    free(piezaschromo);
#ifdef _DEBUG_MALLOC_		
    printf("Free, app_g.c, 512, piezaschromo\n");
#endif
}//End app2_objfunc_g

int app2_leearchivo_g(char *nombrearchivo) {
        FILE *fp;
        int i,num,alt,anc,lim,id=1, lala, lele;
        char nombre_archivo[100];

        sprintf(nombre_archivo, "%s%s", ruta_instancias, nombrearchivo);
        if((fp = fopen(nombre_archivo,"r"))== NULL){
                fprintf(outfp,"error al leer archivo %s\n",nombrearchivo);
                return 0;
        }

        //Inicialización de variables globales
        NumPie = 0;
        cantidadtipospiezas = 0;

        //Lee Ancho y Largo de la Lámina
        fscanf(fp,"%d %d %d %d",&AnchoPl,&AltoPl,&lala,&lele);
        //Lee cantidad de tipos de piezas del problema
        fscanf(fp,"%d",&num);

        //Solicita memoria para piezasdistintas
        if((piezasdistintas = (TNodoAP*) malloc((num+1)*sizeof(TNodoAP))) == NULL){
            nomemory("piezasdistintas en app_lecturaarchivo_g");
            #ifdef _DEBUG_MALLOC_		
            printf("Malloc, App_g.c, 238, piezasdistintas, %d\n", (num+1)*sizeof(TNodoAP));
            #endif
            return 0;
        }

        for(i=1;i<=num;i++) {
            // Lee ancho, alto y restricciones para cada tipo de pieza
            fscanf(fp,"%d %d %d",&anc,&alt,&lim);
            piezasdistintas[i].ancho          = anc;
            piezasdistintas[i].alto           = alt;
            piezasdistintas[i].numero         = id;
            piezasdistintas[i].cantidadpiezas = lim;
            //Incremento id++ si quiero que sólo cada tipo pieza tenga id distinto
            id++;
            NumPie=NumPie+lim;
        }//End for

        fclose(fp); //Cierra archivo del problema

        //Establece la cantidad máxima de tipos de piezas distintos del problema
        id--;
        cantidadtipospiezas=id;
        if(NumPie == 0) return 0;
        largo_cromosoma = bit_reservados_g + NumPie; //Define el largo del cromosoma
        fitness_inicial = (float) (AltoPl * AnchoPl); //Obtiene el fitness_inicial
        arreglo_ocupado_g = (int *) malloc(NumPie * sizeof(int)); //en este arreglo se guarda si la pieza fue ocupada o no
        arreglo_orden_g = (int *) malloc(NumPie * sizeof(int)); //arreglo que guarda el numero de la pieza que es ingresada en la posicion i
        arreglo_rotar_g = (int *) malloc(NumPie * sizeof(int)); //arreglo que indica si la pieza i debe ir rotada o no
        // Establece valor en variables utilizadas en función evaluación
        peso_func_obj    = 0.85;// Uso en función evaluación - Factor de la pérdida
        peso_uni      	 = 0.15;// Uso en función evaluación - Factor unificación de pérdidas
        peso_perdida     = 1.0; //0.6;	/*Factor de la componente perdida*/
        peso_distancia   = 0.2; //0.2;	/*Factor de la componente distancia*/
        peso_digregacion = 1.0; //0.2;	/*Factor de la componente digregacion*/
        app2_ordena_piezas_problema_g();
        return -1;
}//End app_leearchivo_g

void app2_free_g(void)
//Función que libera variables del problema
{
	free(piezasdistintas);
#ifdef _DEBUG_MALLOC_		
	printf("Free, app_g.c, 287, piezasdistintas\n");
#endif
    free(piezasproblema);
#ifdef _DEBUG_MALLOC_		
	printf("Free, app_g.c, 290, piezasproblema\n");
#endif
}//End app_free_g


void app2_genera_resultados_problema_g(int corrida, int tipo_problema, char *nombrearchivo)
//Función que imprime los resultados del problema corte de pieza guillotina
{
    if (encabezado_resultado_problema == 0) {
        //IMPRIME ENCABEZADO DE LOS RESULTADOS DEL PROBLEMA
        fprintf(rprofp, "CORRIDA;TIPO_PROBLEMA;ARCHIVO_PROBLEMA;CANTIDAD_PIEZAS;CANTIDAD_TIPOS_PIEZAS;");
        fprintf(rprofp, "ANCHO_LAMINA;LARGO_LAMINA;PESO_PERDIDA;PESO_DISTANCIA;PESO_DIGREGACION;");
        fprintf(rprofp, "MS_CALIDAD;MS_FITNESS;MS_PERDIDA_REAL;MS_DISTANCIA;MS_DIGREGACION;");
        fprintf(rprofp, "MS_AREA_OCUPADA;MS_NUMERO_PIEZAS_COLOCADAS;MS_NUMERO_PERDIDAS_GENERADAS;");
        fprintf(rprofp, "GENERACION_OCURRENCIA\n");
        encabezado_resultado_problema = 1; //Con ésto no imprime más el encabezado
    }//End if 
    //IMPRIME RESULTADOS DEL PROBLEMA
    fprintf(rprofp, "%d;%d;%s;%d;%d;%d;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%d;%d;%d\n",
            //CORRIDA;TIPO_PROBLEMA;ARCHIVO_PROBLEMA;CANTIDAD_PIEZAS;CANTIDAD_TIPOS_PIEZAS;ANCHO_LAMINA;LARGO_LAMINA;
            corrida, tipo_problema, nombrearchivo, NumPie, cantidadtipospiezas, AnchoPl, AltoPl,
            //PESO_PERDIDA;PESO_DISTANCIA;PESO_DIGREGACION;MS_CALIDAD;MS_FITNESS;		
            peso_perdida, peso_distancia, peso_digregacion, bestfit.PEval.calidad, bestfit.fitness,
            //MS_PERDIDA_REAL;MS_DISTANCIA;MS_DIGREGACION;
            bestfit.PEval.c_perdidareal, bestfit.PEval.c_distancia, bestfit.PEval.c_digregacion,
            //MS_AREA_OCUPADA;MS_NUMERO_PIEZAS_COLOCADAS;MS_NUMERO_PERDIDAS_GENERADAS;GENERACION_OCURRENCIA;
            bestfit.PEval.areaocupada, bestfit.PEval.piezas, bestfit.PEval.n_perdidas, bestfit.generation);
}//End app_genera_resultados_problema_g

void app2_ordena_piezas_problema_g(void)
//Obtiene Arreglo con todas las Piezas del Problema
{
	int i, j, k=0;

#define ORDEN_x_NINGUNO  				0
#define ORDEN_x_AREA 					1
#define ORDEN_x_LADO_HORIZONTAL         2
#define ORDEN_x_LADO_VERTICAL		    3
//#define TIPO_ORDEN 						ORDEN_x_AREA
#define TIPO_ORDEN 						ORDEN_x_NINGUNO

   	//Solicita memoria para piezasdistintas
   	if((piezasproblema = (TNodoAP*) malloc(NumPie*sizeof(TNodoAP))) == NULL)
   		nomemory("piezasproblema en app_ordena_piezas_problema_g");
#ifdef _DEBUG_MALLOC_		
   printf("Malloc, App_g.c, 237, piezasproblema, %d\n", NumPie*sizeof(TNodoAP));
#endif

#if (TIPO_ORDEN == ORDEN_x_NINGUNO)
   	//No hay ordenamiento
#elif(TIPO_ORDEN == ORDEN_x_AREA)
	//Ordenamiento Qsort por Area de cada pieza
	//Deja primero las piezas con area menor en forma ascendente con respecto al area de c/pieza
   	qsort(piezasdistintas, cantidadtipospiezas + 1, sizeof(TNodoAP), (void *) AreaNodoAPCompara_g);
#elif(TIPO_ORDEN == ORDEN_x_LADO_HORIZONTAL)
	//Ordenamiento Qsort horizontal (considera el ancho de cada pieza)
	//Deja primero las piezas con ancho menor en forma ascendente con respecto al ancho de c/pieza
   	qsort(piezasdistintas, cantidadtipospiezas + 1, sizeof(TNodoAP), (void *) HorizontalNodoAPCompara_g);
#elif(TIPO_ORDEN == ORDEN_x_LADO_VERTICAL)
	//Ordenamiento Qsort Vertical (considera el alto de cada pieza)
	//Deja primero las piezas con alto menor en forma ascendente con respecto al alto de c/pieza
   	qsort(piezasdistintas, cantidadtipospiezas + 1, sizeof(TNodoAP), (void *) VerticalNodoAPCompara_g);
#endif

   	// Una vez ordenadas las piezas, se leen de mayor a menor.
   	// Como el ordenamiento es de menor a mayor hay que leer las piezas de atras para adelante
   	for(i=cantidadtipospiezas;i>0;i--) {
      	for(j=1;j<=piezasdistintas[i].cantidadpiezas;j++) {
   			piezasproblema[k].ancho 				 = piezasdistintas[i].ancho;
   			piezasproblema[k].alto  				 = piezasdistintas[i].alto;
   			piezasproblema[k].numero				 = piezasdistintas[i].numero;
   			piezasproblema[k].cantidadpiezas = 1;
   			k++;
      	}//End for
   	}//End for
}//End app_ordena_piezas_problema_g