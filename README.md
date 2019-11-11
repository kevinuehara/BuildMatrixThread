# BuildMatrixThread
Repositório utilizado para o projeto da matéria de Sistemas Operacionas ministrada pelo professor André Leon Grandvohl.

# Objetivo
Este projeto visa a criação de um programa que utilize múltiplas threads para ler valores inteiros de N arquivos de entrada e armazenar em um único arquivo de saída no formato de matriz (linhas e colunas). O programa deverá ser escrito para o sistema operacional Linux e obrigatoriamente utilizar a biblioteca POSIX Threads.

Considere que existem N arquivos de entrada com diferentes quantidades de valores inteiros que podem ser repetidos ou não. O programa deverá ler os valores inteiros desses vários arquivos e, de forma ordenada, armazená-los em um arquivo de saída no formato de matriz com N linhas (onde N é a quantidade de arquivos) e M colunas (onde M é a maior quantidade de valores entre os N arquivos). As posições da matriz devem ser preenchidas com zero, quando necessário.

# Como rodar?
```shell
./main {number_of_threads} arq1.dat arq2.dat arq3.dat –o {file_out.dat}
```

Exemplo:

```shell
gcc main.c - pthread -o main
./main 2 arq1.dat arq2.dat arq3.dat –o saida.dat
```

Sendo o possível inserir N arquivos e o número de threads variando entre 2, 4, 8 e 16.