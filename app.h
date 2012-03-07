/*----------------------------------------------------------------------------*/
/* app.h - rutinas relacionadas con el problema				                  */
/*----------------------------------------------------------------------------*/

#ifndef _APP_H_
#define _APP_H_

int     app_leearchivo(int tipo_problema, char *nombrearchivo);
void   	app_objfunc(int tipo_problema, struct individual *critter);
void 	app_free(int tipo_problema);
void 	app_genera_resultados_problema(int corrida, int tipo_problema, char *nombrearchivo);
 
#endif
