#include "Tetris.h"
#include <iostream>
#include <SDL.h>
#include <cctype>
#include "Audio.h"
#include "GraphicManager.h"
using namespace std;

static const int PAS_VOLUM = 5;
static const double TEMPS_PANEL_VOLUM = 2.0;
static const int MAX_PUNTUACIONS_MOSTRADES = 10;
static const size_t MAX_LLARGADA_NOM = 12;
static const Uint32 TEMPS_ESPERA_NOM = 500;   // ms abans d'acceptar tecles al game over

// Keyboard_GetKeyTrg converteix la tecla a scancode; si la distribucio del teclat
// no te aquesta tecla retorna el scancode 0 i podria donar falsos positius
static bool teclaPremuda(int tecla)
{
    return SDL_GetScancodeFromKey(tecla) != SDL_SCANCODE_UNKNOWN && Keyboard_GetKeyTrg(tecla);
}

void Tetris::gestionaVolum(double deltaTime)
{
    bool canvi = false;

    if (teclaPremuda(KEYBOARD_V)) {
        m_panelVolum = !m_panelVolum;
        m_tempsPanelVolum = 0.0;
    }

    // Amb el panel obert el joc esta aturat i les fletxes queden lliures per al volum
    bool pujaFletxa = m_panelVolum && (teclaPremuda(KEYBOARD_UP) || teclaPremuda(KEYBOARD_RIGHT));
    bool baixaFletxa = m_panelVolum && (teclaPremuda(KEYBOARD_DOWN) || teclaPremuda(KEYBOARD_LEFT));

    // '+' te codis diferents segons el teclat (espanyol: PLUS, angles: EQUALS)
    if (pujaFletxa || teclaPremuda(KEYBOARD_PLUS) || teclaPremuda(KEYBOARD_EQUALS) || teclaPremuda(KEYBOARD_KEYPAD_PLUS)) {
        Audio::changeVolume(PAS_VOLUM);
        canvi = true;
    }

    if (baixaFletxa || teclaPremuda(KEYBOARD_MINUS) || teclaPremuda(KEYBOARD_KEYPAD_MINUS)) {
        Audio::changeVolume(-PAS_VOLUM);
        canvi = true;
    }

    if (teclaPremuda(KEYBOARD_M)) {
        Audio::toggleMute();
        canvi = true;
    }

    // Si el panel esta tancat, el mostrem una estona per veure el canvi
    if (canvi)
        m_tempsPanelVolum = TEMPS_PANEL_VOLUM;
    else if (m_tempsPanelVolum > 0)
        m_tempsPanelVolum -= deltaTime;
}

void Tetris::dibuixaPanelVolum()
{
    if (!m_panelVolum && m_tempsPanelVolum <= 0)
        return;

    GraphicManager* gm = GraphicManager::getInstance();
    const int x = 150, y = 420, amplada = 300, alcada = 170;
    const int centreX = x + amplada / 2;

    // Fons semitransparent amb vora blanca
    gm->drawRect(x - 2, y - 2, amplada + 4, alcada + 4, 255, 255, 255, 255);
    gm->drawRect(x, y, amplada, alcada, 20, 20, 30, 230);

    gm->drawFontCentered(FONT_WHITE_30, centreX, y + 10, 0.8, "VOLUM");

    // Barra de volum
    const int barraX = x + 25, barraY = y + 55, barraAmplada = amplada - 50, barraAlcada = 22;
    gm->drawRect(barraX, barraY, barraAmplada, barraAlcada, 70, 70, 80, 255);

    int ple = barraAmplada * Audio::getVolume() / 100;
    if (Audio::isMuted())
        gm->drawRect(barraX, barraY, ple, barraAlcada, 110, 110, 110, 255);
    else
        gm->drawRect(barraX, barraY, ple, barraAlcada, 0, 200, 80, 255);

    if (Audio::isMuted())
        gm->drawFontCentered(FONT_RED_30, centreX, y + 85, 0.65, "SILENCI (" + to_string(Audio::getVolume()) + "%)");
    else
        gm->drawFontCentered(FONT_GREEN_30, centreX, y + 85, 0.65, to_string(Audio::getVolume()) + "%");

    if (m_panelVolum) {
        gm->drawFontCentered(FONT_WHITE_30, centreX, y + 115, 0.5, "Fletxes o +/-  volum");
        gm->drawFontCentered(FONT_WHITE_30, centreX, y + 140, 0.5, "M  silenci     V  tancar");
    }
    else {
        gm->drawFontCentered(FONT_WHITE_30, centreX, y + 115, 0.5, "+/-  volum     M  silenci");
        gm->drawFontCentered(FONT_WHITE_30, centreX, y + 140, 0.5, "V  obrir panel");
    }
}

