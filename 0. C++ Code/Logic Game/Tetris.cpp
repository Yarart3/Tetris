#include "Tetris.h"
#include <iostream>
#include <SDL.h>
using namespace std;

void Tetris::juga(Screen& pantalla, int mode)
{
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    bool isPaused = false;

    pantalla.show();

    PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    m_partida.inicialitza(mode, "./data/Games/partida.txt", "./data/Games/figures.txt", "./data/Games/moviments.txt");

    bool exitGame = false;

    while (!exitGame) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

        pantalla.processEvents();

        if (Keyboard_GetKeyTrg(KEYBOARD_ESCAPE)) {
            exitGame = true; // Volvemos al menú
            isPaused = true; // Pausamos el juego
        }

        if (Keyboard_GetKeyTrg(KEYBOARD_P)) {
            isPaused = !isPaused;
            if (isPaused) {
                cout << "Juego pausado. Presiona 'P' para reanudar." << endl;
            }
        }

        if (!isPaused)
        {
            if (!m_partida.gameOver())
            {
                m_partida.actualitza(mode, deltaTime);
                pantalla.update();
            }
            else {
                exitGame = true;
            }
        }
        else {
            SDL_Delay(100);
        }
    }

    if (m_partida.gameOver()) {
        record();
    }

    // Detenemos la música, pero no cerramos la pantalla ni el renderizador para mantener el estado
    PlaySound(NULL, 0, 0);
}

bool Tetris::Menu(Screen& pantalla)
{
    bool exitApp = false;

    while (!exitApp) {
        cout << "                   ---------------MENU---------------" << endl;
        cout << "                   -----1.-Jugar en mode normal------" << endl;
        cout << "                   -----2.-Jugar en mode test--------" << endl;
        cout << "                   -----3.-Millors puntuacions-------" << endl;
        cout << "                   -----4.-Controls------------------" << endl;
        cout << "                   -----5.-Puntuacions---------------" << endl;
        cout << "                   -----6.-Sortir--------------------" << endl;
        cout << endl;
        cout << "Escull una opcio: ";

        int opcio = 0;
        cin >> opcio;

        switch (opcio)
        {
        case 1:
            // Reanudar o iniciar un nuevo juego en modo normal
            pantalla.show();
            juga(pantalla, 1);
            break;

        case 2:
            // Reanudar o iniciar un nuevo juego en modo test
            pantalla.show();
            juga(pantalla, 0);
            break;

        case 3:
            mostraPuntuacions();
            cout << endl;
            break;

        case 4:
            menuControls();
            break;

        case 5:
            menuPuntuacions();
            break;

        case 6:
            cout << "Sortint..." << endl;
            exitApp = true;
            break;

        default:
            cout << "Opció no vàlida!" << endl;
            break;
        }

        // Pausa y limpia la pantalla después de cada opción
        if (!exitApp) {
            cout << "Prem una tecla per continuar...";
            system("pause>nul");
            system("cls");
        }
    }

    return exitApp;
}

void Tetris::menuControls()
{
    cout << "                   ---------------MENU CONTROLS---------------" << endl;
    cout << "                   -----------FLETXA AMUNT / TECLA W = GIR HORARI-------" << endl;
    cout << "                   -----------FLETXA AVALL / TECLA S = GIR ANTIHORARI---" << endl;
    cout << "                   -----------TECLA C                = BAIXAR FIGURA----" << endl;
    cout << "                   -----------TECLA SPACE            = BAIXAR DE COP----" << endl;
    cout << "                   --------FLETXA ESQUERRA / TECLA A = MOURE ESQUERRA---" << endl;
    cout << "                   -----------FLETXA DRETA / TECLA D = MOURE DRETA------" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
}

void Tetris::menuPuntuacions()
{
    cout << "                              ---------------MENU PUNTUACIONS------------" << endl;
    cout << "                              ------ELIMINAR 1 FILA  -> +100 punts-------" << endl;
    cout << "                              ------ELIMINAR 2 FILES -> +150 punts-------" << endl;
    cout << "                              ------ELIMINAR 3 FILES -> +175 punts-------" << endl;
    cout << "                              ------ELIMINAR 4 FILES -> +200 punts-------" << endl;
    cout << "                              ------FIGURA COLOCADA  -> +10 punts--------" << endl;
    cout << "                              ------BAIXAR DE COP    -> +30 punts--------" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
}

ifstream& operator>>(ifstream& input, Puntuacio& pts)
{
    input >> pts.nom >> pts.punts;
    return input;
}

void Tetris::llegirPuntuacions(const string& nomFitxer)
{
    ifstream fitxer;
    Puntuacio usuari;
    fitxer.open(nomFitxer);

    if (fitxer.is_open())
    {
        fitxer >> usuari;

        while (!fitxer.eof())
        {
            guardarPuntuacio(usuari, false);
            fitxer >> usuari;
        }

        fitxer.close();
    }
}

void Tetris::guardarPuntuacio(Puntuacio usuari, bool escriure)
{
    if (escriure)
        escriureFitxer(usuari, "./data/Games/puntuacions.txt");

    if (m_puntuacions.empty()) {
        m_puntuacions.push_front(usuari);
    }
    else {
        list<Puntuacio>::iterator actual = m_puntuacions.begin();
        bool trobat = false;

        while (actual != m_puntuacions.end() && !trobat)
        {
            if (usuari.punts > (*actual).punts)
            {
                trobat = true;
            }
            else
                actual++;
        }

        m_puntuacions.insert(actual, usuari);
    }
}

void Tetris::mostraPuntuacions()
{
    list<Puntuacio>::iterator actual = m_puntuacions.begin();
    int num = 0;

    while (actual != m_puntuacions.end() && num < 5)
    {
        cout << (*actual).nom << " " << (*actual).punts << endl;
        actual++;
    }
}

void Tetris::escriureFitxer(Puntuacio& usuari, const string& path)
{
    ofstream fitxer(path, ios::app);

    if (fitxer.is_open()) {
        fitxer << usuari.nom << " " << to_string(usuari.punts) << endl;
        fitxer.close();
    }
}

void Tetris::record()
{
    string nom;
    cout << "Introdueix el teu nom per guardar la teva puntuacio (" << m_partida.getPunts() << "): ";
    cin >> nom;

    Puntuacio pts;
    pts.nom = nom;
    pts.punts = m_partida.getPunts();

    guardarPuntuacio(pts, true);
}
