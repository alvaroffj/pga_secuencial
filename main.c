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

/*
-pr 1 -m 0.3 -c 0.6 -g 20 -po 3600 => 16240
-pr 1 -m 0.3 -c 0.9 -g 20 -po 2400 => 16100
-pr 1 -m 0.2 -c 0.9 -g 200 -po 3600 => 15680
*/

int main(int argc, char *argv[]) {
    // Obtiene rutas de trabajo y crea archivos generales
    if (inicializa_archivos(argc, argv) == -1) exit(-1);
// ./paramils -numRun 0 -scenariofile escenario.txt -validN 5
    runmax = consistenciaarchivo();
    int s = 0, genStop = 100, nGen = 0, go = 1;
    float lastBest = 0.0;
    // Revisa consistencia del archivo de entrada (contiene el tipo y problema a resolver + parámetros genéticos 
    for (run = 1; run <= runmax; run++) {
        //Nueva semilla aleatoria
        s = atoi(argv[7]);
        if(s==0) {
            do {
                randomseed = nueva_semilla();
            } while (randomseed == 0);
        } else {
            randomseed = (float)((atoi(argv[7])%10000)/10000.0);
        }
/*
        printf("lala %i\n", atoi(argv[8])%10000);
*/
/*
        printf("randomseed: %f\n", randomseed);
*/

        // Lee archivo en infp con parámetros (todos ya chequeados)
/*
        fscanf(infp, "%d %s %d %d %f %f", &tipo_problema, nomarch, &popsize, &maxgen, &pcross, &pmutation);
*/
        int i=0;
        for(i=0; i<argc; i++) {
            if(strcmp(argv[i], "-pr")==0) tipo_problema = atoi(argv[++i]);
            if(strcmp(argv[i], "-po")==0) popsize = atoi(argv[++i]);
            if(strcmp(argv[i], "-g")==0) genStop = atoi(argv[++i]);
            if(strcmp(argv[i], "-m")==0) pmutation = atof(argv[++i]);
            if(strcmp(argv[i], "-c")==0) pcross = atof(argv[++i]);
            if(strcmp(argv[i], "-f")==0) sprintf(nomarch, "%s", argv[++i]);
        }
/*
        tipo_problema = atoi(argv[3]);
        sprintf(nomarch, "%s", argv[4]);
        popsize = atoi(argv[5]);
        maxgen = atoi(argv[6]);
        pcross = atof(argv[7]);
        pmutation = atof(argv[8]);
*/
                
/*
        printf("Espere, efectuando Corrida %d, Archivo %s...\n", run, nomarch);
        printf("Generaciones: %d\n", maxgen);
        printf("Poblacion: %d\n", popsize);
*/
        if (app_leearchivo(tipo_problema, nomarch)) {
            //Inicializa Contador de Segundos 
            ticks_start = times(&time_start);

            // Rutina de inicialización de variables globales
            initialize();

            // Initializa la población de individuos 
            // y mejor individuo de la población
            initpop(tipo_problema, run);
            gen = 0;
            nGen = 0;
            go = 1;
            while(go) {
                // Crea una nueva generación
                generation(tipo_problema, run);
/*
                printf("generacion: %d => %f\n\n",gen, bestfit.fitness);
*/
                // Avanza de Generación
                tempold = oldpop;
                oldpop = newpop;
                newpop = tempold;

                // Condicin para salir del problema => que la prdida sea cero
                if (bestfit.PEval.perdida == 0.0) go = 0;
                if(gen>0) {
                    if(lastBest == bestfit.fitness) {
                        nGen++;
                        if(nGen>=genStop) go = 0;
                    } else {
                        lastBest = bestfit.fitness;
                        nGen = 0;
                    }
                } else lastBest = bestfit.fitness;
                gen++;
            }

            //Finaliza Contador de Segundos y calcula tiempos 
            ticks_end = times(&time_end);
            elapsed_times(&time_start, ticks_start, &time_end, ticks_end, &time_consumtion);

            //Graba datos en archivo de resultados del algoritmo
            genera_resultados_algoritmo(run, tipo_problema, nomarch, &time_consumtion);

            //Graba datos en archivo de resultados del problema
            app_genera_resultados_problema(run, tipo_problema, nomarch);

            //Genera la Salida hacia archivo de Layout
            app_objfuncfinal(tipo_problema, ruta_salida, nomarch, &time_consumtion, &(bestfit));
/*
            app_objfunc(tipo_problema, ruta_salida, nomarch, &time_consumtion, &(bestfit));
*/
/*
            printf("RunsExecuted = 1\n");
            printf("CPUTime_Mean = %f\n", time_consumtion.elapsed_time);
            printf("BestSolution_Mean = %f\n", bestfit.fitness);
*/
            printf("Result for ParamILS: SAT, %f, %i, %f, %f\n", -1.0, -1, bestfit.fitness, randomseed);
            // Libera memoria temporal del algoritmo no del problema
            freeall();
        }//End if
        // Libera variables del problema  
        app_free(tipo_problema);
//        Final best parameter configuration: c=0.9, g=20, m=0.3, po=30, pr=1
//        Final best parameter configuration: c=0.9, g=20, m=0.1, po=30, pr=1
/*
        printf("Corrida %d, Archivo %s procesado...   [OK]\n", run, nomarch);
*/
    }//End for
    //Cierra Archivos importantes
    cierra_archivos();

    exit(0);
}//End main