void Tetris::juga(Screen& pantalla, int mode)
{
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    bool isPaused = false;

    pantalla.show();

    Audio::playMusic();

    m_partida.inicialitza(mode, "./data/Games/partida.txt", "./data/Games/figures.txt", "./data/Games/moviments.txt");

    bool exitGame = false;

    while (!exitGame) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

        pantalla.processEvents();

        // Esc torna al menu; tancar la finestra surt i el menu tancara l'aplicacio
        if (Keyboard_GetKeyTrg(KEYBOARD_ESCAPE) || pantalla.isExit()) {
            exitGame = true;
            continue;
        }

        if (Keyboard_GetKeyTrg(KEYBOARD_P)) {
            isPaused = !isPaused;
            if (isPaused)
                Audio::pauseMusic();
            else
                Audio::resumeMusic();
        }

        gestionaVolum(deltaTime);

        // El panel de volum obert amb V tambe atura el joc (pero no la musica)
        if (!isPaused && !m_panelVolum)
        {
            m_partida.actualitza(mode, deltaTime);
            if (m_partida.gameOver())
                exitGame = true;
        }
        else {
            m_partida.dibuixa();
            if (isPaused) {
                GraphicManager::getInstance()->drawRect(POS_X_TAULER, 250, N_COL_TAULER * MIDA_QUADRAT, 110, 0, 0, 0, 180);
                GraphicManager::getInstance()->drawFontCentered(FONT_RED_30, POS_X_TAULER + N_COL_TAULER * MIDA_QUADRAT / 2, 260, 1.5, "PAUSA");
                GraphicManager::getInstance()->drawFontCentered(FONT_WHITE_30, POS_X_TAULER + N_COL_TAULER * MIDA_QUADRAT / 2, 318, 0.6, "Prem P per continuar");
            }
            SDL_Delay(16);
        }

        dibuixaPanelVolum();
        pantalla.update();
    }

    if (m_partida.gameOver()) {
        record(pantalla);
    }

    // Detenemos la música, pero no cerramos la pantalla ni el renderizador para mantener el estado
    Audio::stopMusic();
}

typedef enum
{
    PANTALLA_MENU,
    PANTALLA_MILLORS,
    PANTALLA_CONTROLS,
    PANTALLA_PUNTS
} PantallaMenu;

static const int N_OPCIONS = 6;
static const char* OPCIONS[N_OPCIONS] = {
    "Jugar en mode normal",
    "Jugar en mode test",
    "Millors puntuacions",
    "Controls",
    "Puntuacions",
    "Sortir"
};

