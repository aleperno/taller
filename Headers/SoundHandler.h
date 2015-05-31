#include <SDL_mixer.h>

#ifndef HEADERS_SOUNDHANDLER_H_
#define HEADERS_SOUNDHANDLER_H_

using namespace std;

class SoundHandler {
	public:
		//Musica
		Mix_Music *musicaMenu;
		Mix_Music *musicaPelea;

		//Efectos de sonido - Pelea
		Mix_Chunk *miss;
		Mix_Chunk *hit;
		Mix_Chunk *gancho;
		Mix_Chunk *fire;
		Mix_Chunk *caida;

		//Efectos de sonido - Menu
		Mix_Chunk *select;
		Mix_Chunk *selected;
		Mix_Chunk *inicio;

		SoundHandler();
		~SoundHandler();

		//void PlaySonido(
};

#endif /* HEADERS_SOUNDHANDLER_H_ */