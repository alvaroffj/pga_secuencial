/*----------------------------------------------------------------------------*/
/* app_sp.c - rutinas dependientes de la aplicación                           */
/*----------------------------------------------------------------------------*/
/*                       PROBLEMA STRIP PACKING                               */
/*----------------------------------------------------------------------------*/
/* Desarrollado por: Alvaro Flores                                            */
/*----------------------------------------------------------------------------*/


#include <stdlib.h>

#include "memory.h"
#include "sga.h"
#include "var_sp.h"

/*
 * Funciones Auxiliares
 */

int mayorAltura() {
    int pos=0, i, mayor = arreglo_alturas[0];
    
    for(i=1; i<ancho; i++) {
        if(arreglo_alturas[i]>mayor) {
            mayor = arreglo_alturas[i];
            pos = i;
        }
    }
    
    return mayor;
}

int posMenorAltura() {
    int pos=0, i, menor = arreglo_alturas[0];
    
    for(i=1; i<ancho; i++) {
        if(arreglo_alturas[i]<menor) {
            menor = arreglo_alturas[i];
            pos = i;
        }
    }
    
    return pos;
}

int posNMenorAltura(int n) {
    int menor[n], pos[n], i, j, k, mayor, distinto = 1;
    
    for(i=0; i<n; i++) {
        if(i==0) {
            menor[i] = arreglo_alturas[0];
            mayor = arreglo_alturas[0];
            pos[i] = 0;
            for(j=1; j<ancho; j++) {
                if(arreglo_alturas[j]<menor[i]) {
                    menor[i] = arreglo_alturas[j];
                    pos[i] = j;
                } else if(arreglo_alturas[j] > mayor){
                    mayor = arreglo_alturas[j];
                }
            }
        } else {
            distinto = 1;
            menor[i] = mayor+1;
            for(j=0; j<ancho; j++) {
                for(k=0; k<i; k++) {
                    if(pos[k] == j)
                        distinto = 0;
                    else
                        distinto = 1*distinto;
                }
                if(arreglo_alturas[j]<menor[i] && arreglo_alturas[j]>=menor[i-1] && distinto) {
                    menor[i] = arreglo_alturas[j];
                    pos[i] = j;
                }
            }
        }
    }
    return pos[n-1];
}

float fitness() {
    float fitness = (float)(2*mayorAltura()*ancho)-area_total;
/*
    printf("fitness: %d\n", fitness);
*/
    return(fitness);
}

/*
 * Inserta las piezas en la tira, según el orden y sentido correspondiente
 */
void creaLayout() {
    int i, x = 0, j, altura = 0, n = 1, fit = 0;
    Datos_pieza cPieza;

    for(i=0; i<ancho; i++) {
        arreglo_alturas[i] = 0;
    }
    for(i=0; i<numero_piezas; i++) {
        arreglo_ocupado[i] = 0;
    }
    for(i=0; i<numero_piezas; i++) {
        x = posMenorAltura();
        cPieza = lista_piezas[arreglo_orden[i]];
/*
        printf("rotacion pieza %d: %d\n", arreglo_orden[i], arreglo_rotar[arreglo_orden[i]]);
*/
        n = 1;
        while(!fit) {
            x = posNMenorAltura(n);
/*
            printf("%d x: %d => %d\n", n, x, arreglo_alturas[x]);
*/
            if(cPieza.ancho <= (ancho - x)) fit = 1;
            else n++;
        }
        fit = 0;
        altura = arreglo_alturas[x];
        for(j=x; j<cPieza.ancho+x-1; j++) {
            if(altura < arreglo_alturas[j+1]) {
                altura = arreglo_alturas[j+1];
            }
        }
        
/*
        printf("insertar pieza[%d]: %d x %d en (%d,%d)\n", arreglo_orden[i], cPieza.ancho, cPieza.alto, x, altura);
*/
        for(j=x; j<cPieza.ancho+x; j++) {
            arreglo_alturas[j] = altura + cPieza.alto;
        }
/*
        printf("arreglo_altura: ");
        for(j=0; j<ancho; j++) {
            printf("%d ", arreglo_alturas[j]);
        }
        printf("\n");
*/
    }
/*
    printf("arreglo_altura: ");
    for(j=0; j<ancho; j++) {
        printf("%d ", arreglo_alturas[j]);
    }
    printf("\n");
*/
}


/*
 * Funciones obligatorias
 */

/*
 * Leer archivo de entrada
 */
