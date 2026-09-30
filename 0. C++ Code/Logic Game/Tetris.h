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
	void record(Screen& pantalla);

private:
	void menuPuntuacions();
	void menuControls();
	void dibuixaMenu(int seleccionada);
	void dibuixaPeuSubmenu();
	void gestionaVolum(double deltaTime);
	void dibuixaPanelVolum();
	list<Puntuacio> m_puntuacions;
	Partida m_partida;
	bool m_panelVolum = false;          // Panel obert amb la tecla V
	double m_tempsPanelVolum = 0.0;     // Segons que queda visible despres de tocar +/-/M
};

ifstream& operator>>(ifstream& input, Puntuacio& pts);

#endif 