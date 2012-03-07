/*----------------------------------------------------------------------------------*/
/* app_irb.c - rutinas dependientes de la aplicación                           		*/
/*----------------------------------------------------------------------------------*/
/*       PROBLEMA CORTE DE PIEZAS IRREGULARES MANEJADO CON CROMOSOMA BINARIO		*/
/*----------------------------------------------------------------------------------*/

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "var_pcpir.h"
#include "sga.h"
#include "memory.h"

//*****************************************************************
unsigned short usPiezaParaEncajar;
PIEZA	 *ppzaPiezaParaEncajar;
PUNTO	 *pptoPuntosPiezaParaEncajar;
unsigned uPiezaEncajada;
PIEZA	 *ppzaPiezaEncajada;
PUNTO	 *pptoPuntosPiezaEncajada;

unsigned short usPuntoPiezaParaEncajar;
unsigned short usPuntoPiezaEncajada;
unsigned short usPuntoAnteriorPiezaEncajada;

double	dYMayorPerfil;
double dAreaPiezasEncajadas;

unsigned short usPuntoUltimo;

double AVectorDesplazamiento;
double BVectorDesplazamiento;
double CVectorDesplazamiento;

double dDeltaX;
double dDeltaY;
double dDestinoX;
double dDestinoY;

BOOLEAN fPendienteInfinita;
double dPendienteDesplazamiento;

double dMayorDistanciaPuntoIzquierdoVector;
double dMayorDistanciaPuntoIzquierdoOrigen;
double dMayorDistanciaPuntoDerechoVector;
double dMayorDistanciaPuntoDerechoOrigen;
double dMayorDistanciaPuntoCentroOrigen;

double dDistanciaPuntoVector;
double dDistanciaPuntoOrigen;

unsigned short usPuntoIzquierdo;
unsigned short usPuntoDerecho;
unsigned short usPuntoCentro;

double dDistanciaDesplazamiento;
double dDistanciaAuxiliar;

int		iPuntoPerfil;
//*****************************************************************

void app_ordena_piezas_problema_irb(void)
//Obtiene Arreglo con todas las Piezas del Problema
{
}//End app_ordena_piezas_problema_irb

