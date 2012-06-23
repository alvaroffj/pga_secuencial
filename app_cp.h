/*----------------------------------------------------------------------------*/
/* app_g.h - rutinas para Problema de Corte de Piezas Guillotina              */
/*----------------------------------------------------------------------------*/

#ifndef APP_G_H_
#define APP_G_H_

#include "var_pcp.h"

void     app_liberamemlistaperdidas_cp(int perdidaexterna);
void     app_agregalistap_cp(int anc,int alt);
void     app_ordena_piezas_problema_cp(void);
int      app_leearchivo_cp(char *nombrearchivo);
void 	 app_free_cp(void);
TEval    app_funceval_cp(TNodoAP *piezas);
void     app_objfunc_cp(struct individual *critter);
void     app_objfuncfinal_cp(struct bestever *critter);
TNodoRE  app_pieza2perdida_cp(int anc,int alt);
int      AreaNodoAPCompara_cp(TNodoAP *Nodoi, TNodoAP *Nodoj);
int      HorizontalNodoAPCompara_cp(TNodoAP *Nodoi, TNodoAP *Nodoj);
int      VerticalNodoAPCompara_cp(TNodoAP *Nodoi, TNodoAP *Nodoj);
void 	 app_genera_resultados_problema_cp(int corrida, int tipo_problema, char *nombrearchivo);

#endif /*APP_G_H_*/
