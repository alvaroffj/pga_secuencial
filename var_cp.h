/* 
 * File:   var_cp.h
 * Author: Alvaro
 *
 * Created on 23 de junio de 2012, 06:20 PM
 */

#ifndef VAR_CP_H
#define	VAR_CP_H

#ifdef	__cplusplus
extern "C" {
#endif

int 			NumPie; 			/* Número total de piezas */
int 			cantidadtipospiezas;/* Variable que almacena la cantidad de */
									/* de tipos de piezas del problemas		*/
int         	AltoPl,AnchoPl; 	/* Dimensiones de la placa */
int				TotalPie; 			/* Número total de piezas => Piezas Ganancias + Piezas Pérdidas */
float 			peso_func_obj;  	/* Uso en funcion evaluación - Factor de la pérdida */
float 			peso_uni;			/* Uso en funcin evaluación - Factor unificación de pérdidas */
float 			peso_perdida;		/* Factor de la componente pérdida */
float 			peso_distancia;		/* Factor de la componente distancia */
float 			peso_digregacion;	/* Factor de la componente digregación */
int bit_reservados_cp = 5; //Bits usados en el cromosoma para interpretarlo
int *arreglo_ocupado_cp; //arreglo que guarda si la pieza a sido ingresada o no al strip
int *arreglo_orden_cp; //arreglo que guarda el numero de la pieza que es ingresada en la posicion i
int *arreglo_rotar_cp; //arreglo que indica si la pieza i debe ir rotada o no

/* Estructuras de la aplicación */

/*Estructura para almacenar una pieza, en un arreglo*/
typedef struct nodoap
{
   	int ancho;
   	int alto;
   	int numero;
	int cantidadpiezas;
}TNodoAP;

/*Estructura para retornar una pieza de la lista de prdidas*/
typedef struct nodore
{
   int ancho;
   int alto;
}TNodoRE;

/*Estructura para la lista donde se almacenan las prdidas*/
typedef struct nodope
{
   int ancho;
   int alto;
   struct nodope *prox;
}TListaPE;

/*Estructura para retornar una pieza de la lista de piezas (Prdida o Ganancia)*/
typedef struct nodore2
{
   	int  ancho;
   	int  alto;
   	int  xini;
   	int  yini;
   	char tipo;  //P=Prdida o G=Ganacia
   	int  pieza; //Nmero de pieza utilizada
}TNodoRE2;

/*Estructura para la lista donde se almacenan las piezas (Prdida o Ganancia)*/
typedef struct nodope2
{
   	int  ancho;
  	int  alto;
  	int  xini;
   	int  yini;
   	char tipo;  //P=Pérdida o G=Ganacia
   	int  pieza; //Nmero de pieza utilizada
   	struct nodope2 *prox;
}TListaPE2;

/*Variables globales*/
TListaPE2  		*LPer2; /*Lista de Pérdidas o Ganancias*/
TListaPE    	*LPer; /*Lista de prdidas*/
TNodoAP  		*piezasdistintas;    /* variable que apunta a las piezas de la lmina */
TNodoAP     	*piezasproblema;   /* variable que apunta a todas piezas del problema */


#ifdef	__cplusplus
}
#endif

#endif	/* VAR_CP_H */