bool Tetris::Menu(Screen& pantalla)
{
    bool exitApp = false;
    int seleccionada = 0;
    PantallaMenu actual = PANTALLA_MENU;

    Uint64 ara = SDL_GetPerformanceCounter();
    Uint64 abans;
    double deltaTime;

    // Mostrem la finestra i li donem el focus per no haver de clicar-hi
    pantalla.show();
    SDL_RaiseWindow(g_Video.window);

    while (!exitApp) {
        abans = ara;
        ara = SDL_GetPerformanceCounter();
        deltaTime = (double)((ara - abans) / (double)SDL_GetPerformanceFrequency());

        pantalla.processEvents();

        if (pantalla.isExit()) {
            exitApp = true;
            continue;
        }

        gestionaVolum(deltaTime);

        // Amb el panel de volum obert les fletxes son per al volum
        if (!m_panelVolum)
        {
            if (actual == PANTALLA_MENU)
            {
                int opcio = -1;

                if (teclaPremuda(KEYBOARD_UP) || teclaPremuda(KEYBOARD_W))
                    seleccionada = (seleccionada + N_OPCIONS - 1) % N_OPCIONS;
                if (teclaPremuda(KEYBOARD_DOWN) || teclaPremuda(KEYBOARD_S))
                    seleccionada = (seleccionada + 1) % N_OPCIONS;
                if (teclaPremuda(KEYBOARD_ESCAPE))
                    seleccionada = N_OPCIONS - 1;   // Porta el cursor a "Sortir"

                if (teclaPremuda(KEYBOARD_RETURN) || teclaPremuda(KEYBOARD_KEYPAD_ENTER) || teclaPremuda(KEYBOARD_SPACE))
                    opcio = seleccionada;

                // Tambe es pot triar directament amb el numero
                for (int i = 0; i < N_OPCIONS; i++)
                {
                    if (teclaPremuda(KEYBOARD_1 + i) || teclaPremuda(KEYBOARD_KEYPAD_1 + i)) {
                        seleccionada = i;
                        opcio = i;
                    }
                }

                switch (opcio)
                {
                case 0:
                    juga(pantalla, 1);
                    break;
                case 1:
                    juga(pantalla, 0);
                    break;
                case 2:
                    actual = PANTALLA_MILLORS;
                    break;
                case 3:
                    actual = PANTALLA_CONTROLS;
                    break;
                case 4:
                    actual = PANTALLA_PUNTS;
                    break;
                case 5:
                    exitApp = true;
                    break;
                }

                // Despres d'una partida no comptem el temps jugat com a deltaTime del menu
                if (opcio == 0 || opcio == 1)
                    ara = SDL_GetPerformanceCounter();
            }
            else if (teclaPremuda(KEYBOARD_ESCAPE) || teclaPremuda(KEYBOARD_RETURN) || teclaPremuda(KEYBOARD_KEYPAD_ENTER) ||
                     teclaPremuda(KEYBOARD_SPACE) || teclaPremuda(KEYBOARD_BACKSPACE))
            {
                actual = PANTALLA_MENU;
            }
        }

        if (exitApp || pantalla.isExit())
            continue;

        switch (actual)
        {
        case PANTALLA_MENU:
            dibuixaMenu(seleccionada);
            break;
        case PANTALLA_MILLORS:
            mostraPuntuacions();
            break;
        case PANTALLA_CONTROLS:
            menuControls();
            break;
        case PANTALLA_PUNTS:
            menuPuntuacions();
            break;
        }

        dibuixaPanelVolum();
        pantalla.update();
        SDL_Delay(16);
    }

    return true;
}

void Tetris::dibuixaMenu(int seleccionada)
{
    GraphicManager* gm = GraphicManager::getInstance();
    const float centreX = SCREEN_SIZE_X / 2.0f;

    gm->drawSprite(GRAFIC_FONS, 0, 0, false);
    gm->drawFontCentered(FONT_RED_30, centreX, 70, 2.5f, "TETRIS");

    for (int i = 0; i < N_OPCIONS; i++)
    {
        float y = 220.0f + i * 60.0f;
        string text = to_string(i + 1) + ".  " + OPCIONS[i];

        if (i == seleccionada)
        {
            gm->drawRect(100, (int)y - 8, SCREEN_SIZE_X - 200, 50, 40, 40, 90, 255);
            gm->drawFontCentered(FONT_GREEN_30, centreX, y, 0.9f, "> " + text + " <");
        }
        else
            gm->drawFontCentered(FONT_WHITE_30, centreX, y, 0.9f, text);
    }

    gm->drawFontCentered(FONT_WHITE_30, centreX, SCREEN_SIZE_Y - 60, 0.5f, "Fletxes: moure     Enter: triar     1-6: opcio directa     V: volum");
}

