#ifndef FIGURA_H
#define FIGURA_H

#include <fstream>
#include <iostream>
#include "InfoJoc.h"
using namespace std;


//typedef enum
//{
//    COLOR_NEGRE = 0,
//    COLOR_GROC,
//    COLOR_BLAUCEL,
//    COLOR_MAGENTA,
//    COLOR_TARONJA,
//    COLOR_BLAUFOSC,
//    COLOR_VERMELL,
//    COLOR_VERD,
//    NO_COLOR
//} ColorFigura;
//
//typedef enum
//{
//    NO_FIGURA = 0,
//    FIGURA_O,
//    FIGURA_I,
//    FIGURA_T,
//    FIGURA_L,
//    FIGURA_J,
//    FIGURA_Z,
//    FIGURA_S,
//} TipusFigura;

const int MAX_ALCADA = 4;
const int MAX_AMPLADA = 4;


typedef enum
{
    GIR_HORARI = 0,
    GIR_ANTI_HORARI
} DireccioGir;

class Figura
{
private:
    TipusFigura m_tipus;
    ColorFigura m_color;
    int m_posX;
    int m_posY;
    ColorFigura m_figura[MAX_ALCADA][MAX_AMPLADA];
    int m_files;
    int m_columnes;

public:

    Figura();

    TipusFigura getTipus() const { return m_tipus; }
    ColorFigura getColor() const { return m_color; }
    int getPosX() const { return m_posX; }
    int getPosY() const { return m_posY; }
    //int getCodiGir() const { return m_codiGir; }
    void setPosX(int posX) { m_posX = posX; }
    void setPosY(int posY) { m_posY = posY; }
    int getFiles() const { return m_files; }
    int getColumnes() const { return m_columnes; }
    ColorFigura getValForma(int i, int j) const { return m_figura[i][j]; };

    void inicialitza(TipusFigura tipusFigura, int x, int y);
    void moureFigura(int dirX);

    void moureAbaix();
    void transposarMatriu(ColorFigura figura[MAX_ALCADA][MAX_AMPLADA]);
    void girar(DireccioGir direccio);

    int numAleatori(int num);
    void generarFormaAleatoria();

    void dibuixaFigura();

    
};
ifstream& operator>>(ifstream& input, Figura& figura);

#endif