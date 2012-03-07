/*----------------------------------------------------------*/
/* var_sp.h                                                 */
/* variables globales del Problema Strip Packing            */
/*----------------------------------------------------------*/

int ancho; //ancho de la strip
int numero_piezas; //numero de piezas
int bit_reservados = 5; //Bits usados en el cromosoma para interpretarlo
int area_total;
int *arreglo_alturas; //arreglo que guarda las alturas del strip
int *arreglo_espacios; //arreglo que guarda el espacio siguiente del strip
int *arreglo_ocupado; //arreglo que guarda si la pieza a sido ingresada o no al strip
int *arreglo_orden; //arreglo que guarda el numero de la pieza que es ingresada en la posicion i
int *arreglo_rotar; //arreglo que indica si la pieza i debe ir rotada o no

/* Estructuras de la aplicación */
typedef struct pieza{
    int ancho;  //ancho de la pieza
    int alto;  //largo de la pieza
    int id;  //id de la pieza      
    int area;  //area de la pieza (largo * ancho)  
} Datos_pieza;

Datos_pieza *lista_piezas;
