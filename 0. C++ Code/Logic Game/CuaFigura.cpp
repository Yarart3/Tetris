#include "CuaFigura.h"

novaFigura::novaFigura(const Figura& valor) : m_seguent(nullptr), m_valor(valor) {}
CuaFigura::CuaFigura() : m_primer(nullptr), m_ultim(nullptr) {}

bool CuaFigura::esBuida()
{
    bool buida = false;

    if (m_primer == nullptr)
        buida = true;

    return buida;
}

void CuaFigura::afegir(novaFigura& figura)
{
    if (esBuida()) {
        m_primer = &figura;
        m_ultim = &figura;
    }
    else {
        m_ultim->setNext(&figura);
        m_ultim = &figura;
        m_ultim->setNext(nullptr);
    }
   
}

void CuaFigura::treure()
{
    if (!esBuida()) {
        novaFigura* aux;
        aux = m_primer->getSeguent();
        delete m_primer;
        m_primer = aux;

        if (m_primer == nullptr)
            m_ultim = nullptr;
    }
}


