#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int ** matrix;

typedef struct arrayData {
    char fileName[200];
    int * arrayNumberReaded;
    int top;
} ArrayData;

typedef struct data_manager {
    ArrayData ** array_data;
    int top;
} DataManager;

ArrayData * newArrayData(int size, int * array, char * filename) {
    ArrayData * arrayData = (ArrayData *) malloc(sizeof(ArrayData));

    strcpy(arrayData->fileName, filename);
    arrayData->arrayNumberReaded = malloc(sizeof(int) * size);
    arrayData->top = size;
    arrayData->arrayNumberReaded = array;

    return arrayData;
}

DataManager * newDataManager(int size) {
    DataManager * dataManager = (DataManager *) malloc(sizeof(DataManager));

    dataManager->array_data = malloc(sizeof(ArrayData) * size);
    dataManager->top = size;
    
    return dataManager;
}

void addArrayData(ArrayData * arrayData, DataManager * dataManager, int index) {
    printf("qtd %d\n", index);
    dataManager->array_data[index] = arrayData;
}

int main(int argc, char ** argv) {
    int numbers;
    int cont = 0;
    int qtdFiles = 0;

    DataManager * dataManager = newDataManager(argc-1);
    
    for (int i=1; i<argc; i++) {
        FILE * file = fopen(argv[i], "r");
        cont = 0;
        int array[10000];

        if(file == NULL)
            return EXIT_FAILURE;

        
        while(fscanf(file, "%d", &numbers) == 1) {
            array[cont] = numbers;
            cont++;
        }

        ArrayData * arrayData = newArrayData(cont, array, argv[i]);  
        dataManager->array_data[qtdFiles] = arrayData; 

        //addArrayData(arrayData, dataManager, qtdFiles);

        qtdFiles++;
        fclose(file);
    }

    for(int i=0; i<dataManager->array_data[0]->top; i++) {
            printf("%d b\n", dataManager->array_data[0]->arrayNumberReaded[i]);
        }


    return 0;
}