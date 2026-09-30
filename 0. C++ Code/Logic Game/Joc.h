#ifndef JOC_H
#define JOC_H
#include "Tauler.h"
#include "Figura.h"
#include <string>
#include <iostream>
#include <fstream>
#include "CuaFigura.h"
using namespace std;



class Joc
{
public:

	void setTauler(const Tauler& tauler) { m_tauler = tauler; }
	void setFiguraActual(const Figura& figura) { m_figuraActual = figura; }


	void inicialitzaFitxer(const string& nomFitxer, const string& nomFitxer2);
	bool giraFigura(DireccioGir direccio);
	bool mouFigura(int dirX);
	int baixaFigura();
	void escriuTauler(const string& nomFitxer);
	void colocarFigura();
	void eliminaFigura(int posX, int posY);


	void dibuixaJoc();
	int baixarDeCop();

	bool getColocada() { return m_colocada;  }
	int getX() { return m_figuraActual.getPosX(); }
	int getY() { return m_figuraActual.getPosY(); }
	void setX(int x) { m_figuraActual.setPosX(x); }
	void setY(int y) { m_figuraActual.setPosY(y); }
	int getAmplada() { return m_figuraActual.getColumnes(); }
	int getALcada() { return m_figuraActual.getFiles(); }
	ColorFigura getFigura(int i, int j) { return m_figuraActual.getValForma(i, j); }
	void inicialitza();
	bool finalitzarTetris();
	void llegirFigura(const string& fitxerFigures);
	bool getFinalitzat() const { return m_finalitzat; }

private:
	Tauler m_tauler;
	Figura m_figuraActual;
	bool m_colocada;
	CuaFigura m_fig;
	int m_mode;
	bool m_finalitzat;
};

#endif