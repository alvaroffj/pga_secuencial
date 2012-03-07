/*------------------------------------------------------------------*/
/* app_irb.h - rutinas para Problema de Corte de Piezas Irregulares */
/*   		   manejado con cromosoma binario               		*/
/*------------------------------------------------------------------*/

#ifndef APP_IRB_H_
#define APP_IRB_H_

void     app_ordena_piezas_problema_irb(void);
int      app_leearchivo_irb(char *nombrearchivo);
void 	 app_free_irb(void);
double   app_EncajePrimeraPieza_irb(unsigned Orientacion);
void 	 app_EncajeEnBanda_irb(unsigned Orientacion);
void 	 app_DeterminaPosicionInicial_irb(unsigned Orientacion, double dMax);
double 	 app_dObtieneDistanciaPuntoLado_irb( double xP, double yP,	double xA, double yA, double xB, double yB );
void 	 app_BuscaDistanciaDesplazamiento_irb(unsigned Orientacion);
void 	 app_DesplazaPieza_irb(void);
double 	 app_funceval_irb(unsigned Orientacion, double dMaximoX, double dAreaPiezasEncajadas);
void     app_objfunc_irb(struct individual *critter);
void 	 app_genera_resultados_problema_irb(int corrida, int tipo_problema, char *nombrearchivo);

#endif /*APP_IRB_H_*/
