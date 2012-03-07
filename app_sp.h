/*----------------------------------------------------------------------------*/
/* app_g.h - rutinas para Problema Strip Packing                              */
/*----------------------------------------------------------------------------*/

#ifndef APP_SP_H
#define	APP_SP_H

#include "var_pcp.h"

int      app_leearchivo_sp(char *nombrearchivo);
void 	 app_free_sp(void);
void     app_objfunc_sp(struct individual *critter);
void 	 app_genera_resultados_problema_sp(int corrida, int tipo_problema, char *nombrearchivo);
void     creaLayout(int *orden, int *rotar);

#endif	/* APP_SP_H */
