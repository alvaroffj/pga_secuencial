#ifndef _UTILITY_H_
#define _UTILITY_H_

double 	Second (void);
void   	elapsed_times(struct tms *start, clock_t start_twc,struct tms *end, clock_t end_twc,cpu_consummed_time_t *t);
int    	consistenciaarchivo(void);
void   	datosmalos(char *string);
void 	initialize(void);
void    genera_resultados_algoritmo(int corrida, int tipo_problema, char *nombrearchivo, cpu_consummed_time_t *t);
int 	inicializa_archivos(int argc,char *argv[]);
void 	cierra_archivos(void);

#endif

