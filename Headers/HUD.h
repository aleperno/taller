/*
 * HUD.h
 *
 *  Created on: May 2, 2015
 *      Author: neo
 */

#ifndef HEADERS_HUD_H_
#define HEADERS_HUD_H_

#include <Ventana.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Personaje.h>

#define FONT_PATH "Images/ardestine.ttf"

class Hud{
	public:
		Hud(Ventana* ventana, Personaje* _personaje1, Personaje* _personaje2);
		~Hud();
		void prepararHUD();
		void actualizarHealthbars();
		void printHUD();
	private:
		TTF_Font* font;
		Ventana* _ventana;
		Personaje* _personaje1;
		Personaje* _personaje2;
		struct hudPersonaje {
			SDL_Rect externo;
			SDL_Rect externoIlum;
			SDL_Rect interno;
			SDL_Rect healthIlum;
			SDL_Rect health;
			TextureHandler* nombreTexture;
			SDL_Rect nombre;
		};
		hudPersonaje hud1;
		hudPersonaje hud2;
};



#endif /* HEADERS_HUD_H_ */
