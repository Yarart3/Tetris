#ifndef PARTIDA_H
#define PARTIDA_H

#include <stdio.h>
#include <string>
#include "InfoJoc.h"
#include "Joc.h"
#include "CuaMoviment.h"

using namespace std;

class Partida
{
public:
    Partida();

    void actualitza(int mode, double deltaTime);
    void dibuixa(); // Tauler, figura i marcador, sense avancar el joc
    void inicialitza(int mode, const string& fitxerInicial, const string& fitxerFigures, const string& fitxerMoviments); //int mode, const string& fitxerInicial, const string& fitxerFigures, const string& fitxerMoviments
    void puntuacio(int eliminades);
    void dificultat();
    bool gameOver();
    int getPunts() const { return m_punts; }
    void llegirMoviments(const string& fitxerMoviments);

private:
    double m_temps;
    Joc m_joc;
    Tauler m_tauler;
    Figura m_figura;
    int m_punts;
    float m_dificultat;
    int m_nivell;
    bool m_gameOver;
    CuaMoviment m_moviments;
    double m_tempsTest;
};

#endif 