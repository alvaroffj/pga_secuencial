/*-----------------------------------------------------------------------------------------------*/
/* app2_g.h - rutinas que generan archivo con layout para Problema de Corte de Piezas Guillotina */
/*-----------------------------------------------------------------------------------------------*/

#ifndef APP2_G_H_
#define APP2_G_H_

#include "var_pcp.h"

void     app2_liberamemlistaperdidas_g(void);
void     app2_agregalistap_g(int x, int y, int anc, int alt, char tipo, int t_pieza);
void     app2_funceval_g(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion, TNodoAP *piezas);
void     app2_objfunc_g(int tipo_problema, char *nomarch, cpu_consummed_time_t *time_consumtion, struct bestever *bestcritter);
TNodoRE2 app2_pieza2perdida_g(int anc,int alt);
void 	 app2_genera_layout_g(int tipo_problema, char *nombrearchivo, cpu_consummed_time_t *t, TListaPE2 *Bus2);

#endif /*APP2_G_H_*/
