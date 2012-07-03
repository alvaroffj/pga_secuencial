/* 
 * File:   var_cp.h
 * Author: Alvaro
 *
 * Created on 23 de junio de 2012, 06:20 PM
 */

#include "var_pcp.h"

int bit_reservados_cp = 4; //Bits usados en el cromosoma para interpretarlo
int *arreglo_ocupado_cp; //arreglo que guarda si la pieza a sido ingresada o no al strip
int *arreglo_orden_cp; //arreglo que guarda el numero de la pieza que es ingresada en la posicion i
int *arreglo_rotar_cp; //arreglo que indica si la pieza i debe ir rotada o no


