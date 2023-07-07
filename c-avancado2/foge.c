#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "foge.h"
#include "mapa.h"
#include "ui.h"

MAPA m;
POSICAO heroi;
int pilula = 0;

int fantasmaVai(int xAtual, int yAtual, int *xDestino, int *yDestino)
{
    int opcoes[4][2] = {
        {xAtual, yAtual + 1},
        {xAtual + 1, yAtual},
        {xAtual, yAtual - 1},
        {xAtual - 1, yAtual}};

    srand(time(0));
    for (int i = 0; i < 10; i++)
    {
        int posicao = rand() % 4;

        if (podeAndar(&m, FANTASMA, opcoes[posicao][0], opcoes[posicao][1]))
        {
            *xDestino = opcoes[posicao][0];
            *yDestino = opcoes[posicao][1];
            return 1;
        }
    }

    return 0;
}

void fantasmas()
{
    MAPA copia;

    copiaMapa(&copia, &m);

    for (int i = 0; i < m.linhas; i++)
    {
        for (int j = 0; j < m.colunas; j++)
        {

            if (copia.matriz[i][j] == FANTASMA)
            {

                int xDestino;
                int yDestino;

                int encontrou = fantasmaVai(i, j, &xDestino, &yDestino);

                if (encontrou)
                {
                    andaMapa(&m, i, j, xDestino, yDestino);
                }
            }
        }
    }
    liberaMapa(&copia);
}

int acabou()
{
    POSICAO pos;
    int fogeFogeMapa = encontraMapa(&m, &pos, HEROI);
    return !fogeFogeMapa;
}

int ehDirecao(char direcao)
{
    return direcao == ESQUERDA ||
           direcao == CIMA ||
           direcao == BAIXO ||
           direcao == DIREITA;
}

void move(char direcao)
{
    if (!ehDirecao(direcao))
        return;

    int proximox = heroi.x;
    int proximoy = heroi.y;

    switch (direcao)
    {
    case ESQUERDA:
        proximoy--;
        break;
    case CIMA:
        proximox--;
        break;
    case BAIXO:
        proximox++;
        break;
    case DIREITA:
        proximoy++;
        break;
    }

    if (!podeAndar(&m, HEROI, proximox, proximoy))
        return;

    if (ehPersonagem(&m, PILULA, proximox, proximoy))
    {
        pilula = 1;
    }

    andaMapa(&m, heroi.x, heroi.y, proximox, proximoy);
    heroi.x = proximox;
    heroi.y = proximoy;
}

void explodePilula()
{
    if (!pilula)
        return;

    explodePilula2(heroi.x, heroi.y, 0, 1, 3);
    explodePilula2(heroi.x, heroi.y, 0, -1, 3);
    explodePilula2(heroi.x, heroi.y, 1, 0, 3);
    explodePilula2(heroi.x, heroi.y, -1, 0, 3);

    pilula--;
}

void explodePilula2(int x, int y, int somaX, int somaY, int qtd)
{
    if (qtd == 0)
        return;

    int novoX = x + somaX;
    int novoY = y + somaY;

    if (!ehValida(&m, novoX, novoY))
        return;
    if (ehParede(&m, novoX, novoY))
        return;

    m.matriz[novoX][novoY] = VAZIO;
    explodePilula2(novoX, novoY, somaX, somaY, qtd - 1);
}

int main()
{
    leMapa(&m);
    encontraMapa(&m, &heroi, HEROI);

    do
    {
        printf("Pílula: %s\n", (pilula ? "SIM" : "NÃO"));
        imprimeMapa(&m);

        char comando;
        scanf(" %c", &comando);
        move(comando);
        if (comando == BOMBA)
            explodePilula();
        fantasmas();
    } while (!acabou());

    liberaMapa(&m);
}