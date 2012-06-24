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

int bit_reservados_cp = 5; //Bits usados en el cromosoma para interpretarlo
int *arreglo_ocupado_cp; //arreglo que guarda si la pieza a sido ingresada o no al strip
int *arreglo_orden_cp; //arreglo que guarda el numero de la pieza que es ingresada en la posicion i
int *arreglo_rotar_cp; //arreglo que indica si la pieza i debe ir rotada o no

#ifdef	__cplusplus
}
#endif

#endif	/* VAR_CP_H */

