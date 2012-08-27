/*-----------------------------------------------------------------------------------------------*/
/* app2_g.h - rutinas que generan archivo con layout para Problema de Corte de Piezas Guillotina */
/*-----------------------------------------------------------------------------------------------*/

#ifndef APP2_G_H_
#define APP2_G_H_

#include "var_pcp.h"

void app2_liberamemlistaperdidas_g(void);
void app2_agregalistap_g(int x, int y, int anc, int alt, char tipo, int t_pieza);
void app2_genera_layout_g(TListaPE2 *Bus2);
void app2_funceval_g(TNodoAP *piezas, int write);
void app2_objfunc_g(struct individual *critter);
void app2_objfuncfinal_g(struct bestever *critter);
TNodoRE2 app2_pieza2perdida_g(int anc, int alt);
int app2_leearchivo_g(char *nombrearchivo);
void app2_free_g(void);
void app2_genera_resultados_problema_g(int corrida, int tipo_problema, char *nombrearchivo);

#endif /*APP2_G_H_*/
