/*----------------------------------------------------------------------------*/
/* app_g.c - rutinas dependientes de la aplicación                            */
/*----------------------------------------------------------------------------*/
/*      PROBLEMA CORTE DE PIEZA GUILLOTINA BIDIMENSIONAL RESTRICTO 2.0        */
/*----------------------------------------------------------------------------*/

#include <stdlib.h>

#include "memory.h"
#include "sga.h"
#include "var_cp.h"

void app_liberamemlistaperdidas_cp(int perdidaexterna)
//Libera memoria de la lista de prdidas
{
   	TListaPE *LAux;
   	LAux = LPer;

   	if(LAux == NULL)	return;
   	while(LAux != NULL) {
      	LPer = LAux->prox;
      	free(LAux);
#ifdef _DEBUG_MALLOC_		
   printf("Free, app_g.c, 24, LAux\n");    // Libera LBusca
#endif
      
      	LAux = LPer;
   	}//End while
}//End app_liberamemlistaperdidas_g

// Agrega un elemento al final de la lista de prdidas, su parmetro
// inicial es un puntero apuntando al nodo final de la lista
void app_agregalistap_cp(int anc,int alt) {
    TListaPE *LAux;

    if((LAux = (TListaPE*) malloc(sizeof(TListaPE))) == NULL) nomemory("Laux en app_agregalistap_cp");
    
    #ifdef _DEBUG_MALLOC_		
        printf("Malloc, App_g.c, 115, LAux, %d\n", sizeof(TListaPE));
    #endif

    LAux->ancho = anc;
    LAux->alto = alt;
    LAux->prox = LPer;
    LPer = LAux;
} //End app_agregalistap_g

TNodoRE app_pieza2perdida_cp(int anc,int alt)
// Politica de Encaje : Se escoje la menor perdida de entre las menores...
{
   	TListaPE 	*LAux, *LBusca;
   	TNodoRE 	Ret;
	int 		d_ancho_min = AnchoPl, d_alto_min = AltoPl, d_ancho_actual, d_alto_actual;
   	int  		pos_nodo_best_fit = -1, j = 0;

   	LAux   = LPer;
  	LBusca = LPer;

   	if(LBusca == NULL) {
            Ret.ancho = -1;
            return Ret;
   	}//End if
   	while(LAux != NULL) {  // Mientras LAux.prox no apunte a NULL
            if((anc <= LAux->ancho) && (alt <= LAux->alto)) {
         	d_ancho_actual = LAux->ancho - anc;
        	d_alto_actual = LAux->alto - alt;
    		if((d_ancho_actual == 0) && (d_alto_actual == 0)){
                    pos_nodo_best_fit = j;				
                    break;
         	} else if(d_ancho_actual == 0){
                    if(d_alto_actual <= d_alto_min){
               		d_ancho_min = d_ancho_actual;
               		d_alto_min = d_alto_actual;
                        pos_nodo_best_fit = j;				
                    } else if((d_ancho_min != 0) && (d_alto_min != 0)){
                        //Me interesa no perder la pieza actual ya que por lo menos
                        //esta calza justo por un lado, pero la d_min NO...
            		d_ancho_min = d_ancho_actual;
            		d_alto_min = d_alto_actual;
                        pos_nodo_best_fit = j;				
                    }//End else if
         	} else if(d_alto_actual == 0){
                    if(d_ancho_actual <= d_ancho_min) {
                        d_ancho_min = d_ancho_actual;
                        d_alto_min = d_alto_actual;
                        pos_nodo_best_fit = j;				
                    } else if((d_ancho_min != 0) && (d_alto_min != 0)){
                        //Me interesa no perder la pieza actual ya que por lo menos
                        //esta calza justo por un lado, pero la d_min NO...
            		d_ancho_min = d_ancho_actual;
            		d_alto_min = d_alto_actual;
                        pos_nodo_best_fit = j;				
                    }//End else if
         	} else if((d_ancho_actual < d_ancho_min) || (d_alto_actual < d_alto_min)){
                    d_ancho_min = d_ancho_actual;
                    d_alto_min = d_alto_actual;
                    pos_nodo_best_fit = j;				
         	}//End else if
            }//End if
            j++;
            LAux = LAux->prox;
   	}//End while

	if(pos_nodo_best_fit == 0) {
            //Trata de calzar nueva pieza en el nodo cabeza de la lista de Prdidas
            Ret.ancho = LBusca->ancho; // Nueva pieza calza en primer nodo
            Ret.alto = LBusca->alto;   // Asigna nuevo nodo
            LPer = LBusca->prox;       // LPer queda apuntandoa al prximo

            free(LBusca);
            #ifdef _DEBUG_MALLOC_		
                printf("Free, app_g.c, 187, LBusca\n");             // Libera LBusca
            #endif
      
            return Ret;
   	} else if(pos_nodo_best_fit > 0) {
            LAux = LPer;
            LBusca = LPer;
            j = 1;
            while(LAux != NULL) {  // Mientras LAux.prox no apunte a NULL
        	LAux = LBusca->prox;
         	if(j == pos_nodo_best_fit) {
                    Ret.ancho = LAux->ancho;
                    Ret.alto = LAux->alto;
                    LBusca->prox = LAux->prox;

                    free(LAux);
                    #ifdef _DEBUG_MALLOC_		
                        printf("Free, app_g.c, 200, LAux\n");
                    #endif
                    return Ret;
         	}//End if
         	LBusca = LBusca->prox;
    		j++;
            }//End while
   	}//End elseif
   	Ret.ancho = -1; // Sale por aqui cuando hay una lista de piezas
   	return Ret;     // y la nueva pieza no calza en ninguna de las prdidas.
}//End app_pieza2perdida_g