int app_leearchivo_irb(char *nombrearchivo)
//Función principal de lectura del archivo
{
	FILE 		*fp; //puntero que identifca el archivo de instancia
	char 		nombre_archivo[100];
	unsigned	uPiezaActual, uVezActual, usPuntoActual, usPuntoUltimo;
	unsigned	uCantidadVecesPiezaActual, uCantidadPuntosPiezaActual;
	double		dYMenorII, dXMenorIII, dYMenorIII, dXMenorIV, dAreaPieza, dX, dY;
	int			iMaximoPuntosPerfil = 0, Orientacion, cont_orientacion;

	uMaximoRotaciones=3; //0=Sin Rotación, 1=Una Rotación, 2=Dos Rotaciones, 3=Tres Rotaciones
	sprintf(nombre_archivo, "%s%s", ruta_instancias, nombrearchivo);
   	if((fp = fopen(nombre_archivo,"r"))== NULL){
		fprintf(outfp,"error al leer archivo %s\n",nombre_archivo);
		return 0;
   	}//End if
	
	//Formato Archivo
	//LINEA1		 : 	ANCHO_LAMINA 	CANT_TOTAL_PIEZAS
	//LINEAY_PIEZA_Z :	CANT_VECES_PIEZA_Z	AREA_PIEZA_Z CANT_PUNTOS_PIEZA_Z PUNTO1_X PUNTO1_Y PUNTO2_X PUNTO2_Y...PUNTON_X PUNTON_Y
	
	//Lee primera línea del archivo
	fscanf(fp, "%lf%d", &dAltoLamina, &uCantidadPiezas);
	
	//Asigna el mismo Ancho que Alto (la lámina es un cuadrado)
	dAnchoLamina = dAltoLamina;

	//Reserva memoria para la estructura que almacenará las piezas del problema 
	if ( (ppzaPiezas = (PIEZA*)malloc(uCantidadPiezas*sizeof(PIEZA))) == NULL ){
     	nomemory("ppzaPiezas en app_lecturaarchivo_ir");
		#ifdef _DEBUG_MALLOC_		
		   printf("Malloc, App_ir.c, 43, ppzaPiezas, %d\n", uCantidadPiezas*sizeof(PIEZA));
		#endif
		return 0;
	}//End if

	uPiezaActual = 0;
	dAreaTotalPiezas = 0.0; //almacena el área total que ocupan las piezas
	do{
		//Lee primeros datos de cada línea de piezas
		if (fscanf(fp, "%d%lf%d", &uCantidadVecesPiezaActual, &dAreaPieza, &uCantidadPuntosPiezaActual) < 1){
			fprintf(outfp,"Error al leer datos de piezas del archivo : %s!!\n",nombre_archivo);
            return 0;
		}//End if
		dAreaTotalPiezas += ((double)uCantidadVecesPiezaActual * dAreaPieza);
		//Llena los datos de cada pieza y reserva memoria
		for ( uVezActual = 0; uVezActual < uCantidadVecesPiezaActual; uVezActual++ ){
			ppzaPiezas[ uPiezaActual + uVezActual ].usCantidadPuntos = uCantidadPuntosPiezaActual;
			ppzaPiezas[ uPiezaActual + uVezActual ].dArea = dAreaPieza;
			for ( cont_orientacion = 0; cont_orientacion < 4; cont_orientacion++ ){
				//Solicita memoria para almacenar todos los puntos de cada pieza considerando 4 orientaciones de cada pieza (una en cada cuadrante)
				ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[cont_orientacion] = (PUNTO*)malloc(uCantidadPuntosPiezaActual*sizeof(PUNTO));
				if ( ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[cont_orientacion] == NULL ){
			     	nomemory("ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[X] en app_lecturaarchivo_ir");
					#ifdef _DEBUG_MALLOC_		
					   printf("Malloc, App_ir.c, 65, ppzaPiezas, %d\n", uCantidadPuntosPiezaActual*sizeof(PUNTO));
					#endif
					return 0;
				}//End if
			}//End for
		}//End for
		//Lee cada punto y lo almacena en la estructura de piezas, consideran a cada punto en cada uno de los 4 cuadrantes
		for ( usPuntoActual = 0; usPuntoActual < uCantidadPuntosPiezaActual; usPuntoActual++ ){
			//Lee punto
			if (fscanf(fp, "%lf%lf", &dX, &dY) < 1){
				fprintf(outfp,"Error al leer datos (puntos) de piezas del archivo : %s!!\n",nombre_archivo);
	            return 0;
        	}//End if
        	//Obtiene la forma de cada pieza esté rotada en 90°.
			for ( uVezActual = 0; uVezActual < uCantidadVecesPiezaActual; uVezActual++ ){               // .pptoPuntos[3]  	.pptoPuntos[0]      
					//Punto en Cuadrante I																//			  	  |
					ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[0][ usPuntoActual ].dXAbs = dX;	//			  IV  |  I
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[0][ usPuntoActual ].dYAbs = dY;  // 			 -----------
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[0][ usPuntoActual ].dXRel = dX;	//			  III |  II
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[0][ usPuntoActual ].dYRel = dY;	//				  |
					//Punto en Cuadrante II																   .pptoPuntos[2]	.pptoPuntos[1]
					ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[1][ usPuntoActual ].dXAbs = dY;
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[1][ usPuntoActual ].dYAbs = -dX;
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[1][ usPuntoActual ].dXRel = dY;
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[1][ usPuntoActual ].dYRel = -dX;
        			//Punto en Cuadrante III
					ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[2][ usPuntoActual ].dXAbs = -dX;
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[2][ usPuntoActual ].dYAbs = -dY;
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[2][ usPuntoActual ].dXRel = -dX;
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[2][ usPuntoActual ].dYRel = -dY;
        			//Punto en Cuadrante IV
					ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[3][ usPuntoActual ].dXAbs = -dY;
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[3][ usPuntoActual ].dYAbs = dX;
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[3][ usPuntoActual ].dXRel = -dY;
        			ppzaPiezas[ uPiezaActual + uVezActual ].pptoPuntos[3][ usPuntoActual ].dYRel = dX;
			}//End for
		}//End for
		uPiezaActual += uCantidadVecesPiezaActual; //avanza en la cantidad de piezas
	}while ( uPiezaActual < uCantidadPiezas );
	fclose(fp);
	//Con este for lo que se hace es desplazar las piezas Relativas (dXRel, dYRel) desde el cuadrante II, III, IV al cuadrante I.
	//Entonces, cada pieza se rota en 90° (4 posiciones => 4 formas distintas) y todas (4) las formas de cada pieza, quedan en el Cuadrante I
	for ( uPiezaActual = 0; uPiezaActual < uCantidadPiezas; uPiezaActual++ ){
		dYMenorII =	dXMenorIII = dYMenorIII = dXMenorIV = 0; 
		for ( usPuntoActual = 0; usPuntoActual < ppzaPiezas[ uPiezaActual ].usCantidadPuntos; usPuntoActual++ ){
			if ( ppzaPiezas[ uPiezaActual ].pptoPuntos[1][ usPuntoActual ].dYRel < dYMenorII )  // Y más negativo o más abajo en Cuadrante II
				dYMenorII = ppzaPiezas[ uPiezaActual ].pptoPuntos[1][ usPuntoActual ].dYRel;
			if ( ppzaPiezas[ uPiezaActual ].pptoPuntos[2][ usPuntoActual ].dXRel < dXMenorIII ) // X más negativo o más a la izquierda en Cuadrante III
				dXMenorIII = ppzaPiezas[ uPiezaActual ].pptoPuntos[2][ usPuntoActual ].dXRel;
			if ( ppzaPiezas[ uPiezaActual ].pptoPuntos[2][ usPuntoActual ].dYRel < dYMenorIII ) // Y más negativo o más abajo en Cuadrante III
				dYMenorIII = ppzaPiezas[ uPiezaActual ].pptoPuntos[2][ usPuntoActual ].dYRel;
			if ( ppzaPiezas[ uPiezaActual ].pptoPuntos[3][ usPuntoActual ].dXRel < dXMenorIV )  // X más negativo o más a la izquierda en Cuadrante IV
				dXMenorIV = ppzaPiezas[ uPiezaActual ].pptoPuntos[3][ usPuntoActual ].dXRel;
		}//End for
		//Cada punto relativo (Rel) de cada pieza se traslada al Cuadrante I.
		for ( usPuntoActual = 0; usPuntoActual < ppzaPiezas[ uPiezaActual ].usCantidadPuntos; usPuntoActual++ ){
			//Sube la componente dYRel de cada punto en dYMenorII para la Pieza del Cuadrante II hasta el Cuadrante I
			ppzaPiezas[ uPiezaActual ].pptoPuntos[1][ usPuntoActual ].dYRel = ppzaPiezas[ uPiezaActual ].pptoPuntos[1][ usPuntoActual ].dYRel - dYMenorII;
			//Desplaza hacia la derecha la componente dXRel de cada punto en dXMenorIII para la Pieza del Cuadrante III hasta el Cuadrante I
			ppzaPiezas[ uPiezaActual ].pptoPuntos[2][ usPuntoActual ].dXRel = ppzaPiezas[ uPiezaActual ].pptoPuntos[2][ usPuntoActual ].dXRel - dXMenorIII;
			//Sube la componente dYRel de cada punto en dYMenorIII para la Pieza del Cuadrante III hasta el Cuadrante I
			ppzaPiezas[ uPiezaActual ].pptoPuntos[2][ usPuntoActual ].dYRel = ppzaPiezas[ uPiezaActual ].pptoPuntos[2][ usPuntoActual ].dYRel - dYMenorIII;
			//Desplaza hacia la derecha la componente dXRel de cada punto en dXMenorIV para la Pieza del Cuadrante IV hasta el Cuadrante I
			ppzaPiezas[ uPiezaActual ].pptoPuntos[3][ usPuntoActual ].dXRel = ppzaPiezas[ uPiezaActual ].pptoPuntos[3][ usPuntoActual ].dXRel - dXMenorIV;
		}//End for
	}//End for
	//Llena la Estructura DATOSPIEZA de cada Pieza
	//Por cada orientación 0:0°, 1:90°, 2:180°, 3:270°
	for ( Orientacion = 0; Orientacion <= uMaximoRotaciones; Orientacion++ ){
		//Para cada pieza obtiene uPuntoXMenor,uPuntoYMenorXMenor,uPuntoYMenorXMayor,uPuntoYMayorXMenor,uPuntoYMayorXMayor,dAltura(alto de la pieza)
		for ( uPiezaActual = 0; uPiezaActual < uCantidadPiezas; uPiezaActual++ ){
			ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoXMenor =
			ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMenor =
			ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMenor =
			ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMayor =
			ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMayor = 0;
			//Para cada punto de cada pieza
			for ( usPuntoActual = 0; usPuntoActual < ppzaPiezas[uPiezaActual].usCantidadPuntos; usPuntoActual++ ){
				//Obtiene uPuntoXMenor
				if ( ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dXRel < ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoXMenor ].dXRel )
					ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoXMenor = usPuntoActual;
				//Obtiene uPuntoYMenorXMenor
				if ( ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dYRel < ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMenor ].dYRel )
					ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMenor = usPuntoActual;
				else if ( ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dYRel == ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMenor ].dYRel &&
						  ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dXRel < ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMenor ].dXRel )
					ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMenor = usPuntoActual;
				//Obtiene uPuntoYMayorXMenor
				if ( ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dYRel > ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMenor ].dYRel )
					ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMenor = usPuntoActual;
				else if ( ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dYRel == ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMenor ].dYRel &&
						  ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dXRel < ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMenor ].dXRel )
					ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMenor = usPuntoActual;
				//Obtiene uPuntoYMenorXMayor
				if ( ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dYRel < ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMayor ].dYRel )
					ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMayor = usPuntoActual;
				else if ( ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dYRel == ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMayor ].dYRel &&
						  ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dXRel > ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMayor ].dXRel )
					ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMayor = usPuntoActual;
				//Obtiene uPuntoYMayorXMayor
				if ( ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dYRel > ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMayor ].dYRel )
					ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMayor = usPuntoActual;
				else if ( ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dYRel == ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMayor ].dYRel &&
						  ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][usPuntoActual].dXRel > ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMayor ].dXRel )
					ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMayor = usPuntoActual;
			}//End for
			//Obtiene ancho relativo (en altura) que ocupa la pieza
			ppzaPiezas[uPiezaActual].Datos[Orientacion].dAltura = ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMayorXMayor ].dYRel -
					ppzaPiezas[uPiezaActual].pptoPuntos[Orientacion][ ppzaPiezas[uPiezaActual].Datos[Orientacion].uPuntoYMenorXMayor ].dYRel;
		}//End for
	}//End for
	//Obtiene cantidad máxima de puntos del perfil a lo alto de cada pieza
	for ( uPiezaActual = 0; uPiezaActual < uCantidadPiezas; uPiezaActual++ ){
		usPuntoUltimo = ppzaPiezas[ uPiezaActual ].Datos[0].uPuntoYMayorXMenor;
		for ( usPuntoActual = ppzaPiezas[ uPiezaActual ].Datos[0].uPuntoYMenorXMenor;
			  usPuntoActual != usPuntoUltimo;
			  usPuntoActual = ( usPuntoActual + 1 ) % ppzaPiezas[ uPiezaActual ].usCantidadPuntos ){
			iMaximoPuntosPerfil++;
			if ( ( usPuntoActual + 1 ) % ppzaPiezas[ uPiezaActual ].usCantidadPuntos == ppzaPiezas[ uPiezaActual ].Datos[0].uPuntoYMayorXMenor )
				usPuntoUltimo = ( usPuntoUltimo + 1 ) % ppzaPiezas[ uPiezaActual ].usCantidadPuntos;
		}//End for
	}//End for
	//Reserva memoria para cada perfil (4 perfiles)
	for ( cont_orientacion = 0; cont_orientacion < 4; cont_orientacion++ ){
		if ( (pPerfil[cont_orientacion] = (PUNTO**)malloc(2*iMaximoPuntosPerfil*sizeof(PUNTO*))) == NULL ){
	     	nomemory("pPerfil[X] en app_lecturaarchivo_ir");
			#ifdef _DEBUG_MALLOC_		
			   printf("Malloc, App_ir.c, 188, pPerfil[X], %d\n", 2*iMaximoPuntosPerfil*sizeof(PUNTO*));
			#endif
			return 0;
		}//End for
	}//End for
	//Reserva memoria para Perfil Auxiliar
	if ( (pPerfilAux1 = (PUNTO**)malloc(2*iMaximoPuntosPerfil*sizeof(PUNTO*))) == NULL ){
     	nomemory("pPerfilAux1 en app_lecturaarchivo_ir");
		#ifdef _DEBUG_MALLOC_		
		   printf("Malloc, App_ir.c, 198, pPerfilAux1, %d\n", 2*iMaximoPuntosPerfil*sizeof(PUNTO*));
		#endif
		return 0;
	}
	
	//Setea variables globales
	largo_cromosoma = uCantidadPiezas; //Define el largo del cromosoma
	fitness_inicial = 99999999.0; //Obtiene el fitness_inicial
	
	return -1;
}//End app_leearchivo_irb

