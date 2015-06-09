#ifndef HEADERS_CARTELES_H_
#define HEADERS_CARTELES_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Ventana.h>

#define PATH_FIGHT		"Images/fight!.png"
#define PATH_FATALITY	"Images/fatality.png"
#define PATH_BABALITY	"Images/babality.png"

#define PATH_FONT		"Images/mortalkombat3.ttf"

#define DELAY_SHOWING_FATALITY	1300
#define DELAY_SHOWING_FIGHT		500

class Carteles
{
	private:
		Ventana* _ventana;
		
		TextureHandler* _handlerFight;
		TextureHandler* _handlerFatality;
		TextureHandler* _handlerBabality;
		TextureHandler* _handlerWinner;

		SDL_Color colorNombres;
		TTF_Font* fontNombres;
		
	public:
		Carteles(Ventana* ventana);
		~Carteles(void);
		void viewFigth();
		void viewFatality();
		void viewBabality();
		void viewWinner(string nombre);
};

#endif /* HEADERS_CARTELES_H_ */