int app_leearchivo_sp(char *nombrearchivo) {
    FILE *fp;
    char nombre_archivo[100];
    char datos_pieza[500];
    char datos_pieza2[80800];
    char *ptr;
    int j=0;
    printf("Leyendo archivo de entrada\n");

    sprintf(nombre_archivo, "%s%s", ruta_instancias, nombrearchivo);


    if((fp = fopen(nombre_archivo,"r"))== NULL){//Si no se encuentra o hay errores al leer el archivo de entrada
        printf("Error al leer archivo %s\n",nombrearchivo);
        return 0;
    }

    // Lee el numero de piezas y el ancho del strip
    fscanf(fp,"%d %d",&numero_piezas,&ancho);
    printf("Piezas: %d\nAncho: %d\n", numero_piezas, ancho);
    if(numero_piezas != 0) {
        //Se establece la memoria de la lista de piezas
        lista_piezas = (Datos_pieza *) malloc(numero_piezas * sizeof(Datos_pieza)); //La lista de piezas guarda cada estructura de pieza
        arreglo_espacios = (int *) malloc(ancho * sizeof(int)); //en este arreglo se guarda la altura por cada espacio
        arreglo_alturas = (int *) malloc(ancho * sizeof(int)); //en este arreglo se guarda la altura por cada espacio
        arreglo_ocupado = (int *) malloc(numero_piezas * sizeof(int)); //en este arreglo se guarda si la pieza fue ocupada o no
        arreglo_orden = (int *) malloc(numero_piezas * sizeof(int)); //arreglo que guarda el numero de la pieza que es ingresada en la posicion i
        arreglo_rotar = (int *) malloc(numero_piezas * sizeof(int)); //arreglo que indica si la pieza i debe ir rotada o no

        Datos_pieza aux_pieza;
        area_total=0;
        // Lee ancho y alto para cada pieza
        // Ciclo para leer todas las piezas del archivo de entrada
        while( fscanf(fp,"%d %d", &aux_pieza.ancho, &aux_pieza.alto) != EOF) {
            aux_pieza.area = aux_pieza.ancho * aux_pieza.alto; //Se calcula el area de la pieza
            aux_pieza.id = j; //Se asigna id a la pieza
            area_total += aux_pieza.area;
            lista_piezas[j] = aux_pieza; //Se ingresa la pieza
            printf("Pieza %d: %d x %d => %d\n", lista_piezas[j].id, lista_piezas[j].ancho, lista_piezas[j].alto, lista_piezas[j].area);
            j++;
        }
        
        printf("Area Total: %d\n", area_total);
        //Cierra archivo de piezas
        fclose(fp);
        largo_cromosoma = numero_piezas + bit_reservados;
        return -1;
    } else return 0;
}

/*
 * Libera memoria usada en la ejecucion
 */
void app_free_sp(void) {
    free(lista_piezas);
    #ifdef _DEBUG_MALLOC_		
        printf("Free, app_sp.c, 69, lista_piezas\n");
    #endif
}

/*
 * Funcion Objetivo
 */
void app_objfunc_sp(struct individual *critter) {
    unsigned mask = 1, tp, rt, bitpos, salto, dir, b, m, go = 1;
    int i, j, stop, pPend = numero_piezas, pIni = 0, ini, valPos, nAsig = 0, vueltas = 0;
    int chrom[numero_piezas];
    
    for(i=0; i<numero_piezas; i++) {
        arreglo_orden[i] = -1;
        arreglo_ocupado[i] = 0;
        chrom[i] = 0;
        arreglo_rotar[i] = 0;
    }
    
/*
    printf("app_objfunc_sp\n");
    printf("largo_cromosoma: %d\n", largo_cromosoma);
    printf("Cromosoma: ");
*/
    for (i = 0; i < chromsize; i++) {
        if (i == (chromsize - 1)) //ultimo bloque
            stop = lchrom - (i * UINTSIZE);
        else
            stop = UINTSIZE;
        
        tp = critter->chrom[i];
        rt = critter->chmut[i];
        for (j = 0; j < stop; j++) {
            bitpos = j + UINTSIZE*i;
            if((i==0 && j >=bit_reservados) || i>0) {
                /*
                * Extrae segmento del cromosoma correspondiente a las piezas
                */
                if (tp & mask) chrom[bitpos - bit_reservados] = 1;
                else chrom[bitpos - bit_reservados] = 0;

                /*
                * Extrae segmento del cromosoma correspondiente a la orientacion de la pieza
                */
                if (rt & mask) arreglo_rotar[bitpos - bit_reservados] = 1; //Rotada
                else arreglo_rotar[bitpos - bit_reservados] = 0; //No rotada
            }
            tp = tp >> 1;
            rt = rt >> 1;
        }
    }
/*
    printf("\n");
*/
    tp = critter->chrom[0];
    /*
     * ini: Valor que se considera primero, 0 o 1, al recorrer el cromosoma
     */
    ini = (tp & mask)?1:0;
    tp = tp >> 1;
    /*
     * dir: Direccion inicial en que se recorre el cromosoma
     * 0: der -> izq
     * 1: izq -> der
     */
    dir = (tp & mask)?1:0; 
    salto = 1;
    
    //Obtiene el valor del salto para recorrer el cromosoma
    for(i=0; i<3; i++) {
        tp = tp >> 1;
        for(b = 1, m = 2; m > i; m--) { b *= 2;}
        if(tp & mask) salto = salto + b;
    }
    
/*
    printf("ini: %d\n", ini);
    printf("dir: %d\n", dir);
    printf("salto: %d\n", salto);
    printf("chrom: ");
    for(i=0; i<numero_piezas; i++) {
        printf("%d ", chrom[i]);
    }
    printf("\n");
*/
    
    /*
     * Obtiene el orden en que deben ser insertadas las piezas según el cromosoma
     * orden[]: arreglo donde se almacena el id de las piezas
     */
    vueltas = 0;
    while(go) {
        if(dir) {
            pIni = 0;
            while(arreglo_ocupado[pIni] != 0) {
                pIni++;
            }
            for(i=pIni; i<numero_piezas; i = i+salto) {
                if(arreglo_ocupado[i] == 0) {
                    valPos = chrom[i];
                    if(valPos==ini) {
                        arreglo_ocupado[i] = 1;
                        arreglo_orden[nAsig] = i;
                        nAsig++;
                    }
                    if(nAsig == numero_piezas) go = 0;
                }
            }
        } else {
            pIni = numero_piezas-1;
            while(arreglo_ocupado[pIni] != 0) {
                pIni--;
            }
            for(i=pIni; i>=0; i = i-salto) {
                if(arreglo_ocupado[i] == 0) {
                    valPos = chrom[i];
                    if(valPos==ini) {
                        arreglo_ocupado[i] = 1;
                        arreglo_orden[nAsig] = i;
                        nAsig++;
                    }
                    if(nAsig == numero_piezas) go = 0;
                }
            }
        }
        vueltas++;
        
        dir = (dir==0)?1:0;
        if(vueltas%2 == 0) ini = (ini==0)?1:0;
    }
    
/*
    printf("Orden: ");
    for(i=0; i<numero_piezas; i++) {
        printf("%d ", arreglo_orden[i]);
    }
*/
    creaLayout();
    critter->fitness = fitness();
    TEval PEval;
    PEval.perdida = (float)(mayorAltura()*ancho)-area_total;
    critter->PEval = PEval;
/*
    printf("=> %d %f\n", fitness(), (float)(mayorAltura()*ancho)-area_total);
*/
}