void app_free_irb(void)
//Función que libera variables del problema
{
	unsigned	uPiezaActual;
	int			cont_orientacion;

	for ( uPiezaActual = 0; uPiezaActual < uCantidadPiezas; uPiezaActual++ ){
		for ( cont_orientacion = 0; cont_orientacion < 4; cont_orientacion++ )
			free(ppzaPiezas[ uPiezaActual ].pptoPuntos[cont_orientacion]);
	}//End for
	free(ppzaPiezas);
	for ( cont_orientacion = 0; cont_orientacion < 4; cont_orientacion++ )
		free(pPerfil[cont_orientacion]);
	free(pPerfilAux1);
}//End app_free_irb

double app_EncajePrimeraPieza_irb(unsigned Orientacion)
//Encaja la primera pieza considerando la Orientacion dada
{
	int i;
	double dXMax = 0.0;
	//Los puntos Relativos (dXRel, dYRel) se asignan a los puntos Absolutos (dXAbs, dYAbs)
	for ( usPuntoPiezaParaEncajar = 0; usPuntoPiezaParaEncajar < ppzaPiezaParaEncajar->usCantidadPuntos;
		usPuntoPiezaParaEncajar++ ){
		pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs = pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXRel;
		pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs = pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYRel;
		//Intenta obtener el valor de X mayor para la pieza ppzaPiezaParaEncajar
		if ( pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs > dXMax )
			dXMax = pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs;
	}//End for
	dYMayorPerfil = pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor ].dYRel;

	// Meter al perfil[Orientacion] 
	iInicioPerfil[Orientacion] = 0;
	i = 0;
	usPuntoUltimo = ppzaPiezas[ usPiezaParaEncajar ].Datos[Orientacion].uPuntoYMayorXMayor;
	iPuntoMasIzquierdaAbajo[Orientacion] = 0;
	//For que introduce en el perfil(orientacion) los puntos desde el más abajo a la derecha hacia arriba 
	for ( usPuntoPiezaParaEncajar = ppzaPiezas[ usPiezaParaEncajar ].Datos[Orientacion].uPuntoYMenorXMayor;
		  usPuntoPiezaParaEncajar != usPuntoUltimo;
		  usPuntoPiezaParaEncajar = ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ){
		//Introduce el punto en el Perfil
		pPerfil[Orientacion][ i ] = pptoPuntosPiezaParaEncajar + usPuntoPiezaParaEncajar;
		//Intenta obtener el punto más a la izquierda y más abajo
		if ( pPerfil[Orientacion][ i ]->dXAbs < pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] ]->dXAbs )
			iPuntoMasIzquierdaAbajo[Orientacion] = i;
		else if ( pPerfil[Orientacion][ i ]->dXAbs == pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] ]->dXAbs &&
				  pPerfil[Orientacion][ i ]->dYAbs < pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] ]->dYAbs )
			iPuntoMasIzquierdaAbajo[Orientacion] = i;
		//Mecanismo de salida del for: Esto para que pueda tomar el último punto : usPuntoUltimo
		if ( ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos == ppzaPiezas[ usPiezaParaEncajar ].Datos[Orientacion].uPuntoYMayorXMayor )
			usPuntoUltimo = ( usPuntoUltimo + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos;
//if ( gen==maxgen )
//printf("[%d](%f,%f);",i,pPerfil[Orientacion][ i ]->dXAbs,pPerfil[Orientacion][ i ]->dYAbs);
		i++;
	}//End for
	iFinPerfil[Orientacion] = i - 1;
	return( dXMax ); //Retorna el punto más a la derecha.
}//End app_EncajePrimeraPieza_irb

