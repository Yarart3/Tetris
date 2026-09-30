#include "Figura.h"
#include <iostream>
#include "GraphicManager.h"
#include <Windows.h>
using namespace std;

//Inicializtem la figura a 0
Figura::Figura()
{
    m_posX = 0;
    m_posY = 0;
    m_color = NO_COLOR;
    m_tipus = NO_FIGURA;
    m_columnes = 0;
    m_files = 0;

    for (int i = 0; i < MAX_ALCADA; i++)
    {
        for (int j = 0; j < MAX_AMPLADA; j++)
        {
            m_figura[i][j] = m_color;
        }
    }

}

//Inicialitzem la figura depenent del Tipus
void Figura::inicialitza(TipusFigura tipusFigura, int x, int y)
{
    m_tipus = tipusFigura;
    m_posX = x;
    m_posY = y;
    m_color = (ColorFigura)tipusFigura;

    for (int i = 0; i < MAX_ALCADA; i++)
        for (int j = 0; j < MAX_AMPLADA; j++)
            m_figura[i][j] = NO_COLOR;

    switch (tipusFigura)
    {
    case NO_FIGURA:
        m_files = 0;
        m_columnes = 0;
        break;
    case FIGURA_O:

        m_files = 2;
        m_columnes = 2;
        m_posX = x - 1;
        m_posY = y - 1;
        m_figura[0][0] = COLOR_GROC;
        m_figura[0][1] = COLOR_GROC;
        m_figura[1][0] = COLOR_GROC;
        m_figura[1][1] = COLOR_GROC;
        break;

    case FIGURA_L:
    {
        m_files = 3;
        m_columnes = 3;
        m_posX = x - 1;
        m_posY = y - 1;
        m_figura[0][2] = COLOR_TARONJA;
        m_figura[1][0] = COLOR_TARONJA;
        m_figura[1][1] = COLOR_TARONJA;
        m_figura[1][2] = COLOR_TARONJA;
    }
    break;

    case FIGURA_T:
    {
        m_files = 3;
        m_columnes = 3;
        m_posX = x - 1;
        m_posY = y - 1;
        m_figura[0][1] = COLOR_MAGENTA;
        m_figura[1][0] = COLOR_MAGENTA;
        m_figura[1][1] = COLOR_MAGENTA;
        m_figura[1][2] = COLOR_MAGENTA;
    }
    break;

    case FIGURA_S:

    {
        m_files = 3;
        m_columnes = 3;
        m_posX = x - 1;
        m_posY = y - 1;
        m_figura[0][1] = COLOR_VERD;
        m_figura[0][2] = COLOR_VERD;
        m_figura[1][0] = COLOR_VERD;
        m_figura[1][1] = COLOR_VERD;
    }

    break;

    case FIGURA_Z:
    {
        m_files = 3;
        m_columnes = 3;
        m_posX = x - 1;
        m_posY = y - 1;
        m_figura[0][0] = COLOR_VERMELL;
        m_figura[0][1] = COLOR_VERMELL;
        m_figura[1][1] = COLOR_VERMELL;
        m_figura[1][2] = COLOR_VERMELL;
    }

    break;

    case FIGURA_I:

    {
        m_files = 4;
        m_columnes = 4;
        m_posX = x - 1;
        m_posY = y - 1;
        m_figura[1][0] = COLOR_BLAUCEL;
        m_figura[1][1] = COLOR_BLAUCEL;
        m_figura[1][2] = COLOR_BLAUCEL;
        m_figura[1][3] = COLOR_BLAUCEL;
    }
    break;
    case FIGURA_J:
    {
        m_files = 3;
        m_columnes = 3;
        m_posX = x - 1;
        m_posY = y - 1;
        m_figura[0][0] = COLOR_BLAUFOSC;
        m_figura[1][0] = COLOR_BLAUFOSC;
        m_figura[1][1] = COLOR_BLAUFOSC;
        m_figura[1][2] = COLOR_BLAUFOSC;
    }
    break;
    }
}

//Moviment horitzontal de la figura
void Figura::moureFigura(int dirX)
{
    m_posX += dirX;
}

//Moviment vertical de la figura
void Figura::moureAbaix()
{
    m_posY += 1;
}

//Transposem la matriu
void Figura::transposarMatriu(ColorFigura figura[MAX_ALCADA][MAX_AMPLADA])
{
    ColorFigura temp;

    for (int i = 0; i < m_files; i++)
    {
        for (int j = i + 1; j < m_columnes; j++)
        {
            temp = figura[i][j];
            figura[i][j] = figura[j][i];
            figura[j][i] = temp;
        }
    }
}

//Fem gir horari o antihorari
void Figura::girar(DireccioGir direccio)
{
    ColorFigura temp;

    if (direccio == GIR_HORARI)
    {
        transposarMatriu(m_figura);

        for (int i = 0; i < m_files; i++)
        {
            for (int j = 0; j < m_columnes / 2; j++)
            {
                temp = m_figura[i][j];
                m_figura[i][j] = m_figura[i][m_columnes - j - 1];
                m_figura[i][m_columnes - j - 1] = temp;
            }
        }
    }
    else
    {
        transposarMatriu(m_figura);

        for (int i = 0; i < m_files / 2; i++)
        {
            for (int j = 0; j < m_columnes; j++)
            {
                temp = m_figura[i][j];
                m_figura[i][j] = m_figura[m_files - i - 1][j];
                m_figura[m_files - i - 1][j] = temp;
            }
        }
    }
}

ifstream& operator>>(ifstream& input, Figura& figura)
{
    int tFigura;
    int x, y;
    int codiGir;

    input >> tFigura >> y >> x >> codiGir;
    TipusFigura fig = static_cast<TipusFigura>(tFigura);
    figura.inicialitza(fig, x+1, y);

    for (int i = 0; i < codiGir; i++)
    {
        figura.girar(GIR_HORARI);
    }

    return input;
}

void Figura::dibuixaFigura() //Funció per dibuixar una figura.
{
    for (int i = 0; i < m_files; i++) //Recorrem cada posició que ocupa la figura i la pintem amb el seu color corresponent a la posició corresponent.
    {
        for (int j = 0; j < m_columnes; j++)
        {
            if (m_figura[i][j] != NO_COLOR)
            {
                GraphicManager::getInstance()->drawSprite((IMAGE_NAME)(m_figura[i][j] + 1), //Color de la figura.
                    POS_X_TAULER + ((m_posX + j) * MIDA_QUADRAT), POS_Y_TAULER + ((m_posY + i) * MIDA_QUADRAT), false);
            }

        }
    }
}

int Figura::numAleatori(int num)
{
    int aleatori;
    Sleep(1);

    if (num != 0)
        aleatori = rand() % num + 1;
    else
        aleatori = 2 + rand() % 7;

    return aleatori;
}

void Figura::generarFormaAleatoria()
{
    inicialitza((TipusFigura)numAleatori(7), numAleatori(0), 1);

    for (int i = 0; i < numAleatori(3); i++)
    {
        girar(GIR_HORARI);
    }
}
