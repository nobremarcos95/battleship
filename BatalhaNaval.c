#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//protótipos das funcoes
void menu();
int** newTable();
int getPosition(int **tab, int posX, int posY, int direction);
int** fileTable();
void playGame(int **keyTable);
void removen(char *pChar, int size);
int printVoidTable();
void printActualTable(int** actualTab, int** keyTab, int positionX, int positionY, int tam);
int checkwin(int **actualTab, int** keyTab, int tam);


int vrfTable = 0;


int main()
{
    menu();
    return 0;
}


//menu de opcoes
void menu()
{
    int op, **table2;
    do
    {
        system("cls");
        printf("\n");
        printf("\t(1) Gerar novo tabuleiro\n");
        printf("\t(2) Usar tabuleiro do arquivo\n");
        printf("\t(3) Jogar\n");
        printf("\t(4) Sair\n");

        printf("\nEscolha uma opcao: ");
        fflush(stdin);
        scanf("%d", &op);

        switch(op)
        {
        case 1:
            table2 = newTable();
            vrfTable = 1;
            break;
        case 2:
            table2 = fileTable();
            vrfTable = 2;
            break;
        case 3:
            if (vrfTable == 1 || vrfTable == 2)
            {
                playGame(table2);
            }
            else
            {
                system("cls");
                printf("\n");
                printf("Tabuleiro chave ainda nao foi criado.\n");
                printf("Por favor, crie um novo tabuleiro antes de jogar.\n");
            }
            break;
        case 4:
            break;
        default:
            system("cls");
            printf("\n");
            printf("Opcao invalida.\n");
            break;
        }
        system("pause");
    } while (op != 4);
    free(table2);
}


//cria um novo tabuleiro chave
int** newTable()
{
    int numberShips; //number of ships
    int vrfPos = 2;
    int **table;
    int direcao, posicaoX, posicaoY;
    int number; //numero qualquer gerado randomicamente

    srand(time(NULL));

    printf("Informe a quantidade de navios desejada. ");
    printf("\nMinimo: 3\n");
    fflush(stdin);
    scanf("%d", &numberShips);

    table = (int **) calloc(numberShips*numberShips, sizeof(int *));
    for (int i = 0; i < (numberShips*numberShips); i++)
    {
        table[i] = (int *) calloc(numberShips*numberShips, sizeof(int));
    }
    if (!table)
    {
        printf("Erro na criacao do tabuleiro.\n");
        system("PAUSE");
        exit(1);
    }

    number = rand() % 100;
    if ((number % 2) == 0)
    {
        direcao = 1; //horizontal
    }
    else
    {
        direcao = 2; //vertical
    }

    while (vrfPos != 0)
    {
        posicaoX = rand() % (numberShips*numberShips);
        while (posicaoX >= (numberShips*numberShips - 2))
        {
            posicaoX = rand() % (numberShips*numberShips);
        }

        posicaoY = rand() % (numberShips*numberShips);
        while (posicaoY >= (numberShips*numberShips - 2))
        {
            posicaoY = rand() % (numberShips*numberShips);
        }

        vrfPos = getPosition(table, posicaoX, posicaoY, direcao);
    }

    if (direcao == 1) //horizontal
    {
        table[posicaoX][posicaoY] = 1;
        table[posicaoX+1][posicaoY] = 1;
        table[posicaoX+2][posicaoY] = 1;
    }
    else //vertical
    {
        table[posicaoX][posicaoY] = 1;
        table[posicaoX][posicaoY+1] = 1;
        table[posicaoX][posicaoY+2] = 1;
    }
    return table;
}


