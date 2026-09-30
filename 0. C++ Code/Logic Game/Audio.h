#ifndef AUDIO_H
#define AUDIO_H

#include <string>

// Gestio de la musica i els efectes de so amb MCI (winmm).
// Permet pausar, reprendre i canviar el volum, cosa que PlaySound no permet.
class Audio
{
public:
    static void init(const std::string& fitxerConfig);
    static void shutdown();

    static void playMusic();     // Comenca la musica des del principi, en bucle
    static void stopMusic();
    static void pauseMusic();
    static void resumeMusic();
    static void playGameOver();

    static void setVolume(int volum);   // 0..100
    static void changeVolume(int delta);
    static int getVolume() { return s_volum; }
    static void toggleMute();
    static bool isMuted() { return s_silenci; }

private:
    static void aplicaVolum();
    static void guardaConfig();
    static bool obre(const std::string& fitxer, const std::string& alias);

    static int s_volum;
    static bool s_silenci;
    static bool s_musicaOberta;
    static bool s_gameOverObert;
    static bool s_musicaSonant;
    static std::string s_fitxerConfig;
};

#endif