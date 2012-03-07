/*--------------------------------------------------------------------------------*/
/* app2.c - rutina función objetivo del problema (genera datos finales de salida) */
/*--------------------------------------------------------------------------------*/

#include <stdlib.h>

#include "sga.h"
#include "app2_g.h"
#include "app2_ng.h"
#include "app2_ir.h"
#include "app2_irb.h"
#include "app2_sc_ng.h"

void app2_objfunc(int tipo_problema, char *archivodestino, char *nomarch, cpu_consummed_time_t *time_consumtion, struct bestever *bestcritter)
// Función Objetivo General
{
	if(tipo_problema == 0) 
		//Problema de Corte de Pieza Guillotina
		app2_objfunc_g(tipo_problema, nomarch, time_consumtion, bestcritter);
	else if(tipo_problema == 1)
		//Problema de Corte de Pieza No Guillotina
		app2_objfunc_ng(tipo_problema, nomarch, time_consumtion, bestcritter);
	else if(tipo_problema == 2)
		//Problema de Corte de Pieza Irregulares con cromosoma con ENTEROS
		app2_objfunc_ir(tipo_problema, nomarch, time_consumtion, bestcritter);
	else if(tipo_problema == 3)
		//Problema de Corte de Pieza Irregulares con cromosoma binario
		app2_objfunc_irb(tipo_problema, nomarch, time_consumtion, bestcritter);
	else if(tipo_problema == 4)
		//Problema Stock Cutting No Guillotina
		app2_objfunc_sc_ng(tipo_problema, nomarch, time_consumtion, bestcritter);
}//End app2_objfunc



