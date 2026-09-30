#include "libreria.h"
#include "keyboard.h"
#include "debug.h"

struct T_KEYBOARD g_Keyboard ;


int Keyboard_GetKeyCnt(int cual)
{
  return(g_Keyboard.cnt[SDL_GetScancodeFromKey(cual)]) ;
}

int Keyboard_GetKeyTrg(int cual)
{
  return(g_Keyboard.trg[SDL_GetScancodeFromKey(cual)]) ;
}

void Keyboard_ProcessKeyDown(int tecla)
{
  g_Keyboard.trg[SDL_GetScancodeFromKey(tecla)] = true;
  g_Keyboard.cnt[SDL_GetScancodeFromKey(tecla)] = true;
}

void Keyboard_ProcessKeyUp(int tecla)
{
  g_Keyboard.cnt[SDL_GetScancodeFromKey(tecla)] = false;
}

void Keyboard_ProcessText(const char* text)
{
  size_t usat = strlen(g_Keyboard.text);
  strncat(g_Keyboard.text, text, sizeof(g_Keyboard.text) - usat - 1);
}

const char* Keyboard_GetText()
{
  return g_Keyboard.text;
}

void Keyboard_Init()
{
  memset(&g_Keyboard, 0, sizeof(struct T_KEYBOARD)) ;
}

void Keyboard_Tick()
{
  memset(g_Keyboard.trg, 0, sizeof(int) * NUM_KEYBOARD_KEY_CODES) ;
  g_Keyboard.text[0] = '\0';
}