//verifica se ja existe um navio na posicao indicada
int getPosition(int **tab, int posX, int posY, int direction)
{
    if (direction == 1) //horizontal
    {
        if (tab[posX][posY] == 1)
        {
            return 1;
        }
        else if (tab[posX+1][posY] == 1)
        {
            return 1;
        }
        else if (tab[posX+2][posY] == 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else //vertical
    {
        if (tab[posX][posY] == 1)
        {
            return 1;
        }
        else if (tab[posX][posY+1] == 1)
        {
            return 1;
        }
        else if (tab[posX][posY+2] == 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}


//le um tabuleiro chave de um arquivo
int** fileTable()
{
    char fileName[30], caracter[3];
    int size;

    printf("Informe o nome do arquivo a ser lido: ");
    fflush(stdin);
    fgets(fileName, sizeof(fileName), stdin);
    removen(fileName, sizeof(fileName));

    printf("Informe o tamanho do tabuleiro: ");
    fflush(stdin);
    scanf("%d", &size);

    int **table = (int **) calloc(size*size, sizeof(int *));
    for (int i = 0; i < (size*size); i++)
    {
        table[i] = (int *) calloc(size*size, sizeof(int));
    }
    if(!table)
    {
        printf("Erro na criacao do tabuleiro.\n");
        system("PAUSE");
        exit(1);
    }

    FILE *fp = fopen(fileName, "r");
    if (!fp)
    {
        printf("Erro na leitura do arquivo.\n");
        system("PAUSE");
        exit(1);
    }

    for (int i = 0; i < (size*size); i++)
    {
        for (int j = 0; j < (size*size); j++)
        {
            fgets(caracter, sizeof(caracter), fp);
            removen(caracter, sizeof(caracter));
            table[i][j] = atoi(caracter);
        }
    }

    fclose(fp);
    return table;
}


void playGame(int **keyTable)
{
    int win, positionX, positionY;
    int numTorpedos, size;
    char resp;

    size = printVoidTable();
    numTorpedos = ((size*size*size*size)/2.0) * 0.7;

    int **actual = (int **) calloc(size*size, sizeof(int *));
    for (int i = 0; i < size*size; i++)
    {
        actual[i] = (int *) calloc(size*size, sizeof(int));
    }
    do
    {
        system("cls");
        printf("Insira a linha e a coluna desejadas: ");
        fflush(stdin);
        scanf("%d %d", &positionX, &positionY);
        while ((positionX < 0) || (positionX > size*size) || (positionY < 0) || (positionY > size*size))
        {
            system("cls");
            printf("Posicoes invalidas.\n");
            printf("Tente novamente.\n");
            fflush(stdin);
            scanf("%d %d", &positionX, &positionY);
        }
        numTorpedos--;
        printActualTable(actual, keyTable, positionX, positionY, size);
        printf("\nTorpedos restantes: %d\n", numTorpedos);
        if (numTorpedos == 0)
        {
            printf("\nSeus torpedos acabaram.\n>> FIM DE JOGO. <<\n");
        }
        win = checkwin(actual, keyTable, size);
        if (win == 1)
        {
            printf("PARABENS! VOCE GANHOU!\n>> FIM DE JOGO. <<\n");
        }
        system("pause");
    } while ((win == 0) && (numTorpedos > 0));
    free(actual);

    printf("\n");
    printf("Deseja jogar com o mesmo tabuleiro chave? (S/N)\n");
    fflush(stdin);
    scanf("%c", &resp);
    switch(resp)
    {
    case 's':
    case 'S':
        break;
    case 'n':
    case 'N':
        free(keyTable);
        vrfTable = 0;
        break;
    default:
        printf("Opcao invalida.\n");
        break;
    }
}


//remove o \n ou 'espaço' da string
void removen(char *pChar, int size)
{
    for (int i = 0; i < size; i++)
    {
        if ((pChar[i] == '\n') || (pChar[i] == ' '))
        {
            pChar[i] = '\0';
        }
    }
}


int printVoidTable()
{
    int tam;

    printf("Informe o tamanho do tabuleiro: ");
    fflush(stdin);
    scanf("%d", &tam);

    int **table = (int **) calloc(tam*tam, sizeof(int *));
    for (int i = 0; i < (tam*tam); i++)
    {
        table[i] = (int *) calloc(tam*tam, sizeof(int));
    }
    if (!table)
    {
        printf("Erro na criacao do tabuleiro.\n");
        system("PAUSE");
    }
    else
    {
        printf("\n");
        for (int i = 0; i < (tam*tam); i++)
        {
            for (int j = 0; j < (tam*tam); j++)
            {
                if (j == tam*tam - 1)
                {
                    printf("\t\t%d\n", table[i][j]);
                }
                else
                {
                    printf("\t\t%d ", table[i][j]);
                }

            }
        }
    }
    return tam;
}


void printActualTable(int** actualTab, int** keyTab, int positionX, int positionY, int tam)
{
    printf("\n");
    for (int i = 0; i < (tam*tam); i++)
    {
        for (int j = 0; j < (tam*tam); j++)
        {
            if (keyTab[positionX][positionY] == 1)
            {
                actualTab[positionX][positionY] = 1;
            }

            if (j == tam*tam - 1)
            {
                printf("\t%d\n", actualTab[i][j]);
            }
            else
            {
                printf("\t%d ", actualTab[i][j]);
            }

        }
    }
}


int checkwin(int** actualTab, int** keyTab, int tam)
{
    int cont = 0;
    for (int i = 0; i < (tam*tam); i++)
    {
        for (int j = 0; j < (tam*tam); j++)
        {
            if ((actualTab[i][j] == keyTab[i][j]) && (keyTab[i][j] == 1))
            {
                cont++;
            }
        }
    }

    if (cont == (tam*3))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