int AreaNodoAPCompara_cp(TNodoAP *Nodoi, TNodoAP *Nodoj)
//Funcion que compara Nodos AP  utilizando como comparacion el area de cada pieza (ancho x alto)
{
   	if((Nodoi->ancho * Nodoi->alto) > (Nodoj->ancho * Nodoj->alto))
	   	return(1);
   	if((Nodoi->ancho * Nodoi->alto) < (Nodoj->ancho * Nodoj->alto))
	   	return(-1);
   	return(0);
}//End AreaNodoAPCompara_g

int HorizontalNodoAPCompara_cp(TNodoAP *Nodoi, TNodoAP *Nodoj)
//Funcion que compara Nodos AP  utilizando como comparacion el ancho de cada pieza
{
	if(Nodoi->ancho > Nodoj->ancho)
		return(1);
	if(Nodoi->ancho < Nodoj->ancho)
		return(-1);
	return(0);
}//End HorizontalNodoAPCompara_g

int VerticalNodoAPCompara_cp(TNodoAP *Nodoi, TNodoAP *Nodoj)
//Funcion que compara Nodos AP  utilizando como comparacion el alto de cada pieza
{
	if(Nodoi->alto > Nodoj->alto)
		return(1);
	if(Nodoi->alto < Nodoj->alto)
		return(-1);
	return(0);
}//End VerticalNodoAPCompara_g

void app_ordena_piezas_problema_cp(void)
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

//Función principal de lectura del archivo
int app_leearchivo_cp(char *nombrearchivo) {
        FILE *fp;
        int i,num,alt,anc,lim,id=1;
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
        fscanf(fp,"%d %d",&AnchoPl,&AltoPl);
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
        largo_cromosoma = bit_reservados_cp + NumPie; //Define el largo del cromosoma
        fitness_inicial = (float) (AltoPl * AnchoPl); //Obtiene el fitness_inicial

        // Establece valor en variables utilizadas en función evaluación
        peso_func_obj    = 0.85;// Uso en función evaluación - Factor de la pérdida
        peso_uni      	 = 0.15;// Uso en función evaluación - Factor unificación de pérdidas
        peso_perdida     = 1.0; //0.6;	/*Factor de la componente perdida*/
        peso_distancia   = 0.2; //0.2;	/*Factor de la componente distancia*/
        peso_digregacion = 1.0; //0.2;	/*Factor de la componente digregacion*/

        //Ordena las piezas y determina arreglo piezasproblema[]
        app_ordena_piezas_problema_cp();
        return -1;
}//End app_leearchivo_g

void app_free_cp(void)
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

