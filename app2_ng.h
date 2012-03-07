/*---------------------------------------------------------------------------------------------------*/
/* app2_ng.h - rutinas que generan archivo con layout para Problema de Corte de Piezas NO Guillotina */
/*---------------------------------------------------------------------------------------------------*/

#ifndef _APP2_NG_H_
#define _APP2_NG_H_

void     app2_agregalistap_ng(int x, int y, int anc, int alt, char tipo, int t_pieza);
void     app2_funceval_ng(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion, TNodoAP *piezas);
void     app2_objfunc_ng(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion, struct bestever *bestcritter);
void 	 app2_genera_layout_ng(int tipo_problema, char *nombrearchivo, cpu_consummed_time_t *t, TListaPE2 *Bus2);

#endif
