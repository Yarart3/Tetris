#include "Partida.h"
#include "InfoJoc.h"
#include "GraphicManager.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include "Audio.h"

#include <windows.h>
#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

Partida::Partida()
{
    m_temps = 0; 
    m_punts = 0;
    m_dificultat = 1.0;
    m_nivell = 1;
    m_gameOver = false;
    m_tempsTest = 0.0;
    srand(time(NULL));
}

bool Partida::gameOver() {
    return m_gameOver;
}

void Partida::llegirMoviments(const string& fitxerMoviments)
{
    ifstream fitxer(fitxerMoviments);

    if (fitxer.is_open())
    {
        int moviment;

        fitxer >> moviment;

        while (!fitxer.eof()) {

            Moviment* mov;
            mov = new Moviment(static_cast<TipusMoviment> (moviment));
            m_moviments.afegir(*mov);
            fitxer >> moviment;
        }

        fitxer.close();
    }
}

void Partida::inicialitza(int mode, const string& fitxerInicial, const string& fitxerFigures, const string& fitxerMoviments)//int mode, const string& fitxerInicial, const string& fitxerFigures, const string& fitxerMoviments)
{
    m_temps = 0;
    m_punts = 0;
    m_dificultat = 1.0;
    m_nivell = 1;
    m_gameOver = false;
    m_tempsTest = 0.0;

    if (mode == 0) {
        m_joc.inicialitzaFitxer(fitxerInicial, fitxerFigures);
        llegirMoviments(fitxerMoviments);
    }
    else
        m_joc.inicialitza();
}

void Partida::puntuacio(int eliminades)
{
    int punts = 0;

    switch (eliminades)
    {
        case 1:
            punts += 100;
            break;
        case 2:
            punts += 150;
            break;
        case 3:
            punts += 175;
            break;
        case 4:
            punts += 200;
            break;
    }

    if (m_joc.getColocada() == true)
        punts += 10;

    m_punts += punts;
    
}

void Partida::dificultat()
{
    if (m_punts > 9000) {
        m_dificultat = 0.1;
        m_nivell = 10;
    }
    else if (m_punts > 8000) {
        m_dificultat = 0.2;
        m_nivell = 9;
    }
    else if (m_punts > 7000) {
        m_dificultat = 0.3;
        m_nivell = 8;
    }
    else if (m_punts > 6000) {
        m_dificultat = 0.4;
        m_nivell = 7;
    }
    else if (m_punts > 5000) {
        m_dificultat = 0.5;
        m_nivell = 6;
    }
    else if (m_punts > 4000) {
        m_dificultat = 0.6;
        m_nivell = 5;
    }
    else if (m_punts > 3000) {
        m_dificultat = 0.7;
        m_nivell = 4;
    }
    else if (m_punts > 2000) {
        m_dificultat = 0.8;
        m_nivell = 3;
    }
    else if (m_punts > 1000) {
        m_dificultat = 0.9; 
        m_nivell = 2;
    }
}

void Partida::dibuixa()
{
    m_joc.dibuixaJoc();

    //Puntuacio i nivell
    string msg = "Puntuacio: " + to_string(m_punts) + ",            Nivell: " + to_string(m_nivell);
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER, POS_Y_TAULER - 50, 0.85, msg);
}

void Partida::actualitza(int mode, double deltaTime)
{

    // Dibujar fons y tauler buit
    //GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0, false);
    //GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER, false);
    
    int eliminades;

    dibuixa();

    m_temps += deltaTime;
    m_tempsTest += deltaTime;

    dificultat();

    if (m_temps > m_dificultat)
    {
        eliminades = m_joc.baixaFigura();
        //m_joc.baixaFigura();
        puntuacio(eliminades);
        m_temps = 0.0;
    }

    if (mode == 1)
    {
        //Moviments
        if (Keyboard_GetKeyTrg(KEYBOARD_SPACE))
        {
            eliminades = m_joc.baixarDeCop();
            m_punts += 30;
            puntuacio(eliminades);
        }

        if (Keyboard_GetKeyTrg(KEYBOARD_C))
        {
            eliminades = m_joc.baixaFigura();
            puntuacio(eliminades);
        }

        if (Keyboard_GetKeyTrg(KEYBOARD_RIGHT) || Keyboard_GetKeyTrg(KEYBOARD_D))
            m_joc.mouFigura(1);

        if (Keyboard_GetKeyTrg(KEYBOARD_LEFT) || Keyboard_GetKeyTrg(KEYBOARD_A))
            m_joc.mouFigura(-1);

        if (Keyboard_GetKeyTrg(KEYBOARD_UP) || Keyboard_GetKeyTrg(KEYBOARD_W))
            m_joc.giraFigura(GIR_HORARI);

        if (Keyboard_GetKeyTrg(KEYBOARD_DOWN) || Keyboard_GetKeyTrg(KEYBOARD_S))
            m_joc.giraFigura(GIR_ANTI_HORARI);
    }
    else
    {
        if (m_tempsTest > 0.6 && !m_moviments.esBuida()) 
        {
            TipusMoviment moviment;
            moviment = m_moviments.getPrimer()->getMov();
            m_moviments.treure();
            m_tempsTest = 0.0;
            switch (moviment)
            {
            case MOVIMENT_DRETA:
                m_joc.mouFigura(1);
                break;
            case MOVIMENT_ESQUERRA:
                m_joc.mouFigura(-1);
                break;
            case MOVIMENT_BAIXA:
                m_joc.baixaFigura();
                break;
            case MOVIMENT_BAIXA_FINAL:
                m_joc.baixarDeCop();
                break;
            case MOVIMENT_GIR_ANTI_HORARI:
                m_joc.giraFigura(GIR_ANTI_HORARI);
                break;
            case MOVIMENT_GIR_HORARI:
                m_joc.giraFigura(GIR_HORARI);
                break;
            }
        }
    }

    //Finalitzar el joc
    if (m_joc.finalitzarTetris() || m_joc.getFinalitzat())
    {
        Audio::playGameOver();
        m_gameOver = true;
        GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0, false);
        string msg = "GAME OVER";
        string msg2 = "Torna-ho a intentar una altra vegada";
        string msg3 = "La teva puntuacio ha sigut: " + to_string(m_punts);
        GraphicManager::getInstance()->drawFont(FONT_RED_30, POS_X_TAULER, POS_Y_TAULER + 150, 1.75, msg);
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER - 20, POS_Y_TAULER + 250, 0.75, msg2);
        GraphicManager::getInstance()->drawFont(FONT_WHITE_30, POS_X_TAULER , POS_Y_TAULER + 290, 0.75, msg3);
    }

}   

