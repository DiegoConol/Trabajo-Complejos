// AHORA ACTÚA EL DEMONIO AL REVÉS, COMO DICE EL PROFE.
//
//Hay una puerta permanente que no deja pasar a las partículas.
//Cuando deja pasar a una partícula entonces aumenta la entropía.


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



#define N 20                 //Numero de filas.
#define M 20                 //Numero de columnas.
#define div_N 4             //Dimension de la sección en fila. Por favor, pon divisores del número de filas y columnas.
#define div_M 4             //Dimension de la sección en columna.

#define part_hot 50         //Número de partículas calientes.
#define part_cold 50        //Número de partículas frías.
#define T_TOTAL 500         //Número total de posibilidad de pasos. Es decir. Numero de iteraciones en las que la matriz ha podido modificarse.

#define umbral_cold 0.5     //Número entre 0 y 1 que tiene que superar la probabilidad para que se mueva la partícula fría.
#define umbral_hot 0.1      //Lo mismo pero para la caliente. SIEMPRE umbral_hot < umbral_cold



int matriz[N][M];           //Matriz principal. Es nuestra cuadrícula.
int matriz_auxiliar[N][M];  //Matriz en la que se harán cambios. Esto es para no modificar y trabajar en la misma matriz.
int semueve=0;              //Variable que indicará dirección de movimiento. 0 arriba, 1 derecha, 2 abajo, 3 izquierda.
int puedemoverse=0;         //Variable que indica la posibilidad de movimiento. ¿Hay celdas contiguas libres?
int haydireccion=-1;        //Variable que indica si la dirección propuesta es válida. ¿Justo a la celda que me quiero mover está libre?
double prob_movimiento=0.0; //Variable que tendrá que superar el umbral para moverse.
int divisor_columna = M/div_M;
int divisor_fila = N/div_N;     //Estas variables ponen la columna o fila que deben atravesar en multiplos enteros. Osea si divisior_M es divisor entero de 5, en esa columna hay una división.
int actua_demonio = 0;      //Veces que actúa el demonio
int valor_demonio = 0;      //¿Ha actuado en esa iteración el demonio?

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

    FILE *matriz_file = fopen("MATRIZ.txt", "w");       //Fichero donde se guarda la matriz y sus pasos
    FILE *demonio_file = fopen("demonio.txt", "w");     //Fichero donde se guarda el paso, la cantidad de veces que ha actuado el demonio.     

    if (matriz_file == NULL || demonio_file==NULL) {
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

    

    ////////////////////////////////////////////////////////////////////////////////////////////////////////


    //Array de movimiento. La primera posicion es arriba, la segunda derecha, tercera abajo y cuarta izquierda. 0 indica libre, 1 indica ocupado.
    int movimiento[4];

    //HAGO EL BUCLE GRANDE. NO PARARÁ HASTA QUE TERMINE T_TOTAL
    int contador=0;
    while(contador<T_TOTAL)
    {   
        //Inicializo la matriz auxiliar a la matriz. La matriz auxiliar la usaré para poner los cambios de matriz principal. Luego la igualaré y el bucle estará completo.
        for (int i=0; i<N; i++)
        {
            for (int j=0; j<M; j++)
            {
                matriz_auxiliar[i][j]=matriz[i][j];
            }
        }


        for (int i=0; i<N; i++)
        {
            for (int j=0; j<M; j++)
            {   

                valor_demonio=0;
                
                if(matriz[i][j]>0)
                {
                    //COMPRUEBO QUE NO HAYA PARTÍCULA EN ESA POSICIÓN.
                    //Si hay un 0, puede moverse en esa dirección. Sino no, tiene que escoger otra. Lo que voy a hacer es que vaya ciclando.
                    //Es decir, si no puede ir arriba, que vaya a la derecha, sino abajo y sino a la izquierda.
                    //La variable auxiliar_contador_semueve ayuda a que no entre en bucle. Si llega a 4 es que no hay movimiento posible --> que pase a la siguiente partícula.


                    //Veo primero a DONDE se puede mover. Asumo primero que todo está bloqueado. De esta forma me quito los bordes de la matriz como muros infranqueables.

                    movimiento[0] = 1;
                    movimiento[1] = 1;
                    movimiento[2] = 1;
                    movimiento[3] = 1;

                    //También pongo la variable 'puedemoverse'. Si hay almenos una casilla que sea 0, pongo su valor en 1. Si no puede moverse, pasamos a la siguiente partícula.
                    puedemoverse=0;
                    //La condicion de los bordes es que sea mayor y menor respectivamente. Como en la posición 0 no puede irse a la -1, solo puede ir a la 0 la que esté en 1. Mismo razonamiento para todos.

                    if (i > 0 && matriz_auxiliar[i-1][j] == 0) {
                        movimiento[0] = 0;
                        puedemoverse=1;
                    }

                    if (j < M-1 && matriz_auxiliar[i][j+1] == 0) {
                        movimiento[1] = 0;
                        puedemoverse=1;
                    }

                    if (i < N-1 && matriz_auxiliar[i+1][j] == 0) {
                        movimiento[2] = 0;
                        puedemoverse=1;
                    }

                    if (j > 0 && matriz_auxiliar[i][j-1] == 0) {
                        movimiento[3] = 0;
                        puedemoverse=1;
                    }

                    //Otro if grande. Si es que puede moverse:
                    
                    if(puedemoverse==1)
                    {
                        haydireccion=-1;
                        //Escojo una dirección aleatoria. 0 es arriba, 1 derecha, 2 abajo, 3 izquierda.

                        semueve=rand()%4; //Se escoge la dirección de movimiento al azar.

                        if (semueve==0 && movimiento[0]==0){haydireccion=0;}
                        if (semueve==1 && movimiento[1]==0){haydireccion=1;}
                        if (semueve==2 && movimiento[2]==0){haydireccion=2;}
                        if (semueve==3 && movimiento[3]==0){haydireccion=3;}

                        if(haydireccion!=-1)
                        {
                            //Ahora escojo una probabilidad de que se mueva.
                            prob_movimiento = (double)rand()/ (double) RAND_MAX; //Esto da una probabilidad uniforme. El double le hace casting.
                            //Para el caso de que sea fría
                            if (matriz[i][j]==1)
                            {
                                if (prob_movimiento >= umbral_cold)
                                {
                                    //ARRIBA
                                    if (haydireccion == 0)
                                    {
                                        if (((i)%divisor_fila == 0) && (i) != 0)
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i-1][j] = 1;
                                            actua_demonio++;
                                            valor_demonio = 1;
                                        }
                                        else
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i-1][j] = 1;
                                        }
                                    }
                                    //DERECHA
                                    else if (haydireccion == 1)
                                    {
                                        if (((j+1)%divisor_columna == 0) && (j+1) != M)
                                        {
                                            matriz_auxiliar[i][j] = 1;   // se queda donde estaba
                                            
                                        }
                                        else
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i][j+1] = 1;
                                        }
                                    }
                                    //ABAJO
                                    else if (haydireccion == 2)
                                    {
                                        if (((i+1)%divisor_fila == 0) && (i+1) != N)
                                        {
                                            matriz_auxiliar[i][j] = 1;   // se queda donde estaba
                                            
                                        }
                                        else
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i+1][j] = 1;
                                        }
                                    }
                                    //IZQUIERDA
                                    else if (haydireccion == 3)
                                    {
                                        if (((j)%divisor_columna == 0) && (j) != 0)
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i][j-1] = 1;
                                            actua_demonio++;
                                            valor_demonio = 1;
                                        }
                                        else
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i][j-1] = 1;
                                        }
                                    }
                                }
                                //Termina el if de fría
                            }
                            //Para el caso de que sea caliente
                            else
                            {
                                if (prob_movimiento >= umbral_hot)
                                {
                                    //ARRIBA
                                    if(haydireccion==0)
                                    {
                                        if (((i)%divisor_fila == 0) && (i) != 0)
                                        {
                                            matriz_auxiliar[i][j] = 2;   // se queda donde estaba
                                        }
                                        else
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i-1][j] = 2;
                                        }
                                    }
                                    //DERECHA
                                    else if(haydireccion==1)
                                    {
                                        if (((j+1)%divisor_columna == 0) && (j+1) != M)
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i][j+1] = 2;
                                            actua_demonio++;
                                            valor_demonio = 2;
                                        }
                                        else
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i][j+1] = 2;
                                        }
                                    }
                                    //ABAJO
                                    else if(haydireccion==2)
                                    {
                                        if (((i+1)%divisor_fila == 0) && (i+1) != N)
                                        {
                                            matriz_auxiliar[i][j]=0;
                                            matriz_auxiliar[i+1][j]=2;
                                            actua_demonio++;
                                            valor_demonio = 2;
                                        }
                                        else
                                        {
                                            matriz_auxiliar[i][j]=0;
                                            matriz_auxiliar[i+1][j]=2;
                                        }
                                    }
                                    //IZQUIERDA
                                    else if(haydireccion==3)
                                    {
                                        if (((j)%divisor_columna == 0) && (j) != 0)
                                        {
                                            matriz_auxiliar[i][j] = 2;   // se queda donde estaba
                                        }
                                        else
                                        {
                                            matriz_auxiliar[i][j] = 0;
                                            matriz_auxiliar[i][j-1] = 2;
                                        }
                                    }
                                }
                                //Termina if de caliente
                            }
                            //Termina la elección de dirección.
                        }

                        //Termina el poder moverse.
                    }
                    
                    //Termina el elemento de matriz no nulo.
                }
                
                fprintf(demonio_file, "%d", valor_demonio); //Si no hay partícula, el demonio no actúa..
                fprintf(demonio_file, "\n");
                //Termina la columna
            }
            //Termina la fila
        }

        //Cada vez que hagamos una matriz entera, pongo un salto de línea al demonio.
        fprintf(demonio_file, "\n");
        
    

        //Pongo de nuevo los valores de matrizauxiliar en la matriz normal y los guardo en el fichero.

        for(int i=0; i<N; i++)
        {
            for(int j=0; j<M; j++)
            {
                matriz[i][j]=matriz_auxiliar[i][j];
                fprintf(matriz_file, "%d", matriz[i][j]);

                if (j < M - 1) {
                    fprintf(matriz_file, "\t");
                }
            }
            fprintf(matriz_file, "\n");
        }
        fprintf(matriz_file, "\n");

        contador++;
        //Termina el bucle total de movimiento.
    }



    
    fclose(matriz_file);
    fclose(demonio_file);
    printf("Lo he hecho todo bien :)");
    return 0;    
}
