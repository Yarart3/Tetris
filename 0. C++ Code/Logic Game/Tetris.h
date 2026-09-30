#ifndef TETRIS_H
#define TETRIS_H

#include <list>
#include <iostream>
#include "Partida.h"
#include <Windows.h>

using namespace std;

typedef struct
{
	int punts;
	string nom;
}Puntuacio;


class Tetris
{
public:
	//Tetris();
	//~Tetris();
	void juga(Screen& pantalla, int mode);
	bool Menu(Screen& pantalla);
	void llegirPuntuacions(const string& nomFitxer);
	void guardarPuntuacio(Puntuacio usuari, bool escriure);
	void mostraPuntuacions();
	void escriureFitxer(Puntuacio& usuari, const string& path);
	void record();

private:
	void menuPuntuacions();
	void menuControls();
	list<Puntuacio> m_puntuacions;
	Partida m_partida;
};

ifstream& operator>>(ifstream& input, Puntuacio& pts);

#endif 