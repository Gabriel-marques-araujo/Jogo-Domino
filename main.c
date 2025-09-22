#include "Mainlib.h"

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int escolha;
    while (1)
    {
        srand(time(NULL));
        jogadores[0].maoJogador = malloc(7 * sizeof(pecaDomino));
        jogadores[1].maoJogador = malloc(7 * sizeof(pecaDomino));
        campo.pecas = malloc(0 * sizeof(pecaDomino));
        banco.pecas = malloc(28 * sizeof(pecaDomino));
        banco.tam = 28;
        printf("----------------------------------------------\n");
        printf(" -------- DOMIN0 --------\n");
        printf(" -------- MENU ----------\n");
        wprintf(L"1 - Começar Um Novo Jogo (Dois Jogadores)\n");
        printf("2 - Carregar Um Jogo Salvo\n");
        printf("3 - Regras Do Domino\n");
        printf("4 - Fechar o Jogo\n");

        wprintf(L"Escolha Uma Opção: ");
        while (1) {
            if (scanf("%d", &escolha) != 1 || escolha > 4 || escolha < 1) {
                // Limpar o buffer do teclado
                while (getchar() != '\n');

                printf("Digite uma opção válida: ");
            } else {
                break; // Sair do loop se a entrada for válida
            }
        }
        printf("\n");

// Limpar a tela
        system("cls"); // Use "cls" se estiver no Windows

        while (getchar() != '\n' || escolha > 4 || escolha < 1)
        {
            wprintf(L"Digite uma opcão válida: ");
            scanf("%d", &escolha);
            printf("\n");
        }

        switch (escolha)
        {
            case 1:
                wprintf(L"Começando Um Novo Jogo ..........\n");
                gerador();
                embaralhador();
                distribuirMao(0);
                distribuirMao(1);
                JogadorAtual = pecaInicial();
                break;

            case 2:
                printf("Carregando Um Jogo Salvo ......\n");
                carregarJogo();
                break;

            case 3:
                printf("\n");
                wprintf(L"* O jogo começa com um conjunto de 28 peças únicas e diferentes, variando de [0|0] até [6|6].\n");
                printf("\n");
                wprintf(L"* Cada jogador começa comprando 7 peças.\n");
                printf("\n");
                wprintf(L"* O jogador inicial é o que possui a dupla mais alta. Se nenhum jogador possuir uma dupla, o que tiver a maior soma inicia. Em caso de empate na soma, o jogador com a peça de maior número em sua composição começa. \n");
                printf("\n");
                wprintf(L"* Em cada vez, o jogador deve realizar uma das três ações: jogar uma peça de sua mão, comprar peças da mesa até que seja possivel  jogar ou passar a vez, se todas as peças do banco estiverem esgotadas.\n");
                printf("\n");
                wprintf(L"* As regras para jogar uma peça da mão para a mesa são as seguintes: é permitido jogar peças que tenham um lado com o mesmo número do lado esquerdo da peça mais a esquerda jogada ou um lado com o mesmo número do lado direito da peça mais a direita. Se ambas as possibilidades forem o mesmo numero, o jogador pode escolher em que lado quer jogar a pe�a.\n");
                printf("\n");
                wprintf(L"* Após a jogada, a peça jogada deve se conectar a peça que estava em mesa pelo lado com o número em comum,  formando assim  uma sequência linear de pe�as.\n");
                printf("\n");
                system("pause");
                system("cls");
                continue;

            case 4:
                printf("Saindo Do Jogo\n");
                exit(0);
            default:
                wprintf(L"Opcão Invalida. Tente Novamente.\n");
                break;
        }

        if (JogadorAtual == 1)
        {
            adversario = 1;
        }
        else
        {
            adversario = 2;
        }
        system("pause");
        system("cls");
        int opcao, peca, qualLado;
        while (1)
        {
            printf("MESA ATUAL\n");
            imprimirCampo();
            printf("\n");
            imprimirMao(JogadorAtual);
            printf("\n");
            wprintf(L"1 - Jogar uma Peça\n");
            printf("2 - Comprar do Banco (Qtd: %d)\n", banco.tam);
            wprintf(L"3 - Passar Vez (Qnt de peças do adversário: %d)\n", jogadores[adversario - 1].tamMao);
            printf("4 - Salvar Partida\n");
            printf("5 - Voltar ao Menu Inicial\n");

            wprintf(L"Digite uma opção de escolha para fazer: ");
            while (1) {
                if (scanf("%d", &opcao) != 1 || opcao > 5 || opcao < 1) {
                    // Limpar o buffer do teclado
                    while (getchar() != '\n');

                    wprintf(L"Opção inválida. Pressione Enter para tentar novamente.\n");
                    // Pausar até que o usuário pressione Enter
                    while (getchar() != '\n');
                    // Imprimir novamente a mensagem de escolha
                    wprintf(L"Digite uma opção de escolha para fazer: ");
                }       else {
                    break; // Sair do loop se a entrada for válida
                }

            }

            while (getchar() != '\n' || opcao > 6 || opcao < 1)
            {
                wprintf(L"\nEntrada inválida. Tente novamente.\n");
                system("pause");
                break;
            }

            if (opcao == 5)
            {
                printf("1");
                break;
            }
            if (opcao == 90)
            {
                wprintf(L"Código de parada\n");
                system("cls");
                exit(1);
            }
            if (opcao == 6)
            {
                wprintf(L"Você escolheu a opção de hack, vc é um MERDA!!!\n");
                system("pause");
                jogadores[JogadorAtual].tamMao = 1;
            }

            switch (opcao)
            {
                case 1:
                    wprintf(L"Escolha uma peça para jogar: ");
                    scanf("%d", &peca);

                    while (getchar() != '\n' || peca + 1 > jogadores[JogadorAtual].tamMao || peca < 0)
                    {
                        wprintf(L"Digite uma opcão de peça válida: ");
                        scanf("%d", &peca);
                        printf("\n");
                    }

                    printf("Escolha um lado (0->esquerda)(1->direita): ");
                    scanf("%d", &qualLado);
                    int jogada = podeJogar(peca, JogadorAtual, qualLado);
                    if (jogada == 0)
                    {
                        break;
                    }
                    if (jogadores[JogadorAtual].tamMao == 0)
                    {
                        printf("Jogador %i venceu\n", JogadorAtual + 1);
                        system("pause");
                        system("cls");
                        exit(0);
                    }
                    if (JogadorAtual == 1)
                    {
                        JogadorAtual = 0;
                        adversario = 2;
                    }
                    else
                    {
                        JogadorAtual = 1;
                        adversario = 1;
                    }
                    break;
                case 2:
                    comprarPecas(JogadorAtual);
                    break;
                case 3:
                    passaVez(&JogadorAtual);
                    break;
                case 4:
                    salvarJogo();
                    printf("O jogo foi salvo\n");
                    system("pause");
                    break;
                default:
                    wprintf(L"Digite uma opcão de escolha válida!\n");
                    break;
            }
            system("cls");
        }
        campo.tam = 0;
        jogadores[0].tamMao = 0;
        jogadores[1].tamMao = 0;
        free(jogadores[0].maoJogador);
        free(jogadores[1].maoJogador);
        free(campo.pecas);
        free(banco.pecas);
        system("cls");
    }

    return 0;
}