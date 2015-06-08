#ifndef HEADERS_CARTELES_H_
#define HEADERS_CARTELES_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Ventana.h>

#define PATH_FIGHT		"Images/fight!.png"
#define PATH_FATALITY	"Images/fatality.png"
#define PATH_BABALITY	"Images/babality.png"

#define DELAY_SHOWING	400

class Carteles
{
	private:
		Ventana* _ventana;
		
		TextureHandler* _handlerFight;
		TextureHandler* _handlerFatality;
		TextureHandler* _handlerBabality;
		
	public:
		Carteles(Ventana* ventana);
		~Carteles(void);
		void viewFigth();
		void viewFatality();
		void viewBabality();
};

#endif /* HEADERS_CARTELES_H_ */