void app_EncajeEnBanda_irb(unsigned Orientacion)
//Función que Encaja en banda: La pieza será colocada arriba del Perfil
{
	//Cada punto de pptoPuntosPiezaParaEncajar (la pieza a encajar) se sube hasta el último punto (el más arriba) del Perfil
	for ( usPuntoPiezaParaEncajar = 0; usPuntoPiezaParaEncajar < ppzaPiezaParaEncajar->usCantidadPuntos;
		usPuntoPiezaParaEncajar++ ){
		pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs = pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXRel;
		pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs = pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYRel
			+ pPerfil[Orientacion][ iFinPerfil[Orientacion] ]->dYAbs;
	}//End for
}//End app_EncajeEnBanda_irb

void app_DeterminaPosicionInicial_irb(unsigned Orientacion, double dMax)
//Función que Ubica la pieza en el punto de partida desde la derecha
{
	double dCorrimiento;
	double r, s;

	if ( iPuntoMasIzquierdaAbajo[Orientacion] == iInicioPerfil[Orientacion] ||
		 pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] ]->dYAbs == pPerfil[Orientacion][ iInicioPerfil[Orientacion] ]->dYAbs )
		dCorrimiento = 0;
	else if ( iPuntoMasIzquierdaAbajo[Orientacion] == iFinPerfil[Orientacion] ||
		 pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] ]->dYAbs == pPerfil[Orientacion][ iFinPerfil[Orientacion] ]->dYAbs )
		dCorrimiento = -1.0 * ppzaPiezaParaEncajar->Datos[Orientacion].dAltura;
	else {
		s = pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] ]->dYAbs - pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] - 1 ]->dYAbs;
		r = pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] + 1 ]->dYAbs - pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] ]->dYAbs;
		if ( r + s == 0.0 )
			dCorrimiento = 0;
		else
			dCorrimiento = -1.0 * (double)( ( s / ( r + s ) ) * ppzaPiezaParaEncajar->Datos[Orientacion].dAltura);
//		dCorrimiento = -(double)(ppzaPiezaParaEncajar->dAltura / 2.0);
	}//End else
	for ( usPuntoPiezaParaEncajar = 0;
		  usPuntoPiezaParaEncajar < ppzaPiezaParaEncajar->usCantidadPuntos;
		  usPuntoPiezaParaEncajar++ ){

		pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs = 
//***************** LINEA MODIFICADA **************************
//			dMax + 1000.0 +	pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXRel;
			dMax + pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXRel;
//***************** LINEA MODIFICADA **************************

		pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs =
			pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] ]->dYAbs	+
			pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYRel	+ dCorrimiento
