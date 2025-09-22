#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

typedef struct
{
    int esquerda;
    int direita;
} pecaDomino;

typedef struct
{
    pecaDomino *maoJogador;
    int tamMao;
} jogador;

typedef struct
{
    pecaDomino *pecas;
    int tam;
} bancoPecas;

jogador jogadores[2];
bancoPecas banco;
bancoPecas campo;

int JogadorAtual, adversario;

void gerador()
{
    // Função responsável por gerar todas as peças do jogo
    int indice = 0; // Inicializa variável 'indice' que será¡ usada para percorrer todos os elementos do vetor recebido como parâmetro
    for (int i = 0; i <= 6; i++)
    { // Inicia o laço de fora com i=0, que atribuirá¡ os valores de 0 a 6 aos '.esquerda' das peças
        for (int j = i; j <= 6; j++)
        {                                     // Inicia o laço de dentro com j=i, para, dessa forma, não gerar peças repetidas. Pois, 'j' sempre começará¡ em i
            banco.pecas[indice].esquerda = i; // Atribui ao '.esquerda' do elemento atual o valor do 'i' atual
            banco.pecas[indice].direita = j;  // Similar ao anterior, atribui, agora, o '.direita'
            indice++;                         // Incrementa o 'indice', apÃ³s cada atribuição completa de peça, para preencher todo o vetor
        }
    }
}

void embaralhador()
{
    // Função reponsável por embaralhar todas as peças do jogo
    for (int i = 0; i < 28; i++)
    {                                            // Inicia o laço de fora que percorrerá¡ todo o vetor recebido como parâmetro
        int aleatorio = rand() % 27;             // Atribui à variável 'aleatorio', um índice aleatório do vetor parâmetro
        pecaDomino temp = banco.pecas[i];        // Atribui à temporária, o elemento atual determinado pelo laço
        banco.pecas[i] = banco.pecas[aleatorio]; // Atribui ao elemento atual outro elemento, de í­ndice 'aleatorio'
        banco.pecas[aleatorio] = temp;           // Atribui a esse elemento[aleatorio] o valor contido em 'temp'
    }
}

void imprimirMao(int idJogador)
{
    // Função responsável por imprimir no console as peças do jogador selecionado
    int tamMao = jogadores[idJogador].tamMao;     // Inicializa a variável 'tamMao' com o valor do tamanho da mão do jogador atual, para facilitar o manuseio
    wprintf(L"Mão do jogador %i\n", idJogador + 1); // Imprime na tela a mensagem indicando de qual jogador é a mão impressa
    // Vale ressaltar que deve-se somar +1 ao índice 'idJogador', pois o mesmo varia de 0-1
    if (idJogador == 0)
    {
        for (int i = 0; i < tamMao; i++)
        {
            printf("%i: [%i|%i] ", i, jogadores[idJogador].maoJogador[i].esquerda, jogadores[idJogador].maoJogador[i].direita);
        }
    }

    if (idJogador == 1)
    {
        for (int i = 0; i < tamMao; i++)
        {
            printf("%i: [%i|%i] ", i, jogadores[idJogador].maoJogador[i].esquerda, jogadores[idJogador].maoJogador[i].direita);
        }
    }
    printf("\n");
}

void removerPeca(pecaDomino *vetor, int *tam)
{
    // Função responsável por diminuir a quantidade de peças
    (*tam)--; // Diminui o conteúdo apontado por '*tam' em -1

    if (*tam == 0) // Se o tamanho for reduzido a 0, libera memória alocada pelo ponteiro
    {
        free(vetor);
    }
    else if (vetor != NULL) // Caso não, realoca a memória alocada pelo ponteiro, agora com o tamanho reduzido
    {
        realloc(vetor, (*tam) * sizeof(pecaDomino));
    }
}

void diminuirMao(int idPeca, int idJogador)
// Função responsável remover a peça jogada por um jogador e reduzir o tamanho da mão do mesmo
{
    for (int i = idPeca; i < jogadores[idJogador].tamMao; i++) // Loop responsável por copiar todas as peças, a partir da posição seguinte da peça jogada, para uma posição anterior
    {
        jogadores[idJogador].maoJogador[i] = jogadores[idJogador].maoJogador[i + 1];
    }
    removerPeca(jogadores[idJogador].maoJogador, &jogadores[idJogador].tamMao); // Chama a função para remover a última peça do vetor mão, que é uma cópia da penúltima
}

