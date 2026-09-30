#ifndef TAULER_H
#define TAULER_H
#include "Figura.h"
#include <iostream>
#include <fstream>
#include "GraphicManager.h"
using namespace std;


class Tauler
{
private:

    ColorFigura m_tauler[N_FILES_TAULER][N_COL_TAULER];

public:
    Tauler();
    ColorFigura getTauler(int i, int j) const { return m_tauler[i][j]; }

    void setTauler(int x, int y, ColorFigura color) { m_tauler[x][y] = color; }

    bool MovimentValid(const Figura& figura, int novaPosX, int novaPosY); //Comprovem si el moviment que volem fer es valid
    bool GirValid(DireccioGir direccio, const Figura& figura); //Comprovem si el gir de la figura es valid
    bool comprobaFilaCompleta(int fila); //Comprovem si una fila està completa
    int suprimirFiles(int fila); //Bucle per eliminar files
    void suprimirFila(int fila); //Eliminem nomes una fila
    bool taulerPle(const ColorFigura& figura);

    void dibuixaTauler();
};

//Sobrecarrega d'operadors
ofstream& operator<<(ofstream& output, Tauler tauler);
ifstream& operator>>(ifstream& input, Tauler& tauler);


#endif