//			-
//			(double)(ppzaPiezaParaEncajar->dAltura / 2.0)
			;
	}//End for
	dDeltaY = 0.0;
	if ( pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor ].dYAbs < 0.0 )
		dDeltaY = -1.0 * pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor ].dYAbs;
	if ( pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor ].dYAbs > dAltoLamina )
		dDeltaY = -1.0 * ( pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor ].dYAbs - dAltoLamina );
	if ( dDeltaY != 0.0 )
		for ( usPuntoPiezaParaEncajar = 0;
			usPuntoPiezaParaEncajar < ppzaPiezaParaEncajar->usCantidadPuntos;
			usPuntoPiezaParaEncajar++ ){
			pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs = 
				pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs + dDeltaY;
		}//End for
	dDestinoX = 0.0;
	dDestinoY = fabs( pptoPuntosPiezaParaEncajar[ 0 ].dYAbs );// - 0.00001;
}//End app_DeterminaPosicionInicial_irb

double app_dObtieneDistanciaPuntoLado_irb( double xP, double yP,	double xA, double yA, double xB, double yB )
//Función que obtiene distancia punto lado
{
	double dDistanciaPA;
	double dDistanciaPB;
	double fabs();
	double xQ;

	if ( yA == yB ){
		dDistanciaPA = fabs( xA - xP );
		dDistanciaPB = fabs( xB - xP );
		if ( dDistanciaPA < dDistanciaPB )
			return ( dDistanciaPA );
		return ( dDistanciaPB );		
	}else {
		xQ = ( yP - yA ) * ( ( xB - xA ) / ( yB - yA ) ) + xA;
		return ( fabs( xP - xQ ) );
	}//End if
}//End app_dObtieneDistanciaPuntoLado_irb

