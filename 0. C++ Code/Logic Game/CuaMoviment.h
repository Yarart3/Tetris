#ifndef CUAMOVIMENT_H
#define CUAMOVIMENT_H

#include <string>
#include <fstream>
#include <iostream>
#include "Figura.h"
#include "Joc.h"

using namespace std;

typedef enum 
{
    MOVIMENT_ESQUERRA = 0,
    MOVIMENT_DRETA = 1,
    MOVIMENT_GIR_HORARI = 2,
    MOVIMENT_GIR_ANTI_HORARI = 3,
    MOVIMENT_BAIXA = 4,
    MOVIMENT_BAIXA_FINAL = 5,
} TipusMoviment;

class Moviment {
private:
    Moviment* m_seguent;
    TipusMoviment m_mov;

public:
    Moviment(const TipusMoviment& m_mov);
    void setNext(Moviment* seguent) { m_seguent = seguent; }
    Moviment* getSeguent() const { return m_seguent; }
    TipusMoviment getMov() const { return m_mov; }
};

class CuaMoviment {
public:
    CuaMoviment();

    bool esBuida();
    void afegir(Moviment& moviment);
    void treure();
    Moviment* getPrimer() const { return m_primer; }
    //void executarMoviment(TipusMoviment moviment);

private:
    Moviment* m_primer;
    Moviment* m_ultim;
};

#endif 
