#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <string.h>

#define FILE_WRITE "out.txt"

pthread_mutex_t lock;

typedef struct DataManager {
    int ** matrix;
    int line;
    int limit;
    int maxCol;
} DataManager;

int * array_qtd_numbers_each_file;
int thread_no = 0; 

void * build_matrix_in_file(void *args) {

    pthread_mutex_lock(&lock);
    DataManager * my_data =  (DataManager *) args;
    FILE * file = fopen(FILE_WRITE, "a");
    fprintf(file, "\n");

    int limit = my_data->limit;
    int line = my_data->line;
    printf("line in thread: %d\n", line);

    for(int i=line; i<limit; i++) {
        for(int j=0; j<my_data->maxCol; j++) {
            fprintf(file, "%d ", my_data->matrix[i][j]);
        }
    }

    
    fclose(file);
    pthread_mutex_unlock(&lock);
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
    
    int number;
    int totalNumbers, cont_numbers, index = 0;
    int number_threads = atoi(argv[1]);

    DataManager dataManager;
    
    array_qtd_numbers_each_file = malloc(sizeof(int) * (argc -2));

    // READING FILES
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
    int maxColumn = verifyColumn(qtdLines);

    dataManager.matrix = (int **)malloc(qtdLines * sizeof(int*));

    for(int i = 0; i < qtdLines; i++) 
        dataManager.matrix[i] = (int *)malloc(maxColumn * sizeof(int));

    //MOUNTING MATRIX
    int line = 0;
    for(int i=2; i<argc; i++) {
        FILE * file = fopen(argv[i], "r");
        int col = 0;
        

        while(fscanf(file, "%d", &number) == 1){
            dataManager.matrix[line][col] = number;
            col++;
        }

        line++;
    }

    //CALLING THREADS
    pthread_t outrosTIDs[number_threads];
    DataManager array_dm[qtdLines];

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    for(int i=0; i<number_threads; i++) {
        array_dm[i] = dataManager;
        int limit = (thread_no + 1) * (qtdLines / number_threads);
        int line = thread_no * (qtdLines / number_threads);

        array_dm[i].line = line;
        array_dm[i].limit = limit;
        array_dm[i].maxCol = maxColumn;

        pthread_create(&outrosTIDs[i], NULL, build_matrix_in_file, &array_dm[i]);
        printf("CREATING THREAD (id = %ld)\n", outrosTIDs[i]);
        thread_no++;
    }

    for(int i=0; i<number_threads; i++)
        pthread_join(outrosTIDs[i], NULL);

    pthread_mutex_destroy(&lock);

    
    return 0;
}