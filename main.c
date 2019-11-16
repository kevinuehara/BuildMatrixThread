/**
 * @author Kevin Toshihiro Uehara 200666
 * @author Pedro Henrique do Amaral Loures 223340
 * @since 13/11/2019
 * 
 * @description Programa principal do projeto BuildMatrixThread, 
 *  responsável por ler números dos arquivos passados como parâmtros, ordená-los,
 *  e escrever no arquivo em formato de matriz. 
 **/

//Bibliotecas utilizadas pelo programa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h> 
#include <time.h>

/* Matriz onde será armazenado os números lidos para escrever no arquivo de saída */
int ** matrix;

/* Representa o arquivo de saída para a escrita da matriz*/
FILE * file_write;

/* Representa um semáforo para controle da escrita das threads no arquivo de saída*/
sem_t lock; 

/*
* Struct que será utilizado para o controle do acesso das threads na matriz,
* identificando quantas linhas a thread irá ler.
* Atributos:
* line-> representa a linha inicial
* limit_line -> representa a linha final
* limit_col -> representa a quantidade de colunas
*/
typedef struct Data {
    int line;
    int limit_line;
    int limit_col;
} Data;

/*
* Struct que será utilizado para armazenar no atributo array os números lidos
* dos arquivos.
* Atributos:
* array -> representa um array com os números lidos de um arquivo
* top -> representa a quantidade de números do array
*/
typedef struct ArrayDataManager {
    int * array;
    int top;
} ArrayDataManager;

/**
 * Função responsável por escrever a matriz no arquivo de saída, sendo utilizada 
 * para a criação das threads. É utilizado um semáforo para controlar a escrita
 * no arquivo.
 * @param args: representa a struct Data para controlar o acesso à matriz
 * **/
void * build_matrix_in_file(void *args) {
    sem_wait(&lock); 

    Data * my_data =  (Data *) args;
    for(int i=my_data->line; i<=my_data->limit_line; i++) {
        for(int j=0; j<my_data->limit_col; j++) {
            fprintf(file_write, "%d ", matrix[i][j]);
        }
        fprintf(file_write, "\n");
    }

    sem_post(&lock);
    pthread_exit(NULL);
}

/**
 * Função responsável por ordenar os números do array em ordem crescente.
 * @param array: representa um array de inteiros
 * @param size: representa o tamanho do array
 * @return o array ordenado
 * **/
int * sort_array(int * array, int size) {
    int aux = 0;
    for(int i=0; i<size; i++) {
        for(int j=i+1; j<size; j++) {
            if(array[i] > array[j]) {
                aux = array[i];
                array[i] = array[j];
                array[j] = aux;
            }
        }
    }

    return array;
}

/**
 * Função responsável por verificar qual o maior número de colunas dos arquivos lidos, 
 * para que possa ser definido qual será a quantidade de colunas na matriz.
 * @param arrayData: representa a struct ArrayDataManager
 * @param qtdFiles: representa a quantidade de arquivos passados
 * @return um inteiro representado a maior coluna
 * **/
int verify_major_column(ArrayDataManager * arrayData, int qtdFiles) {
    int col = 0;

    for(int i=0; i<qtdFiles; i++) {
        if(arrayData[i].top > col) {
            col = arrayData[i].top; 
        }   
    }
    return col;
}

/**
 * Função principal do programa responsável por realizar a leitura dos arquivos,
 * chamar as funções e criar as threads.
 * @param argc: representa a quantidade de argumentos passados na linha de comando 
 * para a execução do programa
 * @param argv: array que representa os argumentos passados.
 * **/
int main(int argc, char ** argv) {
    
    // Inicialiando variáveis
    int number_of_threads = atoi(argv[1]);
    char * file_name_out = argv[argc-1];
    int number_read = 0;
    int qtdLines = argc-3;

    clock_t start_time, end_time;
    start_time = clock();

    ArrayDataManager arrayData[qtdLines];

    int cont_files = 0;
    
    /* Primeira leitura dos arquivos para criação das structs ArrayDataManager
       e atribuição ao top
    */
    for (int i=2; i<argc-1; i++) {
        FILE * file = fopen(argv[i], "r");
        int size_array = 0;

        if(file == NULL)
            return EXIT_FAILURE;

        while(fscanf(file, "%d", &number_read) == 1){
            size_array++;
        }

        arrayData[cont_files].top = size_array;
        cont_files++;
    }

    int major_col = verify_major_column(arrayData, argc-3);
    cont_files = 0;

    /* Segunda leitura dos arquivos para armazenamento dos números
       lidos nas structs criadas e logo após ordenando os arrays.
    */
    for (int i=2; i<argc-1; i++) {
        FILE * file = fopen(argv[i], "r");
        int cont = 0;

        int * array = (int *) malloc(sizeof(int) * major_col);

        while(fscanf(file, "%d", &number_read) == 1){
            array[cont] = number_read;
            cont++;
        }

        sort_array(array, cont);

        while(cont < major_col) {
            array[cont] = 0;
            cont++;
        }

        arrayData[cont_files].array = array;
        cont_files++;
    }

    /* Alocação da matriz a serem armazenados os números das structs */
    matrix = (int **)malloc(qtdLines * sizeof(int*));
    for(int i = 0; i < qtdLines; i++) matrix[i] = (int *)malloc(major_col * sizeof(int));

    /* Inserindos os valores das structs na matriz */
    for(int i=0; i<qtdLines; i++) {
        for(int j=0; j<major_col; j++) {
            matrix[i][j] = arrayData[i].array[j];
        }
    }

    file_write = fopen(file_name_out, "w");
    int qtdThreadsWillUse = 0;

    /* Validação dos números de threads a serem utilizadas */
    if(number_of_threads >= qtdLines) qtdThreadsWillUse = qtdLines;
    else qtdThreadsWillUse = number_of_threads;

    // Inicializando o semáforo lock
    sem_init(&lock, 0, 1);
    pthread_t outrosTIDs[qtdThreadsWillUse];
    Data array_data_thread[qtdThreadsWillUse];

    /*  Loop para criação das threads definindo a linha inical de leitura
        e até a linha final
     */
    for(int i=0; i<qtdThreadsWillUse; i++) {
        int limit = 0;
        int line = 0;

        limit = (i + 1) * (qtdLines / qtdThreadsWillUse) - 1;
        line = i * (qtdLines / qtdThreadsWillUse);

        if(i+1 == qtdThreadsWillUse){
            limit = qtdLines - 1;
            line = i * (qtdLines / qtdThreadsWillUse);
        }

        array_data_thread[i].limit_col = major_col;
        array_data_thread[i].line = line;
        array_data_thread[i].limit_line = limit;

        pthread_create(&outrosTIDs[i], NULL, build_matrix_in_file, &array_data_thread[i]);
        printf("CREATING THREAD (id = %ld)\n", outrosTIDs[i]);        
    }

    /* Join das threads */
    for(int i=0; i<qtdThreadsWillUse; i++)
        pthread_join(outrosTIDs[i], NULL);
        
    // Destruindo o semárofo criado
    sem_destroy(&lock); 

    /* Cálculo do tempo total do programa */
    end_time = clock();
    double diff_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("TEMPO TOTAL: %0.4f\n", diff_time);
   
    return 0;
}