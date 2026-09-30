#include "CuaMoviment.h"

Moviment::Moviment(const TipusMoviment& mov) : m_seguent(nullptr), m_mov(mov) {}
CuaMoviment::CuaMoviment() : m_primer(nullptr), m_ultim(nullptr) {}

bool CuaMoviment::esBuida()
{
    bool buida = false;

    if (m_primer == nullptr)
        buida = true;

    return buida;
}

void CuaMoviment::afegir(Moviment& moviment)
{
    if (esBuida()) {
        m_primer = &moviment;
        m_ultim = &moviment;
    }
    else {
        m_ultim->setNext(&moviment);
        m_ultim = &moviment;
        m_ultim->setNext(nullptr);
    }
}

void CuaMoviment::treure()
{
    if (!esBuida()) {
        Moviment* aux;
        aux = m_primer->getSeguent();
        delete m_primer;
        m_primer = aux;

        if (m_primer == nullptr)
            m_ultim = nullptr;
    }
}



