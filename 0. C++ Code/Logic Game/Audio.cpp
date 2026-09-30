#include "Audio.h"
#include <windows.h>
#include <mmsystem.h>
#include <fstream>
#include <cstdio>

#pragma comment(lib, "winmm.lib")

using namespace std;

int Audio::s_volum = 70;
bool Audio::s_silenci = false;
bool Audio::s_musicaOberta = false;
bool Audio::s_gameOverObert = false;
bool Audio::s_musicaSonant = false;
string Audio::s_fitxerConfig;

static const char* FITXER_MUSICA = "music.wav";
static const char* FITXER_GAME_OVER = "gameover.wav";

static bool mci(const string& ordre)
{
    return mciSendStringA(ordre.c_str(), NULL, 0, NULL) == 0;
}

bool Audio::obre(const string& fitxer, const string& alias)
{
    // MCI necessita la ruta completa; el tipus mpegvideo permet controlar el volum
    char rutaCompleta[MAX_PATH];
    if (GetFullPathNameA(fitxer.c_str(), MAX_PATH, rutaCompleta, NULL) == 0)
        return false;

    if (GetFileAttributesA(rutaCompleta) == INVALID_FILE_ATTRIBUTES)
        return false;

    return mci("open \"" + string(rutaCompleta) + "\" type mpegvideo alias " + alias);
}

void Audio::init(const string& fitxerConfig)
{
    s_fitxerConfig = fitxerConfig;

    // Llegim el volum guardat de l'ultima partida
    ifstream fitxer(s_fitxerConfig);
    if (fitxer.is_open())
    {
        int volum, silenci;
        if (fitxer >> volum >> silenci)
        {
            s_volum = volum < 0 ? 0 : (volum > 100 ? 100 : volum);
            s_silenci = (silenci != 0);
        }
        fitxer.close();
    }

    s_musicaOberta = obre(FITXER_MUSICA, "musica");
    s_gameOverObert = obre(FITXER_GAME_OVER, "gameover");
    aplicaVolum();
}

void Audio::shutdown()
{
    guardaConfig();
    mci("close all");
    s_musicaOberta = s_gameOverObert = s_musicaSonant = false;
}

void Audio::playMusic()
{
    if (s_gameOverObert)
        mci("stop gameover");

    if (s_musicaOberta)
    {
        s_musicaSonant = mci("play musica from 0 repeat");

        // Si el controlador no accepta "repeat", fem servir PlaySound per a la musica
        if (!s_musicaSonant)
        {
            mci("close musica");
            s_musicaOberta = false;
        }
    }

    if (!s_musicaOberta)
    {
        // Alternativa si MCI no pot obrir el fitxer: sense control de volum
        s_musicaSonant = PlaySoundA(FITXER_MUSICA, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP) != FALSE;
    }
}

void Audio::stopMusic()
{
    if (s_musicaOberta)
        mci("stop musica");
    else
        PlaySoundA(NULL, NULL, 0);

    s_musicaSonant = false;
}

void Audio::pauseMusic()
{
    if (!s_musicaSonant)
        return;

    if (s_musicaOberta)
        mci("pause musica");
    else
        PlaySoundA(NULL, NULL, 0);
}

void Audio::resumeMusic()
{
    if (!s_musicaSonant)
        return;

    if (s_musicaOberta)
    {
        // Alguns controladors no accepten "resume": en aquest cas continuem amb "play"
        if (!mci("resume musica"))
            mci("play musica repeat");
    }
    else
    {
        PlaySoundA(FITXER_MUSICA, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
}

void Audio::playGameOver()
{
    stopMusic();

    if (s_gameOverObert)
        mci("play gameover from 0");
    else if (!s_silenci)
        PlaySoundA(FITXER_GAME_OVER, NULL, SND_FILENAME | SND_ASYNC);
}

void Audio::setVolume(int volum)
{
    if (volum < 0) volum = 0;
    if (volum > 100) volum = 100;

    s_volum = volum;
    if (s_volum > 0)
        s_silenci = false;

    aplicaVolum();
}

void Audio::changeVolume(int delta)
{
    setVolume(s_volum + delta);
}

void Audio::toggleMute()
{
    s_silenci = !s_silenci;
    aplicaVolum();
}

void Audio::aplicaVolum()
{
    // MCI fa servir una escala de 0 a 1000
    int valor = s_silenci ? 0 : s_volum * 10;
    char ordre[64];

    if (s_musicaOberta)
    {
        snprintf(ordre, sizeof(ordre), "setaudio musica volume to %d", valor);
        mci(ordre);
    }
    if (s_gameOverObert)
    {
        snprintf(ordre, sizeof(ordre), "setaudio gameover volume to %d", valor);
        mci(ordre);
    }
}

void Audio::guardaConfig()
{
    if (s_fitxerConfig.empty())
        return;

    ofstream fitxer(s_fitxerConfig);
    if (fitxer.is_open())
    {
        fitxer << s_volum << " " << (s_silenci ? 1 : 0) << endl;
        fitxer.close();
    }
}