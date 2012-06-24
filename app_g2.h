/* 
 * File:   app_cp.h
 * Author: Alvaro
 *
 * Created on 23 de junio de 2012, 06:09 PM
 */

#ifndef APP_CP_H
#define	APP_CP_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "var_cp.h"

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


#ifdef	__cplusplus
}
#endif

#endif	/* APP_CP_H */

