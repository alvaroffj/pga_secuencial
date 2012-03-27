/************************************************************/
/*     ALGORITMO SECUENCIAL DE UNA MÁQUINA PARALELA QUE     */
/*     RESUELVE PROBLEMAS DE OPTIMIZACIÓN COMBINATORIA	    */
/*     Autor : Fernando Romero Z.              			    */
/*     Trabajo para optar al grado de Doctor en Ciencias    */
/*     de la Ingeniería, mención Ingeniería Informática     */
/************************************************************/

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>

#include "sga.h"
#include "generate.h"
#include "memory.h"
#include "utility.h"
#include "app.h"
#include "random.h"

int main(int argc, char *argv[]) {
    // Obtiene rutas de trabajo y crea archivos generales
    if (inicializa_archivos(argc, argv) == -1) exit(-1);

    runmax = consistenciaarchivo();
    // Revisa consistencia del archivo de entrada (contiene el tipo y problema a resolver + parámetros genéticos 
    for (run = 1; run <= runmax; run++) {
        //Nueva semilla aleatoria
        do {
            randomseed = nueva_semilla();
        } while (randomseed == 0);

        // Lee archivo en infp con parámetros (todos ya chequeados)
        fscanf(infp, "%d %s %d %d %f %f", &tipo_problema, nomarch, &popsize, &maxgen, &pcross, &pmutation);
/*
        tipo_problema = atoi(argv[3]);
        sprintf(nomarch, "%s", argv[4]);
        popsize = atoi(argv[5]);
        maxgen = atoi(argv[6]);
        pcross = atof(argv[7]);
        pmutation = atof(argv[8]);
*/
                
        printf("Espere, efectuando Corrida %d, Archivo %s...\n", run, nomarch);
        printf("Generaciones: %d\n", maxgen);
        printf("Poblacion: %d\n", popsize);
        if (app_leearchivo(tipo_problema, nomarch)) {
            //Inicializa Contador de Segundos 
            ticks_start = times(&time_start);

            // Rutina de inicialización de variables globales
            initialize();

            // Initializa la población de individuos 
            // y mejor individuo de la población
            initpop(tipo_problema, run);

            for (gen = 0; gen < maxgen; gen++) {
                // Crea una nueva generación
                generation(tipo_problema, run);
                printf("generacion: %d => %f\n",gen, bestfit.fitness);
                // Avanza de Generación
                tempold = oldpop;
                oldpop = newpop;
                newpop = tempold;

                // Condicin para salir del problema => que la prdida sea cero
                if (bestfit.PEval.perdida == 0.0) gen = maxgen;
            }//End for

            //Finaliza Contador de Segundos y calcula tiempos 
            ticks_end = times(&time_end);
            elapsed_times(&time_start, ticks_start, &time_end, ticks_end, &time_consumtion);

            //Graba datos en archivo de resultados del algoritmo
            genera_resultados_algoritmo(run, tipo_problema, nomarch, &time_consumtion);

            //Graba datos en archivo de resultados del problema
            app_genera_resultados_problema(run, tipo_problema, nomarch);

            //Genera la Salida hacia archivo de Layout
            app2_objfunc(tipo_problema, ruta_salida, nomarch, &time_consumtion, &(bestfit));
/*
            app_objfunc(tipo_problema, ruta_salida, nomarch, &time_consumtion, &(bestfit));
*/

            // Libera memoria temporal del algoritmo no del problema
            freeall();
        }//End if
        // Libera variables del problema  
        app_free(tipo_problema);

        printf("Corrida %d, Archivo %s procesado...   [OK]\n", run, nomarch);
    }//End for

    //Cierra Archivos importantes
    cierra_archivos();

    exit(0);
}//End main

