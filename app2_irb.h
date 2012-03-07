/*---------------------------------------------------------------------------------------------------*/
/* app2_irb.h - rutinas que generan archivo con layout para Problema de Corte de Piezas Irregulares  */
/* 			    manejado con cromosoma binario 														 */
/*---------------------------------------------------------------------------------------------------*/

#ifndef APP2_IRB_H_
#define APP2_IRB_H_

void     app2_agregalistap_irb(int x, int y, int anc, int alt, char tipo, int t_pieza);
void     app2_funceval_irb(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion);
void     app2_objfunc_irb(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion, struct bestever *bestcritter);
void 	 app2_genera_layout_irb(int tipo_problema, char *nombrearchivo, cpu_consummed_time_t *t);

#endif /*APP2_IRB_H_*/
