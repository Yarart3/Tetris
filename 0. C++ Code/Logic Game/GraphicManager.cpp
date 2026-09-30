//
//  GraphicManager.cpp
//  MP_Practica
//
//  Created by Enric Vergara on 13/03/2021.
//  Copyright © 2021 CompiledCreations. All rights reserved.
//

#include "GraphicManager.h"
#include <iostream>

GraphicManager* GraphicManager::instance=NULL;

GraphicManager::GraphicManager()
: m_oFontWhite_30(getFontPath(FONT_WHITE_30).c_str(),30,NFont::Color(255,255,255,255))
, m_oFontRed_30(getFontPath(FONT_RED_30).c_str(),30,NFont::Color(255,0,0,255))
, m_oFontGreen_30(getFontPath(FONT_GREEN_30).c_str(),30,NFont::Color(0,255,0,255))
{
    for (int i = 0; i < GRAFIC_NUM_MAX; i ++){
        string path = getImagePath((IMAGE_NAME)i);
        m_aSprites[i].create(path.c_str());
    }
}


void GraphicManager::drawSprite (IMAGE_NAME name, float posX, float posY, bool centered) 
{
    if (name >= 0 && name < GRAFIC_NUM_MAX)
    {
        if (centered)
        {
            posX -= m_aSprites[name].getScaleX() / 2;
            posY -= m_aSprites[name].getScaleY() / 2;
        }
        m_aSprites[name].draw(posX, posY);
    }
    else
    {
        cout << "Error: no existeix el identificador de image";
    }
    
}

NFont::Rectf GraphicManager::drawFont (FONT_NAME name,
                                       float posX, float posY,
                                       float size,
                                       const string& msg)
{
    switch (name) {
        case FONT_GREEN_30:
            return m_oFontGreen_30.draw(posX, posY, NFont::Scale(size), "%s", msg.c_str());
            break;
        case FONT_RED_30:
            return m_oFontRed_30.draw(posX, posY, NFont::Scale(size), "%s", msg.c_str());
            break;
        case FONT_WHITE_30:
            return m_oFontWhite_30.draw(posX, posY, NFont::Scale(size), "%s", msg.c_str());
            break;
            
        default:
            cout << "Error: no existeix el identificador de font";
            break;
    }
    return NFont::Rectf();
}

NFont::Rectf GraphicManager::drawFontCentered(FONT_NAME name,
                                              float centerX, float posY,
                                              float size,
                                              const string& msg)
{
    NFont* font = getFont(name);
    if (font == NULL)
    {
        cout << "Error: no existeix el identificador de font";
        return NFont::Rectf();
    }

    float amplada = font->getWidth("%s", msg.c_str()) * size;
    return font->draw(centerX - amplada / 2, posY, NFont::Scale(size), "%s", msg.c_str());
}

void GraphicManager::drawRect(int posX, int posY, int width, int height,
                              int r, int g, int b, int alpha)
{
    SDL_Rect rect = { posX, posY, width, height };

    SDL_SetRenderDrawBlendMode(g_Video.renderer, alpha < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(g_Video.renderer, (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)alpha);
    SDL_RenderFillRect(g_Video.renderer, &rect);
    SDL_SetRenderDrawBlendMode(g_Video.renderer, SDL_BLENDMODE_NONE);
}

NFont* GraphicManager::getFont(FONT_NAME name)
{
    switch (name) {
        case FONT_GREEN_30:
            return &m_oFontGreen_30;
        case FONT_RED_30:
            return &m_oFontRed_30;
        case FONT_WHITE_30:
            return &m_oFontWhite_30;
        default:
            return NULL;
    }
}

string GraphicManager::getImagePath(IMAGE_NAME name){
    string path;
    switch (name) {
    case GRAFIC_FONS:
        path = "data/Graphics/fons.png";
        break;
    case GRAFIC_TAULER:
        path = "data/Graphics/tauler.png";
        break;
    case GRAFIC_QUADRAT_GROC:
        path = "data/Graphics/quadrat_groc.png";
        break;
    case GRAFIC_QUADRAT_BLAUCEL:
        path = "data/Graphics/quadrat_blaucel.png";
        break;
    case GRAFIC_QUADRAT_MAGENTA:
        path = "data/Graphics/quadrat_magenta.png";
        break;
    case GRAFIC_QUADRAT_TARONJA:
        path = "data/Graphics/quadrat_taronja.png";
        break;
    case GRAFIC_QUADRAT_BLAUFOSC:
        path = "data/Graphics/quadrat_blaufosc.png";
        break;
    case GRAFIC_QUADRAT_VERMELL:
        path = "data/Graphics/quadrat_vermell.png";
        break;
    case GRAFIC_QUADRAT_VERD:
        path = "data/Graphics/quadrat_verd.png";
        break;
    default:
        path = "";
        break;

    }
    
    
    
    return path;
}
string GraphicManager::getFontPath(FONT_NAME name){
    string path;
    switch (name) {
        case FONT_GREEN_30:
        case FONT_RED_30:
        case FONT_WHITE_30:
            path = "data/Fonts/FreeSans.ttf";
            break;
            
        default:
            path = "";
            break;
    }
    
    return path;
}