int podeJogar(int idPeca, int idJogador, int qualLado)
// Função responsável por avaliar as condições e determinar se é possível jogar uma peça no campo
{
    if (qualLado != 0 && qualLado != 1)
    { // Condicional que avalia se a opção selecionada é diferente das opções válidas
        printf("Lado inválido, Digite uma opção disponível\n");
        system("pause"); // Caso não, escreve a mensagem na tela e termina a execução da função
        return 0;
    }
    if (qualLado == 0)
    {                // Jogar peça na ESQUERDA do campo
        campo.tam++; // Aumenta em +1 o valor da variável e realoca memória, de acordo com o novo tamanho
        campo.pecas = realloc(campo.pecas, campo.tam * sizeof(pecaDomino));
        if (campo.pecas[0].esquerda == jogadores[idJogador].maoJogador[idPeca].direita)
        { // Peça do jogador encaixa normalmente no campo
            for (int i = campo.tam - 2; i >= 0; i--)
            {
                campo.pecas[i + 1].esquerda = campo.pecas[i].esquerda;
                campo.pecas[i + 1].direita = campo.pecas[i].direita;
            } // Loop para copiar todas as peças do campo para uma posição a frente
            campo.pecas[0] = jogadores[idJogador].maoJogador[idPeca];
            // Após isso, a peça jogada foi copiada para a posição 0 do campo
            // CHAMAR A FUNÇÃO DE REMOVER A PEÇA DA MÃO
            diminuirMao(idPeca, idJogador);
            // Copia a peça jogada para a posição vazia mais à direita do campo, determinada pelo loop anterior
        }
        else
        {
            if (campo.pecas[0].esquerda == jogadores[idJogador].maoJogador[idPeca].esquerda)
            { // Peça do jogador deve ser invertida para encaixar
                for (int i = campo.tam - 2; i >= 0; i--)
                {
                    campo.pecas[i + 1].esquerda = campo.pecas[i].esquerda;
                    campo.pecas[i + 1].direita = campo.pecas[i].direita;
                } // Loop para copiar todas as peças do campo para uma posição a frente
                campo.pecas[0].esquerda = jogadores[idJogador].maoJogador[idPeca].direita;
                campo.pecas[0].direita = jogadores[idJogador].maoJogador[idPeca].esquerda; // Inverte a peça jogada e copia na posição 0 do campo
                // Após isso, a peça jogada foi copiada para a posição 0 do campo
                // CHAMAR A FUNÇÃO DE REMOVER A PEÇA DA MÃO
                diminuirMao(idPeca, idJogador);
                // Copia a peça jogada para a posição vazia mais à direita do campo, determinada pelo loop anterior
            }
            else
            { // Caso a peça não possa ser jogada na esquerda, diminui o valor da variável em -1 e realoca memória
                campo.tam--;
                campo.pecas = realloc(campo.pecas, campo.tam * sizeof(pecaDomino));
                printf("Jogada inválida, escolha uma peça correta\n");
                system("pause");
                return 0; // No caso citado, a função retorna 0, para indicar que houve erro na jogada
            }
        }
    }
    else
    {
        if (qualLado == 1)
        { // Jogar peça na DIREITA do campo
            campo.tam++;
            campo.pecas = realloc(campo.pecas, campo.tam * sizeof(pecaDomino));
            if (campo.pecas[campo.tam - 2].direita == jogadores[idJogador].maoJogador[idPeca].esquerda)
            { // Peça encaixa normalmente no campo
                campo.pecas[campo.tam - 1] = jogadores[idJogador].maoJogador[idPeca];
                // Após isso, a peça jogada foi copiada para a posição 0 do campo
                // CHAMAR A FUNÇÃO DE REMOVER A PEÇA DA MÃO
                diminuirMao(idPeca, idJogador);
                // Copia a peça jogada para a posição vazia mais à direita do campo, determinada pelo loop anterior
            }
            else
            {
                if (campo.pecas[campo.tam - 2].direita == jogadores[idJogador].maoJogador[idPeca].direita)
                { // Peça deve ser invertida para encaixar no campo
                    int aux;
                    aux = jogadores[idJogador].maoJogador[idPeca].esquerda;
                    jogadores[idJogador].maoJogador[idPeca].esquerda = jogadores[idJogador].maoJogador[idPeca].direita;
                    jogadores[idJogador].maoJogador[idPeca].direita = aux;
                    campo.pecas[campo.tam - 1] = jogadores[idJogador].maoJogador[idPeca];
                    // Após isso, a peça jogada foi copiada para a posição 0 do campo
                    // CHAMAR A FUNÇÃO DE REMOVER A PEÇA DA MÃO
                    diminuirMao(idPeca, idJogador);
                }
                else
                {
                    campo.tam--; // Caso a peça não possa ser jogada na esquerda, diminui o valor da variável em -1 e realoca memória
                    campo.pecas = realloc(campo.pecas, campo.tam * sizeof(pecaDomino));
                    printf("Jogada invalida, escolha uma peça correta\n");
                    printf("A peça que voçe tentou jogar foi: [%d][%d], comparando com [%d][%d]\n", jogadores[idJogador].maoJogador[idPeca].esquerda, jogadores[idJogador].maoJogador[idPeca].direita, campo.pecas[campo.tam - 1].esquerda, campo.pecas[campo.tam - 1].direita);
                    system("pause");
                    return 0; // No caso citado, a função retorna 0, para indicar que houve erro na jogada
                }
            }
        }
    }
    return 1; // Para o caso da peça ter sido jogada corretamente, a função retorna 1, para indicar que houve sucesso na jogada
}