// Funcion de evaluacin
TEval app_funceval_cp(TNodoAP *piezas) {
   	int i,acum,cont;
   	int Acan = 0,Acal = 0,Nuan,Nual;
   	int PerdExt;
   	int PieInc = NumPie;
   	int AreaPlaca = AnchoPl * AltoPl;
   	float AreaOcup,UnifPerd, TAU, CONSTANTE;
	int siguiente=1;

   	TEval Eval;
   	TNodoRE Perd;
   	TListaPE *Bus;

   	// Inicializa la Lista de Prdidas
   	LPer = NULL;

	// Se saltan las piezas de ancho y largo = 0
   	for(i=0;i<NumPie;i++){
            Acan = piezas[i].ancho;
            Acal = piezas[i].alto;
            if((Acan != 0) && (Acal != 0) && (Acan <= AnchoPl) && (Acal <= AltoPl)) {
                // Entra aqui si la pieza tiene dimensiones > 0 y cabe en la lmina
                siguiente = i+1;
                i = NumPie;
                //aqui se deberia crear la primera perdida a la derecha de la pieza
            } else PieInc--;
	}//End for

	if(siguiente<NumPie) {
            for(i=siguiente;i<NumPie;i++) {
        	Nuan = piezas[i].ancho;
         	Nual = piezas[i].alto;
      		if((Nuan != 0) && (Nual != 0) && (Nuan <= AnchoPl) && (Nual <= AltoPl)) {
                    Perd = app_pieza2perdida_cp(Nuan,Nual);
                    if(Perd.ancho != -1) { // La pieza nueva calza en alguna prdida dentro de la lista de prdidas
                        if((Nuan < Perd.ancho) && (Nual < Perd.alto)) { // La nueva pieza est dentro de una prdida
                            // => se generan 2 nuevas prdidas
                            // Define fragmento de prdida inferior (dentro del patrn y debajo de la nueva pieza)
                            app_agregalistap_cp(Perd.ancho,Perd.alto - Nual);
                            // Define fragmento de prdida derecho (dentro del patrn y al lado de la nueva pieza)
                            app_agregalistap_cp(Perd.ancho - Nuan,Nual);
                        } else if(Nual < Perd.alto)	 // La nueva pieza calza exacto a lo ancho de la prdida
                            // y queda una prdida inferior (por debajo de la nueva pieza)
                            //Agrega nueva prdida del mismo ancho de la prdida anterior pero de menor alto.
                            app_agregalistap_cp(Nuan,Perd.alto - Nual);
                        else if(Nuan < Perd.ancho)   // La nueva pieza calza exacto a lo largo de la pridida
                            // y queda una prdida derecha (por al lado de la nueva pieza)
                            //Agrega nueva prdida del mismo alto de la perdida anterior pero de menor ancho.
                            app_agregalistap_cp(Perd.ancho - Nuan,Nual);
                    } else {  // Nueva pieza no calzó en las pérdidas o no hay pérdidas.
                        if((Acal + Nual) <= AltoPl) {// Determina corte horizontal,
                            // la pieza la pone por debajo del patrn de corte
                            if(Acan > Nuan) {        // Genera Prdida al lado derecho de la nueva pieza
                                // y por debajo del patrn de corte actual
                                // Ingresa nueva Prdida de dimensiones (crecimiento a lo alto
                                // (hacia abajo) del patrn) x (Alto de la nueva pieza)
                                app_agregalistap_cp(Acan - Nuan,Nual);
                                // Dimensiona nuevo alto del patrn de corte actual
                                Acal = Acal + Nual;
                            } else if(Nuan > Acan) {   // Genera Prdida al lado derecho el patrn de corte
                                // y por encima de la nueva pieza.
                                // Ingresa nueva Prdida de dimensiones (crecimiento a lo ancho
                                // (hacia lado derecho) del patrn) x (Alto del patrn de corte antiguo)
                                app_agregalistap_cp(Nuan - Acan,Acal);
                                // Dimensiona nuevo ancho del patrn de corte actual
                                Acan = Nuan;
                                // Dimensiona nuevo alto del patrn de corte actual
                                Acal = Acal + Nual;
                            } else// No hay pérdida,el ancho de la nueva pieza coincide exacto con el del patrn
                                // de corte, pero hay que actualizar el alto del nuevo patrn de corte
                                Acal = Acal + Nual;
                        } else if((Acan + Nuan) <= AnchoPl) { // Determina corte vertical, pieza la pone
                            // al lado derecho del patrn de corte
                            if(Acal > Nual) {        // Genera Prdida bajo la nueva pieza
                                // y a la derecha del patrn de corte actual
                                // Ingresa nueva Prdida de dimensiones
                                // (Ancho nueva pieza) x (crecimiento a lo ancho (derecho) del patrn)
                                app_agregalistap_cp(Nuan,Acal - Nual);
                                // Dimensiona nuevo ancho del patrn de corte actual
                                Acan = Acan + Nuan;
                            } else if(Nual > Acal) {   // Genera Prdida bajo el patrn de corte
                                // y a la izquierda de la nueva pieza.
                                // Ingresa nueva Prdida de dimensiones (Ancho del patrn antes de insertar
                                // nueva pieza) x (crecimiento a lo alto (hacia abajo) del patrn)
                                app_agregalistap_cp(Acan,Nual - Acal);
                                // Dimensiona nuevo ancho del patrn de corte actual
                                Acan = Acan + Nuan;
                                // Dimensiona nuevo alto del patrn de corte actual
                                Acal = Nual;
                            } else// No hay pérdida,el alto de la nueva pieza coincide con el del patrn de corte
                                // pero hay que actualizar el ancho del nuevo patrn de corte
                                Acan = Acan + Nuan;
                        } else PieInc--; // Pieza excede el area disponible en la lámina.
                    }//End else
         	} else PieInc--; // Pieza excede el area disponible en la lámina.
        }//End for
    }//End if
    PerdExt = (AnchoPl - Acan) * AltoPl + Acan * (AltoPl - Acal);
    Bus = LPer;
    acum = 0;
    cont = 0;
    while (Bus != NULL) { //Calcula Prdida Interna
        acum = acum + (Bus->ancho * Bus->alto);
        Bus = Bus->prox;
        cont++;
    }//End while
    acum = acum + PerdExt;

    //UnFitness : Componente_Perdida + Componente_Distancia + Componente_Digregacion
    //Componente_Perdida     = Peso_Perdida * (Perdida_Total / Area_Placa)
    //Componente_Distancia   = Peso_Distancia * (SUMA_en_i(Restriccion_pieza_i - Numero_Pieza_i_Presentes) / SUMA_en_i(Restriccion_pieza_i))
    //Componente_Digregacion = Peso_Digregacion * (1 - EXP(- Numero_Perdidas / TAU)) donde TAU = K * Cantidad_de_Piezas_Distintas => La idea es tener las piezas de un mismo tipo, juntas
    //El sistema podra discriminar la digregacion dentro de 2 TAU, K: Hay que determinarlo experimentalmente.

    CONSTANTE = 1.0;
    TAU = CONSTANTE * (float) cantidadtipospiezas;
    /*
    Eval.c_perdidareal = (peso_perdida * (float) ((float) acum/(float) AreaPlaca)); //Componente Perdida
    Eval.c_distancia   = (peso_distancia * (float) ((float) (NumPie - PieInc) /(float) NumPie)); //Componente Distancia
    Eval.c_digregacion = (peso_digregacion * (1.0 - expf((float) (((float) -cont)/TAU)))); //Componente Digregacion
     */
    /******************** MOMENTANIO *******************/
    Eval.c_perdidareal = (float) acum;
    Eval.c_distancia = 0.0;
    Eval.c_digregacion = 0.0;
    /******************** MOMENTANIO *******************/

    Eval.perdida = Eval.c_perdidareal + Eval.c_distancia + Eval.c_digregacion;
    Eval.piezas = PieInc;
    AreaOcup = (float) (AreaPlaca - acum) / (float) AreaPlaca;
    if (cont == 0)
        UnifPerd = 1.0; //OJO -> Ver esto si est bien.
    else
        UnifPerd = (float) 1.0 / (float) cont;
    Eval.n_perdidas = cont; //Número de Pérdidas generadas
    Eval.areaocupada = AreaOcup;
    Eval.calidad = AreaOcup * peso_func_obj + UnifPerd*peso_uni;
    app_liberamemlistaperdidas_cp(PerdExt);
    return Eval;
}//End app_funceval


