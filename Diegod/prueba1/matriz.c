// CÓDIGO PARA CREAR UNA MATRIZ NXM

#include <stdio.h>
#include <string.h>
#include <time.h> //Para la semilla aleatoria
#include <math.h> //Necesario para la potencia
#include <stdlib.h> //Para el uso de rand() y srand()

// PLANTEAMIENTO //////////////////////
// 
// Voy a crear una matriz nxm, primero la inicializaré y tendrá los valores: 0,1,2.
// 0 = no hay partícula
// 1 = hay una partícula fría
// 2 = hay una partícula caliente

// Si la partícula es fría tendrá menor probabilidad de moverse.
// Crearé también una distribución de probabilidad para que sea solo cambiar eso
// La matriz tendrá varias divisiones, no vamos a poner n o m primo. Por ejemplo si n=9 pues que tenga 3 divisiones y si m=6 pues 2 divisiones. 
// Será en la frontera de estas divisiones que el demonio actúe.
// Si una partícula quiere pasar desde el extremo de una, al borde de otra tendrá que recibir el permiso del demonio.
// Cada vez que el demonio haga una acción, aumentará la entropía.

// CÓMO SE MUEVEN LAS PARTÍCULAS /////////////////
//
// Se moverán primero por fila y luego por columna.
// Se comprobará en cada iteración si hay una partícula alrededor, porque no pueden ocupar 2 el mismo hueco
// 



#define N 9         //Numero de filas
#define M 9         //Numero de columnas
#define div_N 3     //Dimension de la sección en fila
#define div_M 3     //Dimension de la sección en columna

#define T_TOTAL     //Número total de posibilidad de pasos. Es decir. Numero de iteraciones en las que la matriz ha podido modificarse. 



double r[N][M];
