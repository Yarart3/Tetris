#include "Tauler.h"
#include <iostream>
#include <fstream>
using namespace std;

//Operador de sortida, per mostrar el tauler
ofstream& operator<<(ofstream& output, Tauler tauler)
{
    for (int i = 0; i < N_FILES_TAULER; i++)
    {
        for (int j = 1; j < N_COL_TAULER; j++)
        {
            output << tauler.getTauler(i, j) << " ";
        }
        output << endl;
    }
    return output;
}

//Operador d'entrada, per llegir el tauler
ifstream& operator>>(ifstream& input, Tauler& tauler)
{
    int color;
    ColorFigura figura;

    for (int i = 0; i < N_FILES_TAULER; i++)
    {
        for (int j = 1; j < N_COL_TAULER; j++)
        {
            input >> color;
            figura = static_cast<ColorFigura>(color);
            tauler.setTauler(i, j, figura);
        }
    }
    return input;
}

//Inicialitzar el tauler a Color Negre
Tauler::Tauler()
{
    for (int i = 0; i < N_FILES_TAULER; i++)
    {
        for (int j = 1; j < N_COL_TAULER; j++)
        {
            m_tauler[i][j] = COLOR_NEGRE;
        }
    }
}

//Comprovem amb el tauler si el moviment de la figura es valid
bool Tauler::MovimentValid(const Figura& figura, int novaPosX, int novaPosY)
{

    for (int i = 0; i < MAX_ALCADA; i++)
    {
        for (int j = 0; j < MAX_AMPLADA; j++)
        {
            if (figura.getValForma(i, j) != NO_COLOR)
            {
                int posX = novaPosX + j;
                int posY = novaPosY + i;
                if (posX - 1 < 0 || posX >= N_COL_TAULER || posY >= N_FILES_TAULER || m_tauler[posY][posX] != COLOR_NEGRE)
                {
                    return false;
                }
            }

        }
    }

    return true;
}

//Comprovem amb el tauler si el gir de la figura es valid
bool Tauler::GirValid(DireccioGir direccio, const Figura& figura)
{

    int posX = figura.getPosX();
    int posY = figura.getPosY();
    int i = 0;
    int j = 0;

    Figura figuraTemporal = figura;


    figuraTemporal.girar(direccio);

    for (i = 0; i < MAX_ALCADA; i++)
    {
        for (j = 0; j < MAX_AMPLADA; j++)
        {
            if (figuraTemporal.getValForma(i, j) != NO_COLOR)
            {
                int nuevaPosX = posX + j;
                int nuevaPosY = posY + i;


                if (nuevaPosX <= 0 || nuevaPosX >= N_COL_TAULER || nuevaPosY >= N_FILES_TAULER)
                {
                    return false;
                }


                if (m_tauler[nuevaPosY][nuevaPosX] != COLOR_NEGRE)
                {
                    return false;
                }
            }
        }
    }


    return true;
}

//Comprovem si una fila esta completa
bool Tauler::comprobaFilaCompleta(int fila)
{
    for (int j = 1; j < N_COL_TAULER; j++)
    {
        if (m_tauler[fila][j] == COLOR_NEGRE)
        {
            return false;
        }
    }
    return true;
}

//Suprimim la fila i la baixem
void Tauler::suprimirFila(int fila)
{
    for (int i = fila; i > 0; i--)
    {
        for (int j = 0; j < N_COL_TAULER; j++)
        {
            m_tauler[i][j] = m_tauler[i - 1][j];
        }
    }

    for (int i = 0; i < N_COL_TAULER; i++)
    {
        m_tauler[0][i] = COLOR_NEGRE;
    }
}

//Utilitzem les dues funciones per eliminar files
int Tauler::suprimirFiles(int fila)
{
    int filesCompletades = 0;
    for (int i = fila; i < fila + 4; i++)
    {
        if (comprobaFilaCompleta(i))
        {
            suprimirFila(i);
            filesCompletades++;
        }
    }
    return filesCompletades;

}

//Métode per dibiuxar el tauler
void Tauler::dibuixaTauler()
{
    GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0, false);
    GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER, false);

    for (int i = 0; i < N_FILES_TAULER; i++)
    {
        for (int j = 1; j < N_COL_TAULER; j++)
        {
            if (m_tauler[i][j] != COLOR_NEGRE) //Pintem el tauler només quan la casella no és negre.
            {
                    GraphicManager::getInstance()->drawSprite((IMAGE_NAME)(m_tauler[i][j] + 1), //Color de la posició del tauler.
                    POS_X_TAULER + (j * MIDA_QUADRAT), POS_Y_TAULER + (i * MIDA_QUADRAT), false); //Posició x i posició y dins del tauler (possible error).
            }
        }
    }
}

bool Tauler::taulerPle(const ColorFigura& figura)
{
    bool ple = false;
    
    for (int j = 0; j < N_COL_TAULER; j++)
    {
        if (m_tauler[0][j] != COLOR_NEGRE)
        {
            ple = true;
        }
    }
    return ple;
}

