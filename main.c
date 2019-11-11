#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h> 
#include <time.h>


int ** matrix;
FILE * file_write;
sem_t lock; 

typedef struct Data {
    int line;
    int limit_line;
    int limit_col;
} Data;

typedef struct ArrayDataManager {
    int * array;
    int top;
} ArrayDataManager;


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

int verify_major_column(ArrayDataManager * arrayData, int qtdFiles) {
    int col = 0;

    for(int i=0; i<qtdFiles; i++) {
        if(arrayData[i].top > col) {
            col = arrayData[i].top; 
        }   
    }
    return col;
}


int main(int argc, char ** argv) {
    int number_of_threads = atoi(argv[1]);
    char * file_name_out = argv[argc-1];
    int number_read = 0;
    int qtdLines = argc-3;

    clock_t start_time, end_time;
    start_time = clock();

    ArrayDataManager arrayData[argc-3];

    int cont_files = 0;
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


    matrix = (int **)malloc(qtdLines * sizeof(int*));
    for(int i = 0; i < qtdLines; i++) matrix[i] = (int *)malloc(major_col * sizeof(int));

    for(int i=0; i<qtdLines; i++) {
        for(int j=0; j<major_col; j++) {
            matrix[i][j] = arrayData[i].array[j];
        }
    }

    file_write = fopen(file_name_out, "w");
    int qtdThreadsWillUse = 0;

    if(number_of_threads >= qtdLines) qtdThreadsWillUse = qtdLines;
    else qtdThreadsWillUse = number_of_threads;

    sem_init(&lock, 0, 1);
    pthread_t outrosTIDs[qtdThreadsWillUse];
    Data array_data_thread[qtdThreadsWillUse];

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

    for(int i=0; i<qtdThreadsWillUse; i++)
        pthread_join(outrosTIDs[i], NULL);
        
    sem_destroy(&lock); 

    end_time = clock();
    double diff_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("TEMPO TOTAL: %0.4f\n", diff_time);
   
    return 0;
}