//AHORA VEMOS QUE EL DEMONIO VERDADERAMENTE ACTÚA, VAMOS A CALCULAR LA ENTROPÍA, TEMPERATURA Y DEMÁS.

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
int divisor_fila = N/div_N;     //Estas variables ponen la columna o fila que deben atravesar en multiplos enteros. Osea si divisior_M es divisor entero de M, en esa columna (a su derecha) hay una división.
int actua_demonio_cold = 0;     //Veces que actúa el demonio en partículas frías
int actua_demonio_hot = 0;      //Veces que actúa el demonio en partículas calientes      
int valor_demonio = 0;          //¿Ha actuado en esa iteración el demonio?


//Identificación del sub-bloque de matriz en el que está una partícula.
int bloquefila = 0;         //identifica en qué fila de bloques está
int bloquecolumna=0;        //Identifica en qué columna de bloques está.

//Arrays para la densidad: cuenta cuántas partículas hay en cada subsección y las guarda.
int densidad_cold[div_N][div_M];
int densidad_hot[div_N][div_M];
int densidad_total[div_N][div_M]; //Por si acaso.



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




    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////         INICIALIZAR         /////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////






    FILE *matriz_file = fopen("MATRIZ.txt", "w");       //Fichero donde se guarda la matriz y sus pasos. El paso se diferencia por salto de línea vacío.
    FILE *demonio_file = fopen("demonio.txt", "w");     //Fichero donde se se guarda la partícula sobre la que ha actuado el demonio. 0=no ha actuado, 1=sobre fría, 2=sobre caliente. El paso se diferencia por salto de línea vacío.
    FILE *densidad_file = fopen("densidad.txt", "w");   //Fichero donde se guarda la densidad de cada sección de la matriz grande en una matriz más pequeña. El paso se diferencia por salto de línea vacío.    

    if (matriz_file == NULL || demonio_file==NULL || densidad_file==NULL) {
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






    //Pongo las variables del demonio a 0.


    actua_demonio_cold=0;
    actua_demonio_hot=0;

    //Array de movimiento. La primera posicion es arriba, la segunda derecha, tercera abajo y cuarta izquierda. 0 indica libre, 1 indica ocupado.
    int movimiento[4];

    //HAGO EL BUCLE GRANDE. NO PARARÁ HASTA QUE TERMINE T_TOTAL
    int contador=0;









    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////          BUCLE GENERAL           //////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




    
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
                                            actua_demonio_cold++;
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
                                            actua_demonio_cold++;
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
                                            actua_demonio_hot++;
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
                                            actua_demonio_hot++;
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






        ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////////////           DENSIDAD            ///////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////





        //Ahora vamos a calcular la densidad de cada bloque.
        //Para modelar la densidad de cada división trabajaremos con las celdas de cada submatriz. No trabajaremos con submatrices, sino con la grande.
        //Vamos a crear el parámetro "bloquefila" y "bloquecolumna", para que nos diga exactamente en qué región está.
        //Barreremos todas las submatrices e iremos anotando la cantidad de partículas que hay y si son calientes o frías. 
        //No tendremos en cuenta si la partícula caliente aporta mayor o menor densidad al ser una propiedad microscópica y no tener potencial de repulsión ni energía cinética.
        //La temperatura de las partículas se tendrá en cuenta para la temperatura (obvio) y para la presión, un rebote de una caliente trae más presión que un rebote de una fría.

        
        //Inicializo los arrays de densidad a 0

        for(int i=0; i<div_N; i++)
        {
            for(int j=0; j<div_M; j++)
            {
                densidad_cold[i][j]=0;
                densidad_hot[i][j]=0;
                densidad_total[i][j]=0;
            }
        }


        //Barro cada posición de matriz y anoto si está o no para su densidad.

        for(int i=0; i<N; i++)
        {
            for(int j=0; j<M; j++)
            {
                //Miro que haya partícula
                if(matriz[i][j]!=0)
                {   
                    //Hago la división entera para que me dé la sección correcta
                    bloquefila=i/divisor_fila;
                    bloquecolumna=j/divisor_columna;

                    //Identifico si es fría
                    if(matriz[i][j]==1)
                    {
                        densidad_cold[bloquefila][bloquecolumna]++;
                        densidad_total[bloquefila][bloquecolumna]++;
                    //Termina el if de fría
                    }
                    //Identifico si es caliente
                    else
                    {
                        densidad_hot[bloquefila][bloquecolumna]++;
                        densidad_total[bloquefila][bloquecolumna]++;
                    //Termina el if de caliente
                    }

                //Termina el if de elemento matriz no nulo
                }

            //Termino de barrir la columna
            }
        
        //Termino de barrir la fila
        }

        //Guardo esta matriz en el archivo. Los arrays de frío y caliente no los guardo por ahora. PODRÍA HACERLO PERO POR LA SUPOSICIÓN ANTERIOR, SOLO ESTÁN AHÍ POR SI ACASO.

        for(int i=0; i<div_N; i++)
        {
            for(int j=0; j<div_M; j++)
            {
                fprintf(densidad_file, "%d", densidad_total[i][j]);
                if(j<div_M-1)
                {
                    fprintf(densidad_file, "\t");
                }
            }
            fprintf(densidad_file, "\n");
        }
       fprintf(densidad_file, "\n");




        contador++;
        //Termina el bucle total de movimiento.
    }






    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                     PARTE DEL DEMONIO                     /////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





    //Vamos con el demonio como actúa de verdad.
    printf("EL DEMONIO HA HECHO:");
    printf("\n");
    printf("%d", actua_demonio_cold);
    printf("\n");
    printf("%d", actua_demonio_hot);
    printf("\n");



    
    fclose(matriz_file);
    fclose(demonio_file);
    fclose(densidad_file);
    printf("Lo he hecho todo bien :)");
    return 0;    
}
