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



#define N 9         //Numero de filas.
#define M 9         //Numero de columnas.
#define div_N 3     //Dimension de la sección en fila.
#define div_M 3     //Dimension de la sección en columna.

#define part_hot 10 //Número de partículas calientes.
#define part_cold 10//Número de partículas frías.
#define T_TOTAL     //Número total de posibilidad de pasos. Es decir. Numero de iteraciones en las que la matriz ha podido modificarse. 



int matriz[N][M];


//Función de inicializar la matriz.

void inicializar(int matriz[N][M])
{
    
    //Las inicializo a 0
    for (int i=0; i<N; i++)
    {
        for(int j=0; j<M; j++)
        {
            matriz[i][j]=0;
        }
    }

    //Pongo las frías:
    int contador=0;

    while (contador<part_cold)
    {
        int aux_fila = rand()%N;
        int aux_col = rand()%M;

        if (matriz[aux_fila][aux_col]==0)
        {
            matriz[aux_fila][aux_col]=1;
            contador++;
        }
    }

    //Pongo las calientes:
    contador=0;

    while (contador<part_hot)
    {
        int aux_fila = rand()%N;
        int aux_col = rand()%M;

        if (matriz[aux_fila][aux_col]==0)
        {
            matriz[aux_fila][aux_col]=2;
            contador++;
        }
    }

    return;
}



//Creo el programa entero

int main(void)
{

    FILE *matriz_file = fopen("MATRIZ.txt", "w");    //Fichero donde se guarda la matriz y sus pasos

    if (matriz_file == NULL) {
        printf ("Error al abrir el archivo JAJAJA. \n");
        return 1;
    }

    //Voy a crear la matriz de todo 0. 
    //A continuación, rellenaré las partículas frías y calientes. ¿Cómo?
    //Escojo una posición al azar, si hay un 0, pues que ponga una partícula, sino, que lo intente otra vez.

    //Para los números aleatorios, pongo la semilla en null
    srand(time(NULL));

    //Inicializo la matriz llamando a la función
    inicializar(matriz);


    //Guardo esta iteración
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fprintf(matriz_file, "%d", matriz[i][j]);

            if (j < M - 1) {
                fprintf(matriz_file, "\t");
            }
        }
        fprintf(matriz_file, "\n");
    }

    fprintf(matriz_file, "\n");


    
    fclose(matriz_file);
    printf("Lo he hecho todo bien");
    return 0;    
}
