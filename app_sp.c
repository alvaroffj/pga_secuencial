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

int findPos(int w) {
/*
    printf("findPos: %d\n", w);
*/
    int go = 1, menor = -1, mayor = -1, pos = -1, j, ini, lastMenor = -1, ok = 1, n=0;
    while(go == 1) {
        if(menor == -1) {
            menor = arreglo_alturas[0];
            mayor = arreglo_alturas[0];
            pos = 0;
            ini = 1;
        }
        for(j=ancho-1; j>=0; j--) {
            if(arreglo_alturas[j]<=menor && arreglo_alturas[j]>lastMenor) {
                menor = arreglo_alturas[j];
                pos = j;
            }
            if(arreglo_alturas[j]>mayor) mayor = arreglo_alturas[j];
        }
/*
        printf("%d=>%d\n", pos, menor);
*/
        ok = 1;
        if(w + pos <= ancho) {
/*
            for(j=pos; j<w+pos; j++) {
                if(arreglo_alturas[j] > menor) {
                    ok = 0;
                    j = w+pos;
                    lastMenor = menor;
                    menor = mayor+1;
*/
/*
                    printf("lastMenor: %d\n", lastMenor);
                    printf("Menor: %d\n", menor);
*/
/*
                }
            }
*/
        } else {
            ok = 0;
            lastMenor = menor;
            menor = mayor+1;
/*
            printf("*lastMenor: %d\n", lastMenor);
            printf("*Menor: %d\n", menor);
*/
        }
        if(ok == 1) go = 0;
        if(lastMenor == menor) return 0;
    }
    return pos;
}



int mayorHEntre(int a, int b) {
    int i=0, mayor = arreglo_alturas[a];
    for(i=a+1; i<b; i++) {
        if(mayor<arreglo_alturas[i])
            mayor = arreglo_alturas[i];
    }
    return mayor;
}

int posMayorHEntre(int a, int b) {
    int i=0, mayor = arreglo_alturas[a], pos = 0;
    for(i=a+1; i<b; i++) {
        if(mayor<arreglo_alturas[i])
            pos = i;
    }
    return pos;
}

int izquierda(int x, int y) {
    int i = x;
    while(arreglo_alturas[i]<=y && i>=0) i--;
    i++;
    if(x<=i) return -1;
    else return i;
}

int abajo(int x, int h, int w) {
    int mayor = mayorHEntre(x, x+w);
    if(h<=mayor) return -1;
    else return mayor;
}

int posNMenorAltura(int n, int w) {
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
    float fitness = (float)mayorAltura();
/*
    float fitness = (float)(2*mayorAltura()*ancho)-area_total;
*/
/*
    printf("fitness: %d\n", fitness);
*/
    return(fitness);
}

/*
 * Inserta las piezas en la tira, según el orden y sentido correspondiente
 */