// Pantalla amb titol i una llista de linies, comuna a controls i puntuacions
static void dibuixaPantallaText(const string& titol, const char* const linies[], int nLinies)
{
    GraphicManager* gm = GraphicManager::getInstance();

    gm->drawSprite(GRAFIC_FONS, 0, 0, false);
    gm->drawRect(60, 60, SCREEN_SIZE_X - 120, SCREEN_SIZE_Y - 140, 20, 20, 45, 255);
    gm->drawFontCentered(FONT_RED_30, SCREEN_SIZE_X / 2.0f, 85, 1.2f, titol);

    for (int i = 0; i < nLinies; i++)
        gm->drawFont(FONT_WHITE_30, 100, 160.0f + i * 40.0f, 0.65f, linies[i]);
}

void Tetris::dibuixaPeuSubmenu()
{
    GraphicManager::getInstance()->drawFontCentered(FONT_GREEN_30, SCREEN_SIZE_X / 2.0f, SCREEN_SIZE_Y - 60, 0.55f, "Esc / Enter: tornar al menu");
}

void Tetris::menuControls()
{
    static const char* const linies[] = {
        "Fletxa amunt / W:  girar horari",
        "Fletxa avall / S:  girar antihorari",
        "Fletxa esquerra / A:  moure esquerra",
        "Fletxa dreta / D:  moure dreta",
        "C:  baixar figura",
        "Espai:  baixar de cop",
        "P:  pausa",
        "V:  panel de volum (atura el joc)",
        "+ / -:  volum        M:  silenci",
        "Esc:  tornar al menu"
    };

    dibuixaPantallaText("CONTROLS", linies, sizeof(linies) / sizeof(linies[0]));
    dibuixaPeuSubmenu();
}

void Tetris::menuPuntuacions()
{
    static const char* const linies[] = {
        "Eliminar 1 fila:  +100 punts",
        "Eliminar 2 files:  +150 punts",
        "Eliminar 3 files:  +175 punts",
        "Eliminar 4 files:  +200 punts",
        "Figura colocada:  +10 punts",
        "Baixar de cop:  +30 punts"
    };

    dibuixaPantallaText("PUNTUACIONS", linies, sizeof(linies) / sizeof(linies[0]));
    dibuixaPeuSubmenu();
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
    GraphicManager* gm = GraphicManager::getInstance();

    gm->drawSprite(GRAFIC_FONS, 0, 0, false);
    gm->drawRect(60, 60, SCREEN_SIZE_X - 120, SCREEN_SIZE_Y - 140, 20, 20, 45, 255);
    gm->drawFontCentered(FONT_RED_30, SCREEN_SIZE_X / 2, 85, 1.2f, "MILLORS PUNTUACIONS");

    if (m_puntuacions.empty())
        gm->drawFontCentered(FONT_WHITE_30, SCREEN_SIZE_X / 2, 300, 0.7f, "Encara no hi ha puntuacions");

    list<Puntuacio>::iterator actual = m_puntuacions.begin();
    int num = 0;

    while (actual != m_puntuacions.end() && num < MAX_PUNTUACIONS_MOSTRADES)
    {
        FONT_NAME font = (num == 0) ? FONT_GREEN_30 : FONT_WHITE_30;
        float y = 160.0f + num * 42.0f;

        gm->drawFont(font, 110, y, 0.75f, to_string(num + 1) + ".");
        gm->drawFont(font, 160, y, 0.75f, (*actual).nom);
        gm->drawFont(font, 390, y, 0.75f, to_string((*actual).punts));

        actual++;
        num++;
    }

    dibuixaPeuSubmenu();
}

void Tetris::escriureFitxer(Puntuacio& usuari, const string& path)
{
    ofstream fitxer(path, ios::app);

    if (fitxer.is_open()) {
        fitxer << usuari.nom << " " << to_string(usuari.punts) << endl;
        fitxer.close();
    }
}

