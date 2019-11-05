#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {

    FILE * file = fopen(argv[1], "w");
    int qtdNumber = atoi(argv[2]);

    for(int i=0; i<qtdNumber; i++) {
        fprintf(file, "%d", i+1);

        if(i < qtdNumber)
            fprintf(file, "\n");
    }

    fclose(file);

    return 0;

}