void creaLayout(int write) {
    int i, x = 0, j, altura = 0, n = 1, fit = 0, cAncho = 0, cAlto = 0, pen = 0, y = 0, mayorH, ok = 1, down, left, xIni, yIni, go = 1;
    Datos_pieza cPieza;

    for(i=0; i<ancho; i++) {
        arreglo_alturas[i] = 0;
    }
    for(i=0; i<numero_piezas; i++) {
        arreglo_ocupado[i] = 0;
    }
    if(write==1) {
        fprintf(outfp, "Piezas: ");
    }
    for(i=0; i<numero_piezas; i++) {
/*
        x = posMenorAltura();
*/
        cPieza = lista_piezas[arreglo_orden[i]];
/*
        if(write==1)
            printf("ubicar pieza[%d]: %d %d x %d\n", arreglo_orden[i], arreglo_rotar[arreglo_orden[i]], cPieza.ancho, cPieza.alto);
*/
        n = 1;
        down = 1; 
        left = 1;
        mayorH = mayorHEntre(0, ancho);
/*
        if(write==1)
            printf("mayorH: %d\n", mayorH);
*/
        while(!fit) {
            ok=1;
            if(arreglo_rotar[arreglo_orden[i]] == 1) { //normal
                cAncho = cPieza.ancho;
                cAlto = cPieza.alto;
            } else { //rotada en 90°
                if(cPieza.alto <= ancho) {
                    cAlto = cPieza.ancho;
                    cAncho = cPieza.alto;
                } else {
                    arreglo_rotar[arreglo_orden[i]] = 1;
                    ok = 0;
                }
            }
            if(ok) {
                j=0;
                go = 1;
                while(go) {
                    if(j==0) {
                        xIni = ancho-cAncho;
                        yIni = mayorH;
                    } else {
                        xIni = x;
                        yIni = y;
                    }
                    if(down) {
                        y = abajo(xIni, yIni, cAncho);
/*
                        if(write==1) printf("*y: %d\n", y);
*/
                        if(y<0) {
                            down = 0;
                            y = yIni;
                        } else down = 1;
                    }
/*
                    if(write==1) printf("y: %d\n", y);
*/
                    if(left) {
                        x = izquierda(xIni, y);
/*
                        if(write==1) printf("*x: %d, y: %d\n", x, y);
*/
                        if(x<0) {
                            left = 0;
                            x = xIni;
                        } else left = 1;
                    }
/*
                    if(write==1) printf("x: %d\n", x);
*/
                    j++;
                    if(j>10) exit(0);
                    if(down || left) {
                        go = 1;
                        down = 1;
                        left = 1;
                    } else go = 0;
/*
                    if(write==1)
                        if(go) printf("go\n");
                        else printf("stop\n");
*/
                    fit = !go;
                }
            }
        }
        fit = 0;
        if(write==1) {
            fprintf(outfp, "%d,%d,%d,%d;", x, y, cAncho, cAlto);
        }
        for(j=x; j<cAncho+x; j++) {
            arreglo_alturas[j] = y + cAlto;
        }
/*
        if(write==1) {
            printf("arreglo_altura: ");
            for(j=0; j<ancho; j++) {
                printf("%d ", arreglo_alturas[j]);
            }
            printf("\n");
        }
*/
    }
    if(write==1) {
        fprintf(outfp, "\n");
        fprintf(outfp, "arreglo_altura: ");
        for(j=0; j<ancho; j++) {
            fprintf(outfp, "%d ", arreglo_alturas[j]);
        }
        fprintf(outfp, "\n");
    }
/*
    if(pen>0) {
        for(j=0; j<ancho; j++) {
            arreglo_alturas[j] = arreglo_alturas[j] + 10*pen;
        }
    }
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
/*
    printf("Leyendo archivo de entrada\n");
*/

    sprintf(nombre_archivo, "%s%s", ruta_instancias, nombrearchivo);


    if((fp = fopen(nombre_archivo,"r"))== NULL){//Si no se encuentra o hay errores al leer el archivo de entrada
        printf("Error al leer archivo %s\n",nombrearchivo);
        return 0;
    }

    // Lee el numero de piezas y el ancho del strip
    fscanf(fp,"%d", &numero_piezas);
    fscanf(fp,"%d", &ancho);
/*
    printf("Piezas: %d\nAncho: %d\n", numero_piezas, ancho);
*/
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
        int id = 0;
        // Lee ancho y alto para cada pieza
        // Ciclo para leer todas las piezas del archivo de entrada
        while( fscanf(fp,"%d %d %d", &id, &aux_pieza.ancho, &aux_pieza.alto) != EOF) {
            aux_pieza.area = aux_pieza.ancho * aux_pieza.alto; //Se calcula el area de la pieza
            aux_pieza.id = j; //Se asigna id a la pieza
            area_total += aux_pieza.area;
            lista_piezas[j] = aux_pieza; //Se ingresa la pieza
/*
            printf("Pieza %d: %d x %d => %d\n", lista_piezas[j].id, lista_piezas[j].ancho, lista_piezas[j].alto, lista_piezas[j].area);
*/
            j++;
        }
        
/*
        printf("Area Total: %d\n", area_total);
*/
        //Cierra archivo de piezas
        fclose(fp);
        largo_cromosoma = numero_piezas + bit_reservados;
        fitness_inicial = (float) area_total*10;
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
     * arreglo_orden[]: arreglo donde se almacena el id de las piezas
     */
    vueltas = 0;
    while(go) { //mientras queden piezas por ordenar
        if(dir == 1) {//izq->der
            pIni = 0; //posicion inicial
            while(arreglo_ocupado[pIni] != 0) { //si ya se ordeno la pieza pasa a la de la derecha
                pIni++;
            }
            for(i=pIni; i<numero_piezas; i = i+salto) { //desde la posicion inicial hasta el final, con (i + salto) como valor de incremento
                if(arreglo_ocupado[i] == 0) { // si no ha sido agregado
                    valPos = chrom[i]; //obtiene el valor en esa posicion
                    if(valPos==ini) { //si es igual al valor buscado se agrega al listado de piezas ordenado
                        arreglo_ocupado[i] = 1; //se marca como leida
                        arreglo_orden[nAsig] = i; //se agrega al listado de piezas ordenado
                        nAsig++;
                    }
                    if(nAsig == numero_piezas) go = 0; //si se ordenaron todas las piezas termina
                }
            }
        } else {//der->izq
            pIni = numero_piezas-1; //se ubica en la ultima posicion del string
            while(arreglo_ocupado[pIni] != 0) { //si ya se ordeno la pieza pasa a la de la izquierda
                pIni--;
            }
            for(i=pIni; i>=0; i = i-salto) { //desde la posicion inicial hasta el inicio del string, con (i + salto) como valor de retroceso
                if(arreglo_ocupado[i] == 0) { // si no ha sido agregado
                    valPos = chrom[i]; //obtiene el valor en esa posicion
                    if(valPos==ini) { //si es igual al valor buscado se agrega al listado de piezas ordenado
                        arreglo_ocupado[i] = 1; //se marca como leida
                        arreglo_orden[nAsig] = i; //se agrega al listado de piezas ordenado
                        nAsig++;
                    }
                    if(nAsig == numero_piezas) go = 0;
                }
            }
        }
        vueltas++;
        
        dir = (dir==0)?1:0; //se cambia la direccion en que se recorre el string
        if(vueltas%2 == 0) ini = (ini==0)?1:0; //si ya se dio una vuelta completa, entonces se invierte el valor a buscar
    }
    
/*
    printf("Orden: ");
    for(i=0; i<numero_piezas; i++) {
        printf("%d ", arreglo_orden[i]);
    }
*/
    creaLayout(0);
/*
    critter->fitness = fitness();
*/
    TEval PEval;
    PEval.perdida = (float)(mayorAltura()*ancho)-area_total;
    critter->PEval = PEval;
    critter->fitness = (float) mayorAltura();
/*
    critter->fitness = (float)(2*mayorAltura()*ancho)-area_total;
*/
/*
    printf("critter->fitness: %f\n", critter->fitness);
*/
/*
    printf("fitness: %d\n", (2*mayorAltura()*ancho)-area_total);
*/
}

