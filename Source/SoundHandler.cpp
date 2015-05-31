#include <SoundHandler.h>

SoundHandler::SoundHandler()
{

}

SoundHandler::~SoundHandler()
{
	Mix_FreeChunk( miss );
	Mix_FreeChunk( hit );
	Mix_FreeChunk( gancho );
	Mix_FreeChunk( fire );
	Mix_FreeChunk( caida );

	Mix_FreeChunk( select );
	Mix_FreeChunk( selected );
	Mix_FreeChunk( inicio );

	Mix_FreeMusic( musicaMenu );
	Mix_FreeMusic( musicaPelea );

	miss = NULL;
	hit = NULL;
	gancho = NULL;
	fire = NULL;
	caida = NULL;

	select = NULL;
	selected = NULL;
	inicio = NULL;

	musicaMenu = NULL;
	musicaPelea = NULL;
}