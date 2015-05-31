#include <SoundHandler.h>
#include <stdio.h>

SoundHandler::SoundHandler()
{
	this->musicaMenu = Mix_LoadMUS("Sound/mk3-menu_music.mp3");
	if(musicaMenu == NULL)
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
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