void comprarPecas(int idJogador)
{
    // Função responsável por executar uma compra de uma peça por parte do jogador
    int tamanMao, tamBanco = banco.tam;
    if (banco.tam == 0)
    {
        printf("O banco esta vazio!");
        return;
    }
    else
    {
        jogadores[idJogador].tamMao++;                                                                             // Aumenta em +1 o valor da variável
        tamanMao = jogadores[idJogador].tamMao;                                                                    // Atribui o valor campo '.tamMao' à variável 'tamanMan', para facilitar o manuseio
        jogadores[idJogador].maoJogador = realloc(jogadores[idJogador].maoJogador, tamanMao * sizeof(pecaDomino)); // Realoca memória do campo '.maoJogador', agora com o valor correspondente da variável 'tamanMao'
        jogadores[idJogador].maoJogador[tamanMao - 1] = banco.pecas[0];                                            // Atribui à peça da última posição da mão do jogador, até então contendo lixo, os valores dos campos do vetor banco[0]
        for (int i = 0; i < tamBanco - 1; i++)
        { // Loop responsável por copiar todas as peças do banco, a partir da posição '1', uma posição anterior
            banco.pecas[i].esquerda = banco.pecas[i + 1].esquerda;
            banco.pecas[i].direita = banco.pecas[i + 1].direita;
        }                                     // Ao fim do loop, a última posição do vetor e a penúltima ficam com valores iguais, ou seja, peças repetidas
        removerPeca(banco.pecas, &banco.tam); // Chama a função para remover a peça repetida do vetor
    }
}

void distribuirMao(int idJogador)
{
    // Função responsável por distribuir as peças, no início do jogo, para a mão dos jogadores
    for (int i = 0; i < 7; i++)
    {
        comprarPecas(idJogador); // Para cada posição da mão inicial do jogador em questão, adiciona uma peça do vetor banco
    }
}

int passaVez(int *idJogador)
{
    // Função responsável por passar a vez de um jogador para outro
    int tamanBanco = banco.tam;
    if (tamanBanco == 0) // Avalia se o tamanho do vetor 'banco' é igual a 0, sendo essa a condição necessária para ser possível passar a vez
    {
        if (*idJogador == 0) // Condicional responsável por avaliar de qual jogador é a rodada
        {
            *idJogador = 1; // Atribui o valor do próximo jogador à variável 'idJogador'
            adversario = 1; // Semelhante ao anterior, agora atribui para o 'adversário'
        }
        else
        {
            *idJogador = 0;
            adversario = 2;
        }
    }
    else
    {
        printf("Puxe até achar uma peça que possa ser jogada. Só pode passar quando as peças do banco chegarem a 0.\n");
        system("pause");
        return 0;
    }
}

