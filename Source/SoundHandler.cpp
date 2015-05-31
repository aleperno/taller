#include <SoundHandler.h>
#include <stdio.h>
#include <Logger.h>

SoundHandler::SoundHandler()
{
	this->musicaMenu = Mix_LoadMUS("Sound/mk3-menu_music.mp3");
	if(musicaMenu == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de música");

	this->musicaPelea = Mix_LoadMUS("Sound/mk3-fight_music.mp3");
	if(musicaPelea == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de música");

	this->fire = Mix_LoadWAV("Sound/mk3-fire.wav");
	if(fire == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido");
}

SoundHandler::~SoundHandler()
{
	Mix_FreeChunk( miss );
	Mix_FreeChunk( hit );
	Mix_FreeChunk( gancho );
	Mix_FreeChunk( fire );
	Mix_FreeChunk( caida );
	Mix_FreeChunk( jump );

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