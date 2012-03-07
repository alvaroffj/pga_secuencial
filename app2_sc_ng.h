/*---------------------------------------------------------------------------------------------------*/
/* app2_sc_ng.h - rutinas que generan archivo con layout para Stock Cutting NO Guillotina            */
/*---------------------------------------------------------------------------------------------------*/

#ifndef APP2_SC_NG_H_
#define APP2_SC_NG_H_

void     app2_agregalistap_sc_ng(int x, int y, int anc, int alt, char tipo, int t_pieza);
void     app2_funceval_sc_ng(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion, TNodoAP *piezas);
void     app2_objfunc_sc_ng(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion, struct bestever *bestcritter);
void 	 app2_genera_layout_sc_ng(int tipo_problema, char *nombrearchivo, cpu_consummed_time_t *t, TListaPE2 *Bus2);

#endif /*APP2_SC_NG_H_*/