void imprimirCampo()
{ // Função responsável por imprimir na tela as peças presentes no campo
    for (int i = 0; i < campo.tam; i++)
    {                                                                       // Laço de repetição que itera todos os elemento do vetor campo
        printf("[%i|%i]", campo.pecas[i].esquerda, campo.pecas[i].direita); // Imprime as peÃ§as presentes no campo até então
    }
}

int pecaInicial()
{ // Função responsável por avaliar as peças dos dois jogadores e determinar qual deles tem a peça de maior valor e, portanto, quem jogará primeiro
    int contador1E = -1, contador1D = -1, indice1;
    int contador2E = -1, contador2D = -1, indice2;
    // Caso 1, para comparar as peças iguais dos jogador e ver qual deles tem a maior
    for (int i = 0; i < 7; i++)
    {
        if (jogadores[0].maoJogador[i].esquerda == jogadores[0].maoJogador[i].direita)
        { // Caso em que o jogador possui peça dupla
            if (jogadores[0].maoJogador[i].esquerda > contador1E)
            {
                contador1E = jogadores[0].maoJogador[i].esquerda; // Armazena o maior valor encontrado nas peças
                contador1D = jogadores[0].maoJogador[i].direita;
                indice1 = i; // Armazena o valor do índice da maior peçar do jogador em questão
            }
        }
        if (jogadores[1].maoJogador[i].esquerda == jogadores[1].maoJogador[i].direita)
        {
            if (jogadores[1].maoJogador[i].esquerda > contador2E)
            {
                contador2E = jogadores[1].maoJogador[i].esquerda;
                contador2D = jogadores[1].maoJogador[i].direita;
                indice2 = i;
            }
        }
    }
    if (contador1D > contador2D) // Condicional que avalia se o jogador 1 possui a maior peça
    {
        printf("O PRIMEIRO jogador tinha a dupla [%d|%d], logo ele comecou jogando!\n", contador1D, contador1D);
        campo.tam++;                                                        // Aumenta o valor da variável
        campo.pecas = realloc(campo.pecas, campo.tam * sizeof(pecaDomino)); // Aumenta o tamanho do campo, inicialamente igual a 0, para o tamanho correspondente
        campo.pecas[0] = jogadores[0].maoJogador[indice1];                  // Atribui à posição 0 do campo o valor da maior peça do jogador
        diminuirMao(indice1, 0);                                            // Chama a função para remover a peça jogada, de acordo com o índice armazenado anteriormente
        return 1;
    }
    else if (contador2D > contador1D)
    {
        printf("O SEGUNDO jogador tinha a dupla [%d|%d], logo ele comecou jogando!\n", contador2D, contador2D);
        campo.tam++;
        campo.pecas = realloc(campo.pecas, campo.tam * sizeof(pecaDomino));
        campo.pecas[0] = jogadores[1].maoJogador[indice2];
        diminuirMao(indice2, 1);
        return 0;
    }

    int maiorJogador1 = -1, maiorJogador2 = -1, soma; // Inicializa variáveis para armazenar a maior soma de peças de cada jogador
    // Verifica a maior soma de peças para ambos os jogadores
    for (int i = 0; i < jogadores[0].tamMao; i++)
    {
        soma = jogadores[0].maoJogador[i].esquerda + jogadores[0].maoJogador[i].direita;
        if (soma > maiorJogador1)
        {
            maiorJogador1 = soma;
        }
        soma = jogadores[1].maoJogador[i].esquerda + jogadores[1].maoJogador[i].direita;
        if (soma > maiorJogador2)
        {
            maiorJogador2 = soma;
        }
    }

    // Caso nenhum jogador tenha peças duplas, compara as maiores somas
    if (maiorJogador1 > maiorJogador2)
    {
        return 0;
    }
    else if (maiorJogador2 > maiorJogador1)
    {
        return 1;
    }
    else
    {
        return 1;
        // Verifica a maior peça entre os jogadores se as somas forem iguais
        // (código similar ao anterior para comparar peças individuais)
    }
}
void salvarJogo()
{
    // Função responsável por salvar o jogo atual, armazenando as informações necessárias em um arquivo '.txt'
    FILE *arquivo = fopen("savegame.txt", "w"); // Inicaliza o ponteiro do arquivo e verifica se foi aberto corretamente
    if (arquivo == NULL)
    {
        printf("Erro ao salvar o jogo.\n");
        return;
    }

    // Salve as informações necessárias no arquivo
    fprintf(arquivo, "%d\n", JogadorAtual);
    fprintf(arquivo, "%d\n", adversario);
    fprintf(arquivo, "%d\n", jogadores[0].tamMao);
    fprintf(arquivo, "%d\n", jogadores[1].tamMao);
    fprintf(arquivo, "%d\n", campo.tam);
    fprintf(arquivo, "%d\n", banco.tam);

    for (int i = 0; i < campo.tam; i++)
    { // Printa no arquivo as peças presentes no campo atual
        fprintf(arquivo, "%d %d\n", campo.pecas[i].esquerda, campo.pecas[i].direita);
    }
    for (int i = 0; i < banco.tam; i++)
    { // Printa no arquivo as peças presentes no banco atual
        fprintf(arquivo, "%d %d\n", banco.pecas[i].esquerda, banco.pecas[i].direita);
    }

    for (int i = 0; i < jogadores[0].tamMao; i++)
    { // Printa no arquivo as peças presentes na mão do PRIMEIRO jogador
        fprintf(arquivo, "%d %d\n", jogadores[0].maoJogador[i].esquerda, jogadores[0].maoJogador[i].direita);
    }
    for (int i = 0; i < jogadores[1].tamMao; i++)
    { // Printa no arquivo as peças presentes na mão do SEGUNDO jogador
        fprintf(arquivo, "%d %d\n", jogadores[1].maoJogador[i].esquerda, jogadores[1].maoJogador[i].direita);
    }

    // Feche o arquivo
    fclose(arquivo);
    printf("Jogo salvo com sucesso.\n");
}
void carregarJogo()
{
    // Função responsável por carregar o jogo, atribuindo os valores lidos do arquivo às variáveis correspondentes do programa
    FILE *arquivo = fopen("savegame.txt", "r");
    if (arquivo == NULL)
    {
        printf("Nenhum jogo salvo encontrado.\n");
        return;
    }
    // Carregue as informações do arquivo
    fscanf(arquivo, "%d\n", &JogadorAtual);
    fscanf(arquivo, "%d\n", &adversario);
    fscanf(arquivo, "%d\n", &jogadores[0].tamMao);
    fscanf(arquivo, "%d\n", &jogadores[1].tamMao);
    fscanf(arquivo, "%d\n", &campo.tam);
    fscanf(arquivo, "%d\n", &banco.tam);
    campo.pecas = realloc(campo.pecas, campo.tam * sizeof(pecaDomino)); // Realoca memória para o vetor 'campo', de acordo com o valor lido do arquivo
    banco.pecas = realloc(banco.pecas, banco.tam * sizeof(pecaDomino)); // Realoca memória para o vetor 'banco', de acordo com o valor lido do arquivo
    for (int i = 0; i < campo.tam; i++)
    {
        fscanf(arquivo, "%d %d\n", &campo.pecas[i].esquerda, &campo.pecas[i].direita); // Atribui às peças lidas ao campo
    }
    for (int i = 0; i < banco.tam; i++)
    {
        fscanf(arquivo, "%d %d\n", &banco.pecas[i].esquerda, &banco.pecas[i].direita); // Agora, ao banco
    }

    for (int i = 0; i < jogadores[0].tamMao; i++)
    {
        fscanf(arquivo, "%d %d\n", &jogadores[0].maoJogador[i].esquerda, &jogadores[0].maoJogador[i].direita); // Às peças do jogador 1
    }
    for (int i = 0; i < jogadores[1].tamMao; i++)
    {
        fscanf(arquivo, "%d %d\n", &jogadores[1].maoJogador[i].esquerda, &jogadores[1].maoJogador[i].direita); // Às peças do jogador 2
    }

    // Feche o arquivo
    fclose(arquivo);
    printf("Jogo carregado com sucesso.\n");
}