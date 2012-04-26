/*----------------------------------------------------------------------------*/
/* utility.c - Rutinas de Utilidad 																						*/
/*----------------------------------------------------------------------------*/

#include <sys/times.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include "sga.h"
#include "memory.h" 
#include "random.h"

double Second(void)
// Rutina que establece los segundos transcurridos
{
    struct tms use;
    clock_t realtime;
    double t;
    long tps = sysconf(_SC_CLK_TCK);

    realtime = times(&use);
    t = (double) realtime / tps;
    return t;
}//End Second

void elapsed_times(struct tms *start, clock_t start_twc, struct tms *end, clock_t end_twc, cpu_consummed_time_t *t)
// Calcula el tiempo transcurrido entre dos instantes definidos por start y end.
{
    /*
    Descripcion:
============
    Calcula el tiempo transcurrido entre dos instantes definidos por
    start y end.
		
            Las varibles start y end son estructutas del tipo tms,
    llenadas por la llamada al sistema times(&start) y times(&end).
            La definicion de la estructura tms es la siguinte:
	
                                            struct tms {
                                                            clock_t tms_utime;		//User CPU time
                                                            clock_t	tms_stime;		//System CPU time
                                                            clock_t tms_cutime;		//User CPU time of children
                                                            clock_t	tms_cstime;		//System CPU time
                                    };
		
            Todos los valores calculados estan explesados en segundos,
            son los siguintes:
		
            1.- elapsed_time: Tiempo Total Transcurrido. Tiempo transcurrido entre el instante
                            que se hizo la llamada start =times(&start) y el instante que se hizo
                            la llamada end = times(&end). Este intervalo es medido a traves del
                            Wall Clock del sistema.
            2.- parent_user : Fraccion de Tiempo de CPU Consumido por el Proceso Usuario Padre.
            3.- parent_sys: Frac. de Tpo de CPU Cons. por el Systema en la Adm.del Proc. Padre.
            4.- child_user: Fraccion de Tpo de CPU Consumido por el Proceso Usuario Hijo.
            5.- child_sys: Frac. de Tpo de CPU Con. por el Systema en la Adm. del Proc. Hijo
            6.- user: Fraccion de Tiempo de CPU Consumido por el Proceso Usuario
            7.- sys: Frac. de Tpo de CPU Cons. por el Sistema en la Admin. del Proceso
            8.- cpu_time: Frac.de Tiempode de CPU Cons. en la ejec. del frag. del Proceso
            9.- io_time: Fraccion de Tiempo consumido por el Input/Output del sistema
		
            Salida
            ======
     */
    long tps = sysconf(_SC_CLK_TCK);


    /*1.- Tiempo Total Transcurrido*/
    t->elapsed_time = (float) (end_twc - start_twc) / tps;

    /*2.- Fraccion de Tiempo de CPU Consumido por el Proceso Usuario Padre.*/
    t->parent_user = (float) (end->tms_utime - start->tms_utime) / tps;

    /*3.- Frac. de Tpo de CPU Cons. por el Systema en la Adm.del Proc. Padre.*/
    t->parent_sys = (float) (end->tms_stime - start->tms_stime) / tps;

    /*4.- Fraccion de Tpo de CPU Consumido por el Proceso Usuario Hijo.*/
    t->child_user = (float) (end->tms_cutime - start->tms_cutime) / tps;

    /*5.- Frac. de Tpo de CPU Con. por el Systema en la Adm. del Proc. Hijo*/
    t->child_sys = (float) (end->tms_cstime - start->tms_cstime) / tps;

    /*6.- Fraccion de Tiempo de CPU Consumido por el Proceso Usuario*/
    t->user = t->parent_user + t->child_user;

    /*7.- Frac. de Tpo de CPU Cons. por el Sistema en la Admin. del Proceso*/
    t->sys = t->parent_sys + t->child_sys;

    /*8.- Frac.de Tiempode de CPU Cons. en la ejec. del frag. del Proceso*/
    t->cpu_time = t->user + t->sys;

    /*9.- Fraccion de Tiempo consumido por el Input/Output del sistema*/
    t->io_time = t->elapsed_time - t->cpu_time;
}//End elapsed_times

void datosmalos(char *string)
// Imprime en outfp cuando existen datos errneos
{
    fprintf(outfp, "Error en datos de entrada : %s!!\n", string);
}//End datosmalos