/*
 * Escribe resultados en el archivo de salida
 */
void app_genera_resultados_problema_sp(int corrida, int tipo_problema, char *nombrearchivo) {
    printf("app_genera_resultados_problema_sp\n");
    if (encabezado_resultado_problema == 0) {
        //IMPRIME ENCABEZADO DE LOS RESULTADOS DEL PROBLEMA
        fprintf(rprofp, "CORRIDA;TIPO_PROBLEMA;ARCHIVO_PROBLEMA;CANTIDAD_PIEZAS;CANTIDAD_TIPOS_PIEZAS;");
        fprintf(rprofp, "ANCHO_LAMINA;LARGO_LAMINA;PESO_PERDIDA;PESO_DISTANCIA;PESO_DIGREGACION;");
        fprintf(rprofp, "MS_CALIDAD;MS_FITNESS;MS_PERDIDA_REAL;MS_DISTANCIA;MS_DIGREGACION;");
        fprintf(rprofp, "MS_AREA_OCUPADA;MS_NUMERO_PIEZAS_COLOCADAS;MS_NUMERO_PERDIDAS_GENERADAS;");
        fprintf(rprofp, "GENERACION_OCURRENCIA\n");
        encabezado_resultado_problema = 1; //Con ésto no imprime más el encabezado
    }//End if 
    //IMPRIME RESULTADOS DEL PROBLEMA
/*
    fprintf(rprofp, "%d;%d;%s;%d;%d;%d;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%d;%d;%d\n",
        //CORRIDA;TIPO_PROBLEMA;ARCHIVO_PROBLEMA;CANTIDAD_PIEZAS;CANTIDAD_TIPOS_PIEZAS;ANCHO_LAMINA;LARGO_LAMINA;
        corrida, tipo_problema, nombrearchivo, NumPie, cantidadtipospiezas, AnchoPl, AltoPl,
        //PESO_PERDIDA;PESO_DISTANCIA;PESO_DIGREGACION;MS_CALIDAD;MS_FITNESS;		
        peso_perdida, peso_distancia, peso_digregacion, bestfit.PEval.calidad, bestfit.fitness,
        //MS_PERDIDA_REAL;MS_DISTANCIA;MS_DIGREGACION;
        bestfit.PEval.c_perdidareal, bestfit.PEval.c_distancia, bestfit.PEval.c_digregacion,
        //MS_AREA_OCUPADA;MS_NUMERO_PIEZAS_COLOCADAS;MS_NUMERO_PERDIDAS_GENERADAS;GENERACION_OCURRENCIA;
        bestfit.PEval.areaocupada, bestfit.PEval.piezas, bestfit.PEval.n_perdidas, bestfit.generation);
*/
}

