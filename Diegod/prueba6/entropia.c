// PROGRAMA PARA CALCULAR LA ENTROPÍA DE UN ESTADO CUANDO FUNCIONE LO METERÉ EN EL PROGRAMA PRINCIPAL

#include <stdio.h>
#include <string.h>
#include <time.h> //Para la semilla aleatoria
#include <math.h> //Necesario para la potencia
#include <stdlib.h> //Para el uso de rand() y srand()


// PLANTEMIENTO /////////////////////////////
//
//En cada iteración (incluyendo la inicial) se calculará la entropía, contando el número de partículas que hay de cada tipo (fria, caliente, hueco).
//Esto es lo que hace ya la DENSIDAD, así que no hace falta calcularlo. 
//La entropía sigue la fórmula S= k_B * ln(Gamma), con Gamma= N_total! / (N_frio! * N_caliente! * N_vacio!)
//Se hace para cada subsección, se calcula la entropía de esa subsección, se almacena en una matriz y luego se suman todas para la total
//Se asigna entropía 0 cuando todos las celdas están ocupadas por la misma partícula
//Estaría bien anotar cuando el demonio actúa para poder ver cambios.
//
///////////////////////////////////////////////////


/*

    HAY QUE EXPORTAR LOS DATOS DE DENSIDAD_FRIA Y DENSIDAD_CALIENTE A UN FICHERO Y QUE LO LEA LA ENTROPÍA.

*/

//PARÁMETROS (mismos que el programa principal)

#define N 20                    //Numero de filas.
#define M 20                    //Numero de columnas.
#define div_N 5                 //Dimension de la sección en fila. Por favor, pon divisores del número de filas y columnas.
#define div_M 5                 //Dimension de la sección en columna.

#define part_hot 100            //Número de partículas calientes.
#define part_cold 100           //Número de partículas frías.
#define T_TOTAL 500             //Número total de posibilidad de pasos. Es decir. Numero de iteraciones en las que la matriz ha podido modificarse.

#define umbral_cold 0.5         //Número entre 0 y 1 que tiene que superar la probabilidad para que se mueva la partícula fría.
#define umbral_hot 0.1          //Lo mismo pero para la caliente. SIEMPRE umbral_hot < umbral_cold

#define MEMORIA 100

int matriz[N][M];               //Matriz principal. Es nuestra cuadrícula.
int matriz_auxiliar[N][M];      //Matriz en la que se harán cambios. Esto es para no modificar y trabajar en la misma matriz.
int semueve=0;                  //Variable que indicará dirección de movimiento. 0 arriba, 1 derecha, 2 abajo, 3 izquierda. Cicla en +4, por lo que 4 es arriba, 5 derecha, 6 abajo y 7 izquierda.
int puedemoverse=0;             //Variable que indica la posibilidad de movimiento. ¿Hay celdas contiguas libres? 0 indica que no puede moverse, 1 indica que sí puede moverse.
int contador_movimientos = 0;   //Esta variable nos va a decir cuantas veces ha ciclado sin posibilidad de movimiento. Es decir, si llega a 3 significa que ha recorrido todas las posibilidades sin éxito y se tiene que quedar donde estaba.
double prob_movimiento=0.0;     //Variable que tendrá que superar el umbral para moverse.
int divisor_columna = M/div_M;
int divisor_fila = N/div_N;     //Estas variables ponen la columna o fila que deben atravesar en multiplos enteros. Osea si divisior_M es divisor entero de M, en esa columna (a su derecha) hay una división.
int actua_demonio_cold = 0;     //Veces que actúa el demonio en partículas frías
int actua_demonio_hot = 0;      //Veces que actúa el demonio en partículas calientes      
int valor_demonio = 0;          //¿Ha actuado en esa iteración el demonio?

int pl[div_N][div_M][4]; // Matriz que guarda la presión en cada celda. 0 arriba, 1 derecha, 2 abajo, 3 izquierda. Cada vez que una partícula se quiere mover en esa dirección y no puede, se suma 1 a esa posición
int pr[div_N][div_M][4]; // Igual que antes, Pl indica lento, partículas frías. Pr indica rápido, partículas calientes.

int memory[1][2];
float hamiltoniano[2][T_TOTAL];


//Identificación del sub-bloque de matriz en el que está una partícula.

int bloquefila = 0;         //identifica en qué fila de bloques está
int bloquecolumna=0;        //Identifica en qué columna de bloques está.


//Arrays para la densidad: cuenta cuántas partículas hay en cada subsección y las guarda.

int densidad_cold[div_N][div_M];
int densidad_hot[div_N][div_M];
int densidad_total[div_N][div_M]; //Por si acaso.



//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////        PARÁMETROS NUEVOS      ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

double gamma [div_N][div_M];


int main(void)
{

    FILE *densidad_hot_file = fopen("densidad_hot.txt", "w");

    if(densidad_hot_file == NULL)
    {
        printf("No he abierto bien el archivo JAJAJA. \n");
        return 1;
    }


    fclose(densidad_hot_file);
    printf("Lo he hecho todo bien");
    return 0;
}