void app_objfuncfinal_sp(struct bestever *critter) {
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
    
    fprintf(outfp, "Mejor resultado\n");
    fprintf(outfp, "Corrida: %d\n", run);
    fprintf(outfp, "Instancia: %s\n", nomarch);
    fprintf(outfp, "Fitness: %f\n", critter->fitness);
    fprintf(outfp, "ini: %d\n", ini);
    fprintf(outfp, "dir: %d\n", dir);
    fprintf(outfp, "salto: %d\n", salto);
    fprintf(outfp, "chrom: ");
    for(i=0; i<numero_piezas; i++) {
        fprintf(outfp, "%d ", chrom[i]);
    }
    fprintf(outfp, "\n");
    
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
    
    
    fprintf(outfp, "O: ");
    for(i=0; i<numero_piezas; i++) {
        fprintf(outfp, "%d ", arreglo_orden[i]);
    }
    fprintf(outfp, "\n");
    
    fprintf(outfp, "R: ");
    for(i=0; i<numero_piezas; i++) {
        fprintf(outfp, "%d ", arreglo_rotar[arreglo_orden[i]]);
    }
    fprintf(outfp, "\n");
    creaLayout(1);
    fprintf(outfp, "--------------------------------------------------------------------------------------------\n");
}



/*
 * Escribe resultados en el archivo de salida
 */
void app_genera_resultados_problema_sp(int corrida, int tipo_problema, char *nombrearchivo) {
/*
    printf("app_genera_resultados_problema_sp\n");
*/
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