void app_BuscaDistanciaDesplazamiento_irb(unsigned Orientacion)
//Función que Mide y compara distancia al eje y
{
	BOOLEAN	fPrimero = TRUE;

	if ( 0 ){
		usPuntoUltimo = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor;
		for ( usPuntoPiezaParaEncajar = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor;
			  usPuntoPiezaParaEncajar != usPuntoUltimo;
			  usPuntoPiezaParaEncajar = ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ){
			printf( "(%f,%f)\n",
								pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs,
								pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs);
			if ( ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos == ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor )
				usPuntoUltimo = ( usPuntoUltimo + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos;
		}//End for
		for ( iPuntoPerfil = iInicioPerfil[Orientacion]; iPuntoPerfil <= iFinPerfil[Orientacion]; iPuntoPerfil++ )
			printf( "(%f,%f)\n",
								pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs,
								pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs);
	}//End if
	/* Punto del perfil y lado de la pieza */
	usPuntoUltimo = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor;
	for ( usPuntoPiezaParaEncajar = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor;
		  usPuntoPiezaParaEncajar != usPuntoUltimo;
		  usPuntoPiezaParaEncajar = ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ){
		for ( iPuntoPerfil = iInicioPerfil[Orientacion]; iPuntoPerfil <= iFinPerfil[Orientacion]; iPuntoPerfil++ ){
			if ( pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs < pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs &&
				pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs > pptoPuntosPiezaParaEncajar[ ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ].dYAbs ){
				dDistanciaAuxiliar = app_dObtieneDistanciaPuntoLado_irb(
								pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs,
								pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs,
								pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs,
								pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs,
								pptoPuntosPiezaParaEncajar[ ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ].dXAbs,
								pptoPuntosPiezaParaEncajar[ ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ].dYAbs );
				if ( fPrimero || dDistanciaAuxiliar < dDistanciaDesplazamiento ){
					dDistanciaDesplazamiento = dDistanciaAuxiliar;
					fPrimero = FALSE;
				}//End if
			}//End if
		}//End for
		if ( ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos == ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor )
			usPuntoUltimo = ( usPuntoUltimo + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos;
	}//End for
	/* Punto de la pieza y lado del perfil */
	for ( iPuntoPerfil = iInicioPerfil[Orientacion]; iPuntoPerfil < iFinPerfil[Orientacion]; iPuntoPerfil++ ){
		usPuntoUltimo = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor;
		for ( usPuntoPiezaParaEncajar = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor;
			  usPuntoPiezaParaEncajar != usPuntoUltimo;
			  usPuntoPiezaParaEncajar = ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ){
			if ( pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs > pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs &&
				 pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs < pPerfil[Orientacion][ iPuntoPerfil + 1 ]->dYAbs ){
				dDistanciaAuxiliar = app_dObtieneDistanciaPuntoLado_irb(
								pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs,
								pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs,
								pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs,
								pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs,
								pPerfil[Orientacion][ iPuntoPerfil + 1 ]->dXAbs,
								pPerfil[Orientacion][ iPuntoPerfil + 1 ]->dYAbs );
				if ( fPrimero || dDistanciaAuxiliar < dDistanciaDesplazamiento ){
					dDistanciaDesplazamiento = dDistanciaAuxiliar;
					fPrimero = FALSE;
				}//End if
			}//End if
			if ( ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos == ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor )
				usPuntoUltimo = ( usPuntoUltimo + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos;
		}//End for
	}//End for
	for ( iPuntoPerfil = iInicioPerfil[Orientacion]; iPuntoPerfil <= iFinPerfil[Orientacion]; iPuntoPerfil++ ){
		usPuntoUltimo = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor;
		for ( usPuntoPiezaParaEncajar = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor;
			  usPuntoPiezaParaEncajar != usPuntoUltimo;
			  usPuntoPiezaParaEncajar = ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ){
			if ( pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs == pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs ){
				
				if ( (usPuntoPiezaParaEncajar != ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor && usPuntoPiezaParaEncajar != ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor) ||
					(usPuntoPiezaParaEncajar == ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor && ( iPuntoPerfil == iFinPerfil[Orientacion] || pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dYAbs < pPerfil[Orientacion][ iPuntoPerfil + 1 ]->dYAbs )) ||
					(usPuntoPiezaParaEncajar == ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor && pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs > pPerfil[Orientacion][ iPuntoPerfil - 1 ]->dYAbs) ){

					dDistanciaAuxiliar = pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs -
										 pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs;
					if ( fPrimero || dDistanciaAuxiliar < dDistanciaDesplazamiento ){
						dDistanciaDesplazamiento = dDistanciaAuxiliar;
						fPrimero = FALSE;
					}//End if
				}//End if
			}//End if
			if ( ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos == ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor )
				usPuntoUltimo = ( usPuntoUltimo + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos;
		}//End for
	}//End for
	usPuntoUltimo = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor;
	for ( usPuntoPiezaParaEncajar = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor;
		  usPuntoPiezaParaEncajar != usPuntoUltimo;
		  usPuntoPiezaParaEncajar = ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ){
		if ( pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs < dDistanciaDesplazamiento )
			dDistanciaDesplazamiento = pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs;
		if ( ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos == ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor )
			usPuntoUltimo = ( usPuntoUltimo + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos;
	}//End for
}//End app_BuscaDistanciaDesplazamiento_irb

void app_DesplazaPieza_irb(void)
//Función desplaza pieza
{
	for ( usPuntoPiezaParaEncajar = 0;
		usPuntoPiezaParaEncajar < ppzaPiezaParaEncajar->usCantidadPuntos;
		usPuntoPiezaParaEncajar++ ){
		if ( ( pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs = 
			pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs
			-
			dDistanciaDesplazamiento ) < 0.0 ){
			printf("Falla\n");
			printf("x:%f,desp:%f,xfinal:%f\n",
				pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs + dDistanciaDesplazamiento,
				dDistanciaDesplazamiento,
				pptoPuntosPiezaParaEncajar[ usPuntoPiezaParaEncajar ].dXAbs );
			//exit(1);
		}//End if
	}//End for
}//End app_DesplazaPieza_irb

double app_ActualizaPerfil_irb(unsigned Orientacion)
//Función que actualiza el perfil
{
	int iPuntoPerfilAntiguo;
	int iPuntoPerfilNuevo;

	BOOLEAN fPiezaIntegrada;
	BOOLEAN fComienzaPieza;

	int	i;
	double dMaximoX;

	iPuntoPerfilAntiguo = 0;
	iPuntoPerfilNuevo = 0;
	fPiezaIntegrada = 0;
	fComienzaPieza = 0;
	while ( iPuntoPerfilAntiguo <= iFinPerfil[Orientacion] || !fPiezaIntegrada ){
		if ( iPuntoPerfilAntiguo <= iFinPerfil[Orientacion] &&
			pPerfil[Orientacion][ iPuntoPerfilAntiguo ]->dYAbs < pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor ].dYAbs ){

			pPerfilAux1[ iPuntoPerfilNuevo ] = pPerfil[Orientacion][ iPuntoPerfilAntiguo ];
			iPuntoPerfilNuevo++;
			iPuntoPerfilAntiguo++;

		} else if ( !fComienzaPieza &&
			iPuntoPerfilAntiguo <= iFinPerfil[Orientacion] &&
			pPerfil[Orientacion][ iPuntoPerfilAntiguo ]->dYAbs == pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor ].dYAbs &&
			pPerfil[Orientacion][ iPuntoPerfilAntiguo ]->dYAbs > 0 ){

			pPerfilAux1[ iPuntoPerfilNuevo ] = pPerfil[Orientacion][ iPuntoPerfilAntiguo ];
			iPuntoPerfilNuevo++;
			iPuntoPerfilAntiguo++;
			fComienzaPieza = 1;

		} else{

			/*Incorporacion del perfil de la pieza al perfil nuevo*/
			usPuntoUltimo = ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor;
			for ( usPuntoPiezaParaEncajar = ( pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor ].dYAbs == 0 )?ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMayor:ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMenorXMenor;
				  usPuntoPiezaParaEncajar != usPuntoUltimo;
				  usPuntoPiezaParaEncajar = ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos ){
				
				pPerfilAux1[ iPuntoPerfilNuevo ] = pptoPuntosPiezaParaEncajar + usPuntoPiezaParaEncajar;
				iPuntoPerfilNuevo++;

				if ( ( usPuntoPiezaParaEncajar + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos == ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor )
					usPuntoUltimo = ( usPuntoUltimo + 1 ) % ppzaPiezaParaEncajar->usCantidadPuntos;
			}//End for
			fPiezaIntegrada = 1;

			while ( iPuntoPerfilAntiguo <= iFinPerfil[Orientacion]
					&&
					pPerfil[Orientacion][ iPuntoPerfilAntiguo ]->dYAbs < pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor ].dYAbs ){
				iPuntoPerfilAntiguo++;
			}//End while
			if ( iPuntoPerfilAntiguo <= iFinPerfil[Orientacion] && 
				 pPerfil[Orientacion][ iPuntoPerfilAntiguo ]->dYAbs >= pptoPuntosPiezaParaEncajar[ ppzaPiezaParaEncajar->Datos[Orientacion].uPuntoYMayorXMenor ].dYAbs )
				do {

					pPerfilAux1[ iPuntoPerfilNuevo ] = pPerfil[Orientacion][ iPuntoPerfilAntiguo ];
					iPuntoPerfilNuevo++;
					iPuntoPerfilAntiguo++;

				} while ( iPuntoPerfilAntiguo <= iFinPerfil[Orientacion] );
		}//End else
	}//End while
	iFinPerfil[Orientacion] = iPuntoPerfilNuevo - 1;

	pPerfilAux2 = pPerfil[Orientacion];
	pPerfil[Orientacion] = pPerfilAux1;
	pPerfilAux1 = pPerfilAux2;

	if ( pPerfil[Orientacion][ iFinPerfil[Orientacion] ]->dYAbs == pPerfil[Orientacion][ iFinPerfil[Orientacion] - 1 ]->dYAbs )
		iFinPerfil[Orientacion]--;

	if ( iFinPerfil[Orientacion] >= 3 ){
		for ( iPuntoPerfil = iInicioPerfil[Orientacion]; iPuntoPerfil + 2 <= iFinPerfil[Orientacion]; iPuntoPerfil++ ){
			if ( pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs == pPerfil[Orientacion][ iPuntoPerfil + 1 ]->dYAbs &&
				 ( pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs == pPerfil[Orientacion][ iPuntoPerfil + 1 ]->dXAbs ||
				   pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs == pPerfil[Orientacion][ iPuntoPerfil + 2 ]->dYAbs ) ){
				for ( i = iPuntoPerfil + 1; i < iFinPerfil[Orientacion]; i++ )
					pPerfil[Orientacion][ i ] = pPerfil[Orientacion][ i + 1 ];
				iFinPerfil[Orientacion]--;
				iPuntoPerfil--;
			}//End if
		}//End for
	}//End if

	iPuntoMasIzquierdaAbajo[Orientacion] = iInicioPerfil[Orientacion];
	dMaximoX = 0.0;
	for ( iPuntoPerfil = iInicioPerfil[Orientacion]; iPuntoPerfil <= iFinPerfil[Orientacion]; iPuntoPerfil++ ){
		if ( pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs < pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion] ]->dXAbs )
			iPuntoMasIzquierdaAbajo[Orientacion] = iPuntoPerfil;
		else if ( pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs == pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion]  ]->dXAbs &&
				  pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs < pPerfil[Orientacion][ iPuntoMasIzquierdaAbajo[Orientacion]  ]->dYAbs )
			iPuntoMasIzquierdaAbajo[Orientacion] = iPuntoPerfil;
		if ( pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs > dMaximoX )
			dMaximoX = pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs;
