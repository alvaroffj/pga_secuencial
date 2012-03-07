/*----------------------------------------------------------------------------*/
/* app_ng.h - rutinas para Problema de Corte de Piezas NO Guillotina          */
/*----------------------------------------------------------------------------*/

#ifndef _APP_NG_H_
#define _APP_NG_H_

void     app_liberamemlistaperdidas_ng(int perdidaexterna);
void     app_agregalistap_ng(int anc,int alt);
void     app_ordena_piezas_problema_ng(void);
int      app_leearchivo_ng(char *nombrearchivo);
void 	 app_free_ng(void);
TEval    app_funceval_ng(TNodoAP *piezas);
void     app_objfunc_ng(struct individual *critter);
TNodoRE  app_pieza2perdida_ng(int anc,int alt);
int      AreaNodoAPCompara_ng(TNodoAP *Nodoi, TNodoAP *Nodoj);
int      HorizontalNodoAPCompara_ng(TNodoAP *Nodoi, TNodoAP *Nodoj);
int      VerticalNodoAPCompara_ng(TNodoAP *Nodoi, TNodoAP *Nodoj);
void 	 app_genera_resultados_problema_ng(int corrida, int tipo_problema, char *nombrearchivo);

#endif


