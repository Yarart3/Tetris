#ifndef LLISTAFIGURES_H
#define LLISTAFIGURES_H

#include <string>
#include <fstream>
#include <iostream>
#include "Figura.h"

using namespace std;

class novaFigura {
private:
    Figura m_valor;
    novaFigura* m_seguent;

public:
    novaFigura(const Figura& valor);
    void setNext(novaFigura* seguent) { m_seguent = seguent; }
    novaFigura* getSeguent() const { return m_seguent; }
    Figura getFigura() const { return m_valor; }
};

class CuaFigura {
public:
    CuaFigura();

    bool esBuida();
    void afegir(novaFigura& figura);
    void treure();
    novaFigura* getPrimer() const { return m_primer; }

private:
    novaFigura* m_primer;
    novaFigura* m_ultim;
};

#endif 