int consistenciaarchivo(void)
// Rutina que chequea la consistencia del archivo de parametros de entrada
{
    FILE *fpp;
    int cont = 0;
    char nombre_archivo[100], archivo_malo[100];

    while (fscanf(infp, "%d %s %d %d %f %f", &tipo_problema, nomarch, &popsize, &maxgen, &pcross, &pmutation) != EOF) {
        if (tipo_problema != 0 && tipo_problema != 1 && tipo_problema != 2 && tipo_problema != 3) {
            datosmalos("Tipo de Problema");
            return 0;
        }
        sprintf(nombre_archivo, "%s%s", ruta_instancias, nomarch);
        if ((fpp = fopen(nombre_archivo, "r")) == NULL) {
            sprintf(archivo_malo, "Archivo a resolver : %s", nombre_archivo);
            datosmalos(archivo_malo);
            return 0;
        } else
            fclose(fpp);
        if ((popsize <= 0) || ((popsize % 2) != 0)) {
            datosmalos("Tamaño de la Población");
            return 0;
        }//End if
        if (maxgen < 1) {
            datosmalos("Número máximo de generaciones");
            return 0;
        }//End if
        if ((pcross < 0.0) || (pcross > 1.0)) {
            datosmalos("Probabilidad cruzamiento");
            return 0;
        }//End if
        if ((pmutation < 0.0) || (pmutation > 1.0)) {
            datosmalos("Probabilidad mutación");
            return 0;
        }//End if
        cont++;
    }//End while
    rewind(infp);
    return cont;
}//End consistenciaarchivo

void initialize(void)
// Inicializacn de variables
{
    // Se considera relacion uno a uno => El largo de los cromosomas = largo_cromosoma
    bitsxcodigobinario = 1;

    // Definición del largo del cromosoma...
    lchrom = largo_cromosoma * bitsxcodigobinario;
    lchmut = largo_cromosoma;

    // Define el tamaño del cromosoma string en términos de los bytes de la máquina,
    // esto es, el largo del cromosoma en bits (lchrom) / (bits por bytes)
    // chromsize debe ser conocido en malloc() para puntero chrom
    chromsize = (lchrom / UINTSIZE);
    if (lchrom % UINTSIZE) chromsize++;

    // Define tamaño del cromosoma de mutación en términos de bytes de máquina
    chmutsize = (lchmut / UINTSIZE);
    if (lchmut % UINTSIZE) chmutsize++;

    // Define tamaño del cromosoma de lista
    chlistasize = largo_cromosoma;

    // Ubica espacio malloc para las estructuras de datos globales
    initmalloc();

    // Inicializa generador de nmeros aleatorios
    randomize();

    // Inicializa contadores / variables globales
    nmutation = 0;
    ncross = 0;
    mmutation = 0;
    mcross = 0;
    bestfit.fitness = fitness_inicial;
    bestfit.generation = 0;
    gen = 0;

    //Coloca encabezado en archivo de estadísticas
    fprintf(evofp, "\nCorrida;Generación;Mínimo;Máximo;Media;Varianza\n");
}//End initialize

void genera_resultados_algoritmo(int corrida, int tipo_problema, char *nombrearchivo, cpu_consummed_time_t *t)
//Función que imprime los resultados del algoritmo
{
/*
    printf("genera_resultados_algoritmo\n");
*/
    if (encabezado_resultado_algoritmo == 0) {
        //IMPRIME RESULTADOS DEL ALGORIMO GENÉTICO
        fprintf(ralgfp, "CORRIDA;TIPO_PROBLEMA;ARCHIVO_PROBLEMA;TIEMPO_CPU;TIEMPO_IO;TIEMPO_TOTAL;");
        fprintf(ralgfp, "MAXIMO_GENERACIONES;TAMANO_POBLACION;PROBABILIDAD_CRUZAMIENTO;");
        fprintf(ralgfp, "PROBABILIDAD_MUTACION;SEMILLA\n");
        encabezado_resultado_algoritmo = 1; //Con ésto no imprime más el encabezado
    }//End if
    //IMPRIME RESULTADOS DEL ALGORIMO GENÉTICO
    fprintf(ralgfp, "%d;%d;%s;%f;%f;%f;%d;%d;%f;%f;%f\n",
            //CORRIDA;TIPO_PROBLEMA;ARCHIVO_PROBLEMA;TIEMPO_CPU;TIEMPO_IO;TIEMPO_TOTAL;MAXIMO_GENERACIONES;      			
            corrida, tipo_problema, nombrearchivo, t->cpu_time, t->io_time, t->elapsed_time, maxgen,
            //TAMANO_POBLACION;PROBABILIDAD_CRUZAMIENTO;PROBABILIDAD_MUTACION;SEMILLA;"
            popsize, pcross, pmutation, randomseed);
}//End genera_resultados_algoritmo

