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
gcc main.c -pthread -o main
./main 2 arq1.dat arq2.dat arq3.dat –o saida.dat
```

Sendo possível inserir N arquivos e o número de threads variando entre 2, 4, 8 e 16.

# Utilitários

No diretório /utils há o programa fill_file.c cujo propósito é a geração de arquivos que podem ser utilizados para realizar testes no programa principal. Na linha de comando para a execução do programa é possível passar como parâmetros a quantidade de números que devem ser gerados (randomicos) e o nome do arquivo. 

Exemplo:

```shell
gcc fill_file.c -o fill_file
./fill_file file1.dat 500
```

# Documentação

Na primeira parte do programa o objetivo é a leitura dos arquivos passados como parâmetro e o armazenamento dos mesmos em arrays para que possam ser ordenados. Tal funcionalidade é feita na thread principal do programa, ou seja, não é feita ainda a distribuição das tarefas para as threads.

O programa possui duas structs que são utilizadas durante a execução, sendo elas a **DATA**
e a **ArrayDataManager**.

<ul>
    <li><b>Data:</b> struct responsável por gerenciar o acesso à matriz pelas threads. Possui atributos que identificam as linhas e as colunas que as threads devem ler para escrever no arquivo final.</li>
    <li><b>ArrayDataManager:</b> struct responsável por armazenar o array que representa os números de um arquivo lido, além disso possui o atributo top que representa a quantidade de números no array. Para cada arquivo lido é criado uma struct para armazenar os números lidos.</li>
</ul>

<h4>Funções do programa</h4>

<ul>
    <li><b>build_matrix_in_file:</b> função responsável por escrever a matriz no arquivo de saída, sendo utilizada para a criação das threads.</li>
    <li><b>sort_array:</b> função responsável por ordenar o array recebido como parâmetro.</li>
    <li><b>verify_major_column:</b> função responsável por verificar qual o maior número de colunas dos arquivos lidos, para que possa ser definido qual será a quantidade de colunas na matriz.</li>
</ul>

Ao final do programa são criadas as threads para escrever a matriz no arquivo de saída, finalizando-o.

# Resultados
No vídeo fornecido com o este relatório foram realizados os testes com o programa utilizando 2, 4, 8 e 16 threads para um total de 2 arquivos, contendo 10000 números e 5000 números respectivamente. O total de números processados foram 20000 pois o primeiro arquivo contém a maior quantidade de nṹmeros, ou seja, a maior quantidade de colunas da matriz. Logo, o segundo arquivo que possui 5000 será preenchido com 0 até chegar nos 10000 números.

Os resultados obtidos foram respectivamente:

Quantidade de Threads | Tempo (segundos)
--------------------- | ----------------
2                     | 0.2302
4                     | 0.2344
8                     | 0.2353
16                    | 0.2388

Gráfico de apresentação dos resultados:
            
![grafico](https://user-images.githubusercontent.com/20498649/68985956-61f63d00-07f9-11ea-8023-02308f457e49.png)

# Conclusão

A realização desse projeto foi uma ótima forma de se entender o funcionamento das threads e sua implementação utilizando a biblioteca PThreads em C. Com os resultados percebe-se que a execução do programa se tornou mais rápida do que se houvesse apenas a thread principal. Através da distribuição das tarefas o programa executou de forma mais rápida. <br/>
Entretanto, com o aumento das threads percebe-se que há uma demora em milésimos de segundos, isso muitas vezes deve-se aos núcleos do computador em questão. Tal aumento fica mais evidente quando o programa roda utilizando 8 e 16 threads.  <br/>
O principal desafio era entender o funcionamento do projeto e sua
estruturação do código do programa para ordenar, inserir na matriz e armazenar os dados no arquivo de saída com as threads em execução.