//		if ( gen==maxgen )
//			printf("[%d](%f,%f);",iPuntoPerfil,pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs,pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs);
	}//End for
//	if ( gen==maxgen ){
//		printf("\n[%d]\n",iPuntoMasIzquierdaAbajo[Orientacion]);
//		getchar();
//	}
	return( dMaximoX );
}//End app_ActualizaPerfil_irb

double app_funceval_irb(unsigned Orientacion, double dMaximoX, double dAreaPiezasEncajadas)
// Función de evaluación: Determina el área que ocupa el perfil...
{
	double dAreaPerfil;
	int	iPuntoPerfil;

	dAreaPerfil = 0.0;
	for ( iPuntoPerfil = iInicioPerfil[Orientacion]; iPuntoPerfil < iFinPerfil[Orientacion]; iPuntoPerfil++ )
		dAreaPerfil = dAreaPerfil +
					  ( pPerfil[Orientacion][ iPuntoPerfil ]->dXAbs + pPerfil[Orientacion][ iPuntoPerfil + 1 ]->dXAbs ) *
					  ( pPerfil[Orientacion][ iPuntoPerfil + 1 ]->dYAbs - pPerfil[Orientacion][ iPuntoPerfil ]->dYAbs );
	dAreaPerfil = dAreaPerfil / 2.0;

	//return( 1.0 / ( ( dAreaPerfil - dAreaPiezasEncajadas ) + dMaximoX * dAltoLamina + 1.0 ) );
	//return(( dAreaPerfil - dAreaPiezasEncajadas ) + dMaximoX * dAltoLamina );
	
	//Se trata de minimizar la diferencia entre el área del perfil y el área que ocupan las piezas
	return(( dAreaPerfil - dAreaPiezasEncajadas ));
}//End app_funceval_irb