int inicializa_archivos(int argc, char *argv[])
//Inicializa archivos globales
{
    //Asigna ruta de instancias y resultados
    sprintf(ruta_instancias, "%s", "instancias/"); //Ruta donde se encuentras las instancias a resolver
    sprintf(ruta_resultados, "%s", "resultados/"); //Ruta donde se dejarán los archivos de resultados

    // Determina input y output desde argumentos de la funcin main()
    numfiles = argc - 1;
/*
    printf("numfiles: %d\n", numfiles);
*/
    switch (numfiles) {
        case 2:
            if ((infp = fopen(argv[1], "r")) == NULL) {
                fprintf(stderr, "!!! Error, no puede abrir archivo de entrada %s \n", argv[1]);
                return -1;
            }
            sprintf(ruta_salida, "%s%s", ruta_resultados, argv[2]); //Ruta del archivo de salida (layout)
            if ((outfp = fopen(ruta_salida, "w")) == NULL) {
                fprintf(stderr, "!!! Error, no puede abrir archivo de salida %s \n", ruta_salida);
                return -1;
            }
            break;
        case 8:
            if ((infp = fopen(argv[1], "r")) == NULL) {
                fprintf(stderr, "!!! Error, no puede abrir archivo de entrada %s \n", argv[1]);
                return -1;
            }
            sprintf(ruta_salida, "%s%s", ruta_resultados, argv[2]); //Ruta del archivo de salida (layout)
            if ((outfp = fopen(ruta_salida, "w")) == NULL) {
                fprintf(stderr, "!!! Error, no puede abrir archivo de salida %s \n", ruta_salida);
                return -1;
            }
            break;
        default:
            if ((infp = fopen(argv[1], "r")) == NULL) {
                fprintf(stderr, "!!! Error, no puede abrir archivo de entrada %s \n", argv[1]);
                return -1;
            }
            sprintf(ruta_salida, "%s%s", ruta_resultados, argv[2]); //Ruta del archivo de salida (layout)
            if ((outfp = fopen(ruta_salida, "w")) == NULL) {
                fprintf(stderr, "!!! Error, no puede abrir archivo de salida %s \n", ruta_salida);
                return -1;
            }
/*
            fprintf(stderr, "Uso: ./ejecutable [input file] [output file]\n");
*/
            break;
    }//End switch

    sprintf(arch_reporte_pro, "%s%s", ruta_resultados, "reporte_pro.txt"); //Archivo de reporte de variables importantes por corrida, relacionada por cada problema a resolver 
    //Crea un nuevo archivo de reporte de variables importantes de cada corrida de los problemas a resolver
    if ((rprofp = fopen(arch_reporte_pro, "w")) == NULL) {
        fprintf(stderr, "\nNo se pudo crear archivo %s\n", arch_reporte_pro);
        return -1;
    }//End if

    sprintf(arch_reporte_alg, "%s%s", ruta_resultados, "reporte_alg.txt"); //Archivo de reporte de variables importantes por corrida, relacionada con el algoritmo genético a resolver 
    //Crea un nuevo archivo de reporte de variables importantes del Algorito Genético
    if ((ralgfp = fopen(arch_reporte_alg, "w")) == NULL) {
        fprintf(stderr, "\nNo se pudo crear archivo %s\n", arch_reporte_alg);
        return -1;
    }//End if

    sprintf(arch_evolucion, "%s%s", ruta_resultados, "evolucion_alg.txt"); //Archivo con evolución del Algoritmo Genético por cada corrida 
    //Crea un nuevo archivo de evolución
    if ((evofp = fopen(arch_evolucion, "w")) == NULL) {
        fprintf(stderr, "\nNo se pudo crear archivo %s\n", arch_evolucion);
        return -1;
    }//End if

    //Inicializa semilla aleatoria
    inicializa_semilla();

    //Inicializa variables que determinan si encabezados de archivos de resultados están impresas o no
    encabezado_resultado_algoritmo = 0;
    encabezado_resultado_problema = 0;

    return 0;
}//End inicializa_archivos

void cierra_archivos(void)
//Cierra Archivos importantes
{
    fclose(infp); //Cierra archivo de entrada
    fclose(outfp); //Cierra archivo de salida
    fclose(evofp); //Cierra archivo de evolución del algoritmo genético
    fclose(rprofp); //Cierra archivo de reporte de variables importantes de las corridas de cada problema
    fclose(ralgfp); //Cierra archivo de reporte de variables importantes del Agloritmo Genétrico 
}//End cierra_archivos

