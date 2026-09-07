#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define TAM_CELULA 40
typedef struct
{
    Vector2 pos;
    Vector2 vel;
    float raio;
    Color cor;
} Bola;

int **criarMatriz(int linhas, int colunas)
{

    int **matriz = (int **)malloc(linhas * sizeof(int *));
    if (matriz == NULL)
        return NULL;

    for (int i = 0; i < linhas; i++)
    {

        matriz[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++)
        {

            matriz[i][j] = 0;
        }
    }
    return matriz;
}

void liberarMatriz(int **matriz, int linhas)
{
    for (int i = 0; i < linhas; i++)
    {
        free(matriz[i]);
    }
    free(matriz);
}

void desenharMatriz(int **matriz, int linhas, int colunas)
{
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            Color cor = (matriz[i][j] == 1) ? (Color){40, 120, 200, 255}
                                            : (Color){15, 30, 55, 255};
            DrawRectangle(j * TAM_CELULA, i * TAM_CELULA,
                          TAM_CELULA - 2, TAM_CELULA - 2, cor);
        }
    }
}

Bola *criarBolas(int quantidade)
{
    Bola *bolas = (Bola *)malloc(quantidade * sizeof(Bola));
    if (bolas == NULL)
        return NULL;

    for (int i = 0; i < quantidade; i++)
    {
        Bola *b = (bolas + i);
        b->pos = (Vector2){GetRandomValue(50, LARGURA_JANELA - 50),
                           GetRandomValue(50, ALTURA_JANELA - 50)};
        b->vel = (Vector2){(float)GetRandomValue(-4, 4),
                           (float)GetRandomValue(-4, 4)};
        if (b->vel.x == 0)
            b->vel.x = 2;
        if (b->vel.y == 0)
            b->vel.y = 2;
        b->raio = (float)GetRandomValue(10, 25);
        b->cor = (Color){GetRandomValue(100, 255), GetRandomValue(100, 255),
                         GetRandomValue(100, 255), 255};
    }
    return bolas;
}

void atualizarBola(Bola *b)
{
    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;

    if (b->pos.x - b->raio < 0 || b->pos.x + b->raio > LARGURA_JANELA)
        b->vel.x *= -1;
    if (b->pos.y - b->raio < 0 || b->pos.y + b->raio > ALTURA_JANELA)
        b->vel.y *= -1;
}

Bola *adicionarBola(Bola *bolas, int *quantidadeBolas)
{
    int novaQuantidade = *quantidadeBolas + 1;

    Bola *novoVetor = (Bola *)realloc(bolas, novaQuantidade * sizeof(Bola));
    if (novoVetor == NULL)
    {
        printf("Falha ao realocar memoria!\n");
        return bolas;
    }

    Bola *nova = (novoVetor + (novaQuantidade - 1));

    nova->pos = (Vector2){GetRandomValue(50, LARGURA_JANELA - 50),
                          GetRandomValue(50, ALTURA_JANELA - 50)};
    nova->vel = (Vector2){(float)GetRandomValue(-4, 4),
                          (float)GetRandomValue(-4, 4)};

    if (nova->vel.x == 0)
    {
        nova->vel.x = 2;
    }
    if (nova->vel.y == 0)
    {
        nova->vel.y = 2;
    }

    nova->raio = (float)GetRandomValue(10, 25);
    nova->cor = (Color){GetRandomValue(100, 255), GetRandomValue(100, 255), GetRandomValue(100, 255), 255};

    *quantidadeBolas = novaQuantidade;
    return novoVetor;
}

Bola *removerBola(Bola *bolas, int *quantidadeBolas)
{
    if (*quantidadeBolas <= 0)
    {
        return bolas;
    }

    int novaQuantidade = *quantidadeBolas - 1;
    if (novaQuantidade == 0)
    {
        free(bolas);
        *quantidadeBolas = 0;
        return NULL;
    }

    Bola *novoVetor = (Bola *)realloc(bolas, novaQuantidade * sizeof(Bola));
    if (novoVetor == NULL)
    {
        printf("Falha ao realocar memoria!\n");
        return bolas;
    }

    *quantidadeBolas = novaQuantidade;
    return novoVetor;
}

int main(void)
{
    srand((unsigned int)time(NULL));

    InitWindow(LARGURA_JANELA, ALTURA_JANELA,
               "Ponteiros e Alocacao Dinamica - raylib");
    SetTargetFPS(60);

    int linhas = ALTURA_JANELA / TAM_CELULA;
    int colunas = LARGURA_JANELA / TAM_CELULA;
    int **grade = criarMatriz(linhas, colunas);

    int quantidadeBolas = 12;
    Bola *bolas = criarBolas(quantidadeBolas);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            bolas = adicionarBola(bolas, &quantidadeBolas);
        }
        if (IsKeyPressed(KEY_BACKSPACE))
        {
            bolas = removerBola(bolas, &quantidadeBolas);
        }

        for (int i = 0; i < quantidadeBolas; i++)
        {
            atualizarBola(bolas + i);
        }

        for (int i = 0; i < quantidadeBolas; i++)
        {
            int col = (int)(bolas[i].pos.x / TAM_CELULA);
            int lin = (int)(bolas[i].pos.y / TAM_CELULA);

            if (lin >= 0 && lin < linhas && col >= 0 && col < colunas)
            {
                grade[lin][col] = 1;
            }
        }

        int celulasVisitadas = 0;
        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                if (grade[i][j] == 1)
                {
                    celulasVisitadas++;
                }
            }
        }

        desenharMatriz(grade, linhas, colunas);

        for (int i = 0; i < quantidadeBolas; i++)
        {
            DrawCircleV(bolas[i].pos, bolas[i].raio, bolas[i].cor);
        }

        DrawText("Matriz (int**) e vetor de structs (Bola*) alocados com malloc",
                 10, 10, 18, WHITE);
        DrawText(TextFormat("Bolas: %d  |  ESPACO: adicionar  |  BACKSPACE: remover", quantidadeBolas),
                 10, 32, 16, WHITE);
        DrawText("Pressione ESC para sair", 10, ALTURA_JANELA - 25, 16, WHITE);

        DrawText(TextFormat("Celulas visitadas: %d", celulasVisitadas), 10, 55, 16, WHITE);

        DrawText(TextFormat("Pressione ESC para sair"), 10, ALTURA_JANELA - 25, 16, WHITE);

        EndDrawing();
    }

    if (bolas != NULL)
    {
        free(bolas);
    }
    liberarMatriz(grade, linhas);

    CloseWindow();
    return 0;
}