void app_objfuncfinal_cp(struct bestever *critter) {
    
}
// Funcin Objetivo, transforma el string cromosoma en un arreglo de piezas

void app_objfunc_cp(struct individual *critter) {
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
        arreglo_orden_cp[i] = -1;
        arreglo_ocupado_cp[i] = 0;
        chrom[i] = 0;
        arreglo_rotar_cp[i] = 0;
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
            if((i==0 && j >=bit_reservados_cp) || i>0) {
                /*
                * Extrae segmento del cromosoma correspondiente a las piezas
                */
                if (tp & mask) chrom[bitpos - bit_reservados_cp] = 1;
                else chrom[bitpos - bit_reservados_cp] = 0;

                /*
                * Extrae segmento del cromosoma correspondiente a la orientacion de la pieza
                */
                if (rt & mask) arreglo_rotar_cp[bitpos - bit_reservados_cp] = 1; //Rotada
                else arreglo_rotar_cp[bitpos - bit_reservados_cp] = 0; //No rotada
            }
            tp = tp >> 1;
            rt = rt >> 1;
        }
    }
    
    tp = critter->chrom[0];
    /*
     * ini: Valor que se considera primero, 0 o 1, al recorrer el cromosoma
     */
    ini = (tp & mask)?1:0;
    tp = tp >> 1;
    /*
     * dir: Direccion inicial en que se recorre el cromosoma
     * 0: der -> izq
     * 1: izq -> der
     */
    dir = (tp & mask)?1:0; 
    salto = 1;
    
    //Obtiene el valor del salto para recorrer el cromosoma
    for(i=0; i<3; i++) {
        tp = tp >> 1;
        for(b = 1, m = 2; m > i; m--) { b *= 2;}
        if(tp & mask) salto = salto + b;
    }
    
    vueltas = 0;
    while(go) { //mientras queden piezas por ordenar
        if(dir == 1) {//izq->der
            pIni = 0; //posicion inicial
            while(arreglo_ocupado_cp[pIni] != 0) { //si ya se ordeno la pieza pasa a la de la derecha
                pIni++;
            }
            for(i=pIni; i<NumPie; i = i+salto) { //desde la posicion inicial hasta el final, con (i + salto) como valor de incremento
                if(arreglo_ocupado_cp[i] == 0) { // si no ha sido agregado
                    valPos = chrom[i]; //obtiene el valor en esa posicion
                    arreglo_ocupado_cp[i] = 1; //se marca como leida
                    if(valPos==ini) { //si es igual al valor buscado se agrega al listado de piezas ordenado
                        arreglo_orden_cp[nAsig] = i; //se agrega al listado de piezas ordenado
                        if(arreglo_rotar_cp[i]==1) {
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
            while(arreglo_ocupado_cp[pIni] != 0) { //si ya se ordeno la pieza pasa a la de la izquierda
                pIni--;
            }
            for(i=pIni; i>=0; i = i-salto) { //desde la posicion inicial hasta el inicio del string, con (i + salto) como valor de retroceso
                if(arreglo_ocupado_cp[i] == 0) { // si no ha sido agregado
                    valPos = chrom[i]; //obtiene el valor en esa posicion
                    arreglo_ocupado_cp[i] = 1; //se marca como leida
                    if(valPos==ini) { //si es igual al valor buscado se agrega al listado de piezas ordenado
                        arreglo_orden_cp[nAsig] = i; //se agrega al listado de piezas ordenado
                        if(arreglo_rotar_cp[i]==1) {
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

    critter->PEval = app_funceval_cp(piezaschromo);
    critter->fitness = (float) critter->PEval.perdida;

    free(piezaschromo);
#ifdef _DEBUG_MALLOC_		
    printf("Free, app_g.c, 512, piezaschromo\n");
#endif
}//End app_objfunc_g

void app_genera_resultados_problema_cp(int corrida, int tipo_problema, char *nombrearchivo)
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