void Tetris::record(Screen& pantalla)
{
    GraphicManager* gm = GraphicManager::getInstance();
    const float centreX = SCREEN_SIZE_X / 2.0f;
    const int punts = m_partida.getPunts();
    const bool nouRecord = m_puntuacions.empty() || punts > m_puntuacions.front().punts;

    string nom;
    bool acabat = false;
    bool guardar = false;
    Uint32 inici = SDL_GetTicks();

    SDL_StartTextInput();

    while (!acabat) {
        pantalla.processEvents();

        if (pantalla.isExit()) {
            acabat = true;
            continue;
        }

        // Ignorem les tecles dels primers TEMPS_ESPERA_NOM ms: el jugador potser
        // encara premia Espai o una fletxa quan s'ha acabat la partida
        if (SDL_GetTicks() - inici > TEMPS_ESPERA_NOM)
        {
            // Nomes lletres, numeros, '_' i '-': el fitxer separa nom i punts per espais
            for (const char* c = Keyboard_GetText(); *c != '\0'; c++)
            {
                unsigned char lletra = (unsigned char)*c;

                if (nom.length() >= MAX_LLARGADA_NOM)
                    break;
                if (isalnum(lletra) || lletra == '_' || lletra == '-')
                    nom += (char)lletra;
                else if (lletra == ' ')
                    nom += '_';
            }

            if (teclaPremuda(KEYBOARD_BACKSPACE) && !nom.empty())
                nom.pop_back();

            if ((teclaPremuda(KEYBOARD_RETURN) || teclaPremuda(KEYBOARD_KEYPAD_ENTER)) && !nom.empty()) {
                guardar = true;
                acabat = true;
            }

            if (teclaPremuda(KEYBOARD_ESCAPE))
                acabat = true;
        }

        // Pantalla de game over amb el camp per escriure el nom
        gm->drawSprite(GRAFIC_FONS, 0, 0, false);
        gm->drawFontCentered(FONT_RED_30, centreX, 110, 1.75f, "GAME OVER");
        gm->drawFontCentered(FONT_WHITE_30, centreX, 210, 0.8f, "La teva puntuacio: " + to_string(punts));
        if (nouRecord)
            gm->drawFontCentered(FONT_GREEN_30, centreX, 255, 0.8f, "NOU RECORD!");

        gm->drawFontCentered(FONT_WHITE_30, centreX, 340, 0.7f, "Escriu el teu nom:");

        const int caixaX = 120, caixaY = 385, caixaAmplada = SCREEN_SIZE_X - 240, caixaAlcada = 50;
        gm->drawRect(caixaX - 2, caixaY - 2, caixaAmplada + 4, caixaAlcada + 4, 255, 255, 255, 255);
        gm->drawRect(caixaX, caixaY, caixaAmplada, caixaAlcada, 20, 20, 45, 255);

        // Cursor parpellejant
        bool cursor = (SDL_GetTicks() / 500) % 2 == 0 && nom.length() < MAX_LLARGADA_NOM;
        gm->drawFontCentered(FONT_GREEN_30, centreX, caixaY + 8, 0.9f, nom + (cursor ? "_" : " "));

        if (nom.empty())
            gm->drawFontCentered(FONT_WHITE_30, centreX, 460, 0.5f, "Lletres, numeros, _ i -  (maxim " + to_string(MAX_LLARGADA_NOM) + ")");
        else
            gm->drawFontCentered(FONT_WHITE_30, centreX, 460, 0.5f, "Retroces: esborrar");

        gm->drawFontCentered(FONT_WHITE_30, centreX, SCREEN_SIZE_Y - 60, 0.55f, "Enter: guardar          Esc: no guardar");

        pantalla.update();
        SDL_Delay(16);
    }

    SDL_StopTextInput();

    if (guardar)
    {
        Puntuacio pts;
        pts.nom = nom;
        pts.punts = punts;

        guardarPuntuacio(pts, true);
    }
}
