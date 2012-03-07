/*----------------------------------------------------------------------------*/
/* app_sc_ng.h - rutinas para Problema Stock Cutting NO Guillotina            */
/*----------------------------------------------------------------------------*/

#ifndef APP_SC_NG_H_
#define APP_SC_NG_H_

void     app_liberamemlistaperdidas_sc_ng(int perdidaexterna);
void     app_agregalistap_sc_ng(int anc,int alt);
void     app_ordena_piezas_problema_sc_ng(void);
int      app_leearchivo_sc_ng(char *nombrearchivo);
void 	 app_free_sc_ng(void);
TEval    app_funceval_sc_ng(TNodoAP *piezas);
void     app_objfunc_sc_ng(struct individual *critter);
TNodoRE  app_pieza2perdida_sc_ng(int anc,int alt);
int      AreaNodoAPCompara_sc_ng(TNodoAP *Nodoi, TNodoAP *Nodoj);
int      HorizontalNodoAPCompara_sc_ng(TNodoAP *Nodoi, TNodoAP *Nodoj);
int      VerticalNodoAPCompara_sc_ng(TNodoAP *Nodoi, TNodoAP *Nodoj);
void 	 app_genera_resultados_problema_sc_ng(int corrida, int tipo_problema, char *nombrearchivo);

#endif /*APP_SC_NG_H_*/
