#include "Joc.h"
#include <fstream>
#include <iostream>
#include "Tauler.h"

using namespace std;


//Inicialitzem la figura a partir d'un fitxer
void Joc::inicialitzaFitxer(const string& nomFitxer, const string& nomFitxer2)
{
    m_mode = 0;
    m_finalitzat = false;
    ifstream fitxer;
    fitxer.open(nomFitxer);
    if (fitxer.is_open())
    {
        fitxer >> m_figuraActual;
        fitxer >> m_tauler;
        fitxer.close();
    }

    llegirFigura(nomFitxer2);
}

void Joc::inicialitza()
{
    m_colocada = false;
    m_mode = 1;
    m_finalitzat = false;

    // Reiniciar el estado del tablero
    for (int i = 0; i < N_FILES_TAULER; i++)
    {
        for (int j = 0; j < N_COL_TAULER; j++)
        {
            m_tauler.setTauler(i, j, COLOR_NEGRE);
        }
    }

    // Restablecer la figura actual
    m_figuraActual.inicialitza(NO_FIGURA, 0, 0);

    // Generar una nueva figura aleatoria
    m_figuraActual.generarFormaAleatoria();

    m_figuraActual.setPosY(0); // Establece la posición inicial de la figura en la parte superior
    m_figuraActual.setPosX(N_COL_TAULER / 2 - m_figuraActual.getColumnes() / 2); // Centra la figura en el tabler
}

void Joc::colocarFigura()
{
    int i, j, num;
    i = num = 0;

    while (i < m_figuraActual.getFiles() && num < 4)
    {
        j = 0;
        while (j < m_figuraActual.getColumnes() && num < 4)
        {
            if (m_figuraActual.getValForma(i, j) != NO_COLOR)
            {
                m_tauler.setTauler(m_figuraActual.getPosY() + i, m_figuraActual.getPosX() + j, m_figuraActual.getColor());
                num++;
            }
            j++;
        }
        i++;
    }

    if (m_mode == 1)
        m_figuraActual.generarFormaAleatoria();
    else {
        if (!m_fig.esBuida()) {
            m_figuraActual = m_fig.getPrimer()->getFigura();
            m_fig.treure();
        }
        else {
            m_finalitzat = true;
        }
    }
}

//Girem la figura i la coloquem al tauler
bool Joc::giraFigura(DireccioGir direccio)
{
    bool gir = false;
    Figura figuraGirada = m_figuraActual;

    if (m_tauler.GirValid(direccio, m_figuraActual))
    {
        m_figuraActual.girar(direccio);
        gir = true;
    }

    return gir;
}

//Movem la figura i la coloquem al tauler
bool Joc::mouFigura(int dirX)
{
    bool valid = false;

    int posX = m_figuraActual.getPosX() + dirX;
    int posY = m_figuraActual.getPosY();

    if (m_tauler.MovimentValid(m_figuraActual, posX, posY))
    {
        m_figuraActual.moureFigura(dirX);
        valid = true;
    }

    return valid;
}

//Baixem la figura i la coloquem al tauler
int Joc::baixaFigura()
{
    m_colocada = false;
    int filesCompletades = 0;

    int posX = m_figuraActual.getPosX();
    int posY = m_figuraActual.getPosY();

    //eliminaFigura(posX, posY);
    if (m_tauler.MovimentValid(m_figuraActual, posX, posY + 1))
    {
        m_figuraActual.moureAbaix();
    }
    else
    {
        colocarFigura();
        filesCompletades = m_tauler.suprimirFiles(posY);
        m_colocada = true;
    }

    return filesCompletades;
}

//Funcio per poder escriure al tauler
void Joc::escriuTauler(const string& nomFitxer)
{
    ofstream fitxer;
    fitxer.open(nomFitxer);

    if (fitxer.is_open())
    {
        fitxer << m_tauler;
        fitxer.close();
    }
}

//Eliminar la figura del tauler
void Joc::eliminaFigura(int posX, int posY)
{
    for (int i = 0; i < MAX_ALCADA; i++)
    {
        for (int j = 0; j < MAX_AMPLADA; j++)
        {
            if (m_figuraActual.getValForma(i, j) == 1)
            {
                m_tauler.setTauler(i + posY, j + posX, COLOR_NEGRE);
            }
        }
    }
}

void Joc::dibuixaJoc()
{
    m_tauler.dibuixaTauler();

    m_figuraActual.dibuixaFigura();
}

int Joc::baixarDeCop()
{
    int filesCompletades = 0;

    int posX = m_figuraActual.getPosX();
    int posY = m_figuraActual.getPosY();

    
    while (m_tauler.MovimentValid(m_figuraActual, posX, posY + 1)) {
        m_figuraActual.moureAbaix();
        posY++;
    }

    colocarFigura();
    filesCompletades = m_tauler.suprimirFiles(posY);

    return filesCompletades;
}

bool Joc::finalitzarTetris()
{
    bool finalitzar = m_tauler.taulerPle(getFigura(getX(), getY()));
    if (finalitzar)
    {
        m_figuraActual.inicialitza(NO_FIGURA, 0, 0);
    }

    return finalitzar;
}

void Joc::llegirFigura(const string& fitxerFigures)
{
    ifstream fitxer(fitxerFigures);

    if (fitxer.is_open())
    {
        int figura, fila, columna, gir;

        fitxer >> figura >> fila >> columna >> gir;

        while (!fitxer.eof())
        {
            Figura f;
            TipusFigura fig = static_cast<TipusFigura>(figura);
            
            f.inicialitza(fig, columna+1, fila);

            for (int i = 0; i < gir; i++)
            {
                f.girar(GIR_HORARI);
            }
            
            novaFigura* ff;
            ff = new novaFigura(f);

            m_fig.afegir(*ff);
            fitxer >> figura >> fila >> columna >> gir;
        }

        fitxer.close();
    }
}