void app_objfunc_irb(struct individual *critter)
// Función Objetivo, transforma el string cromosoma en un arreglo de piezas
{
   	unsigned 	mask=1;
   	unsigned 	tp;
   	int 		j, k, stop;
	unsigned 	bitpos;
	double 		dValorObjetivo[ 4 ];
	double 		dXMax[4];
    unsigned 	i, Orientacion;
 
	dAreaPiezasEncajadas = 0.0;
	i=0; //indica cantidad de piezas consideradas en LAMINA
   	for(k = 0; k < chromsize; k++) {
    	if(k == (chromsize-1))
         	stop = lchrom-(k*UINTSIZE);
      	else
         	stop = UINTSIZE;
	   	tp  = critter->chrom[k];
      	for(j = 0; j < stop; j++) {
         	bitpos = j + UINTSIZE*k;
         	// Entra al if si el bit actual es 1
         	if(tp&mask) {
	
	//for ( i = 0; i < uCantidadPiezas; i++ ){
		
				//Obtiene número entero que identifica a la pieza seleccionada
				usPiezaParaEncajar = critter->pusListaPiezas[bitpos];
				//En ppzaPiezaParaEncajar asigna la pieza que resulta de desplazarse en usPiezaParaEncajar desde ppzaPiezas
				ppzaPiezaParaEncajar = ppzaPiezas + usPiezaParaEncajar;
				//Suma el Area de la pieza seleccionada
				dAreaPiezasEncajadas = dAreaPiezasEncajadas + ppzaPiezaParaEncajar->dArea;
				if ( i == 0 ){ //Encaje de la primera pieza
					//Para la primera pieza y por cada Orientacion obtiene el perfil y calcula el área del perfil y el ancho máx que ocupan las piezas
					for ( Orientacion = 0; Orientacion <= uMaximoRotaciones; Orientacion++ ){
						pptoPuntosPiezaParaEncajar = ppzaPiezaParaEncajar->pptoPuntos[Orientacion];
						dXMax[Orientacion] = app_EncajePrimeraPieza_irb(Orientacion);
						dValorObjetivo[Orientacion] = app_funceval_irb( Orientacion, dXMax[ Orientacion ], dAreaPiezasEncajadas );
					}//End for
					//Determina qué Orientacion de pieza tiene menor dValorObjetivo
					for ( Orientacion = 0; Orientacion <= uMaximoRotaciones; Orientacion++ ){
						if ( dValorObjetivo[Orientacion] <= dValorObjetivo[0] &&
							 dValorObjetivo[Orientacion] <= dValorObjetivo[1] &&
							 ( uMaximoRotaciones == 1 || ( dValorObjetivo[Orientacion] <= dValorObjetivo[2] && 
							 ( uMaximoRotaciones == 2 || dValorObjetivo[Orientacion] <= dValorObjetivo[3] ) ) ) ){				
							ppzaPiezaParaEncajar->iOrientacion = Orientacion;
							break;
						}//End if 
					}//End for
					//Para la Orientacion con función objetivo mínima se actualizan todos los perfiles con los mismos datos del
					//Perfil donde la Orientacion tuvo función objetivo mínima
					for ( Orientacion = 0; Orientacion <= uMaximoRotaciones; Orientacion++ ){
						dXMax[ Orientacion ] = dXMax[ ppzaPiezaParaEncajar->iOrientacion ];
						dMaximoX = dXMax[ ppzaPiezaParaEncajar->iOrientacion ];
						memcpy( pPerfil[ Orientacion ], pPerfil[ ppzaPiezaParaEncajar->iOrientacion], ( iFinPerfil[ ppzaPiezaParaEncajar->iOrientacion] + 1 ) * sizeof( pPerfil[ ppzaPiezaParaEncajar->iOrientacion ] ) );
						iFinPerfil[ Orientacion ] = iFinPerfil[ ppzaPiezaParaEncajar->iOrientacion ];
						iPuntoMasIzquierdaAbajo[Orientacion] = iPuntoMasIzquierdaAbajo[ppzaPiezaParaEncajar->iOrientacion];
					}//End for
				} else{ //Resto de las piezas
					//Va a probar de poner la pieza en todas las Orientaciones 
					for ( Orientacion = 0; Orientacion <= uMaximoRotaciones; Orientacion++ ){
						//Obtiene los puntos dado una Orientacion
						pptoPuntosPiezaParaEncajar = ppzaPiezaParaEncajar->pptoPuntos[Orientacion];
						//Determina si es posible encajar la pieza de acuerdo a su altura en el último punto (el más arriba) del perfil
						if ( dAltoLamina - pPerfil[Orientacion][ iFinPerfil[Orientacion] ]->dYAbs >= ppzaPiezaParaEncajar->Datos[Orientacion].dAltura ){
							//Es posible encajar la pieza sobre el perfil
							// Encaje en banda : la pieza será colocada arriba del perfil : Se mueve la pieza hasta el punto más arriba del Perfil
							app_EncajeEnBanda_irb(Orientacion);
						}else {
							//No es posible encajar la pieza encima del perfil
							//Determinar posicion inicial
							app_DeterminaPosicionInicial_irb(Orientacion,dXMax[Orientacion]);
							//Busqueda de la distancia a desplazar la pieza hacia el encaje
							app_BuscaDistanciaDesplazamiento_irb(Orientacion);
							//Desplazar la pieza nueva en la distancia hallada
							app_DesplazaPieza_irb();
						}//End else
						/*Actualiza perfil*/
						dXMax[Orientacion] = app_ActualizaPerfil_irb(Orientacion);
						dValorObjetivo[Orientacion] = app_funceval_irb(Orientacion,dXMax[Orientacion], dAreaPiezasEncajadas);
					}//End for
					//Determina qué Orientacion de pieza tiene menor dValorObjetivo
					for ( Orientacion = 0; Orientacion <= uMaximoRotaciones; Orientacion++ ){
						if ( dValorObjetivo[Orientacion] <= dValorObjetivo[0] &&
							 dValorObjetivo[Orientacion] <= dValorObjetivo[1] &&
							 ( uMaximoRotaciones == 1 || ( dValorObjetivo[Orientacion] <= dValorObjetivo[2] &&
							 ( uMaximoRotaciones == 2 || dValorObjetivo[Orientacion] <= dValorObjetivo[3] ) ) ) ){
							ppzaPiezaParaEncajar->iOrientacion = Orientacion;
							break;
						}//End if
					}//End for
					//Para la Orientacion con función objetivo mínima se actualizan todos los perfiles con los mismos datos del
					//Perfil donde la Orientacion tuvo función objetivo mínima
					for ( Orientacion = 0; Orientacion <= uMaximoRotaciones; Orientacion++ ){
						dXMax[ Orientacion ] = dXMax[ ppzaPiezaParaEncajar->iOrientacion ];
						dMaximoX = dXMax[ ppzaPiezaParaEncajar->iOrientacion ];
						memcpy( pPerfil[ Orientacion ], pPerfil[ ppzaPiezaParaEncajar->iOrientacion], ( iFinPerfil[ ppzaPiezaParaEncajar->iOrientacion] + 1 ) * sizeof( pPerfil[ ppzaPiezaParaEncajar->iOrientacion ] ) );
						iFinPerfil[ Orientacion ] = iFinPerfil[ ppzaPiezaParaEncajar->iOrientacion ];
						iPuntoMasIzquierdaAbajo[Orientacion] = iPuntoMasIzquierdaAbajo[ppzaPiezaParaEncajar->iOrientacion];
					}//End for
				}//End else
				
				i++; //incrementa contador de piezas consideradas
				
				//Verifica que dMaximoX no sobrepase el ancho de la lámina
				if (dMaximoX >= dAnchoLamina){
					break;
				}//End if
			}//End if
         	tp = tp>>1;
      	}//End for
 
		//Verifica que dMaximoX no sobrepase el ancho de la lámina
		if (dMaximoX >= dAnchoLamina){
			break;
		}//End if

  	}//End for 
		
		
	//}//End for
	
    critter->fitness = app_funceval_irb(0,dMaximoX, dAreaPiezasEncajadas);
    critter->PEval.perdida = (float) critter->fitness;
    critter->PEval.piezas = i; //Indica la cantida de piezas colocadas
    
  	//if ( critter->fitness < bestfit.fitness ){
	//	dgXMayor = dMaximoX;
	//	//dgAreaPerfil = 1.0 / critter->fitness + dAreaTotalPiezas - ( dMaximoX * dAltoLamina + 1.0 );
	//	dgAreaPerfil = critter->fitness + dAreaTotalPiezas - ( dMaximoX * dAltoLamina + 1.0 );
  	//}//End if
}//End app_objfunc_irb

void app_genera_resultados_problema_irb(int corrida, int tipo_problema, char *nombrearchivo)
//Función que imprime los resultados del problema corte de pieza irregular
{
}//End app_genera_resultados_problema_irb

