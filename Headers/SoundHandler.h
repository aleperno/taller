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
		Mix_Chunk *jump;

		//Efectos de sonido - Menu
		Mix_Chunk *select;
		Mix_Chunk *selected;
		Mix_Chunk *inicio;
		Mix_Chunk* aleluya;
		Mix_Chunk* fight;
		Mix_Chunk* roundOne;
		Mix_Chunk* roundTwo;
		Mix_Chunk* roundThree;
		Mix_Chunk* scorpion_wins;
		Mix_Chunk* liukang_wins;
		Mix_Chunk* fatality;
		Mix_Chunk* finish_him;
		Mix_Chunk* outstanding;
		Mix_Chunk* excellent;
		Mix_Chunk* babality;
		Mix_Chunk* flawless_victory;

		//Toasty
		Mix_Chunk *toasty;

		SoundHandler();
		~SoundHandler();

		//void PlaySonido(
};

#endif /* HEADERS_SOUNDHANDLER_H_ */
