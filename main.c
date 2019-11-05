#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h> 
#include <time.h>

#define FILE_WRITE "out.txt"

sem_t lock; 

pthread_mutex_t mutex;
FILE * file_write;

typedef struct DataManager {
    int ** matrix;
    int line;
    int limit;
    int maxCol;
    int break_line;
} DataManager;

int array[1000000];
int * array_qtd_numbers_each_file;
int thread_no, totalReal, maxColumn, nextBreak = 0; 

void * build_matrix_in_file(void *args) {
    DataManager * my_data =  (DataManager *) args;

    int limit = my_data->limit;
    int line = my_data->line;
    int comp = line;

    if(line == 1) 
        fprintf(file_write, "%d ", array[0]);
    
    printf("LINE: %d, LIMIT: %d, BREAK: %d\n", line, limit, my_data->break_line);
    for(int i=line; i<=limit; i++) {
        printf("I: %d -> LIMIT: %d -> ARRAY POS: %d \n", i, limit, array[i]);
        if(i == my_data->break_line) {
            fprintf(file_write, "\n");
        }
        if(i < totalReal)
            fprintf(file_write, "%d ", array[i]);
    }

    printf("\n");
    pthread_exit(NULL);
    
}

int verifyColumn(int qtdFiles) {
    int col = 0;

    for(int i=0; i<qtdFiles; i++) {
        if(array_qtd_numbers_each_file[i] > col)
            col = array_qtd_numbers_each_file[i];
    }

    return col;
}

int main(int argc, char ** argv) {
    time_t start_time = time(NULL);
    int number;
    int totalNumbers = 0;
    int cont_numbers, index = 0;
    int number_threads = atoi(argv[1]);

    DataManager dataManager;
    
    array_qtd_numbers_each_file = malloc(sizeof(int) * (argc -2));
    // READING FILES
    int c =0;
    for (int i=2; i<argc; i++) {
        FILE * file = fopen(argv[i], "r");
        cont_numbers=0;

        if(file == NULL)
            return EXIT_FAILURE;

        while(fscanf(file, "%d", &number) == 1){
            cont_numbers++;
            totalNumbers++;
        }

        array_qtd_numbers_each_file[index] = cont_numbers;
        index++; 
    }

    int qtdLines = argc-2;
    maxColumn = verifyColumn(qtdLines);

    for (int i=2; i<argc; i++) {
        FILE * file = fopen(argv[i], "r");
        int contador = 0;
        if(file == NULL)
            return EXIT_FAILURE;

        while(fscanf(file, "%d", &number) == 1){
            array[c] = number;
            c++;
            contador++;
            totalReal++;
        }

        while(contador < maxColumn) {
            array[c] = 0;
            contador++;
            totalReal++;
            c++;
        }
    }

    //CALLING THREADS
    pthread_t outrosTIDs[number_threads];
    DataManager array_dm[number_threads];

    pthread_mutex_init(&mutex, NULL);

    file_write = fopen(FILE_WRITE, "w+");

    int newLimit = 0;

    int auxBreak = maxColumn * (thread_no + 1);
    nextBreak = auxBreak;
    int resto = 0;

    printf("TOTAL: %d\n", totalReal);

    //calculando mod
    if(totalReal % number_threads != 0) {
        resto = totalReal % number_threads;
    }

    for(int i=0; i<number_threads; i++) {
        array_dm[i] = dataManager;

        int line = newLimit;
        newLimit += totalReal/number_threads;
        
        int limit = (i + 1) * (totalReal/number_threads);

        if(resto != 0 && number_threads == i+1) { 
            limit = limit + resto; 
        }

        array_dm[i].line = line+1;
        array_dm[i].limit = limit;
        array_dm[i].maxCol = maxColumn;

        //não vai ter quebra de linha
        if(limit < nextBreak) {
             array_dm[i].break_line = auxBreak;
             nextBreak = auxBreak;
             thread_no++;

        } else if(limit >= nextBreak){
            thread_no++;

            array_dm[i].break_line = auxBreak;
            nextBreak += maxColumn;
            auxBreak = nextBreak;
        }

        pthread_create(&outrosTIDs[i], NULL, build_matrix_in_file, &array_dm[i]);
        pthread_join(outrosTIDs[i], NULL);
        printf("CREATING THREAD (id = %ld)\n", outrosTIDs[i]);
    }

    time_t end_time = time(NULL);

    printf("TOTAL NUMEROS: %d\n", totalReal);
    printf("TEMPO TOTAL: %ld segundos", (end_time- start_time));
    
    return 0;
}