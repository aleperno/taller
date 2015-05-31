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

	this->hit = Mix_LoadWAV("Sound/mk3-hit.wav");
	if(hit == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido");

	this->jump = Mix_LoadWAV("Sound/mk3-jump.wav");
	if(jump == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido");

	this->caida = Mix_LoadWAV("Sound/mk3-fall.wav");
	if(caida == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido");

	this->gancho = Mix_LoadWAV("Sound/mk3-hit_gancho.wav");
	if(gancho == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido");

	this->miss = Mix_LoadWAV("Sound/mk3-miss.wav");
	if(miss == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido");

	this->inicio = Mix_LoadWAV("Sound/mk3-init.wav");
	if(inicio == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido");

	this->select = Mix_LoadWAV("Sound/mk3-select_player.wav");
	if(select == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido");

	this->selected = Mix_LoadWAV("Sound/mk3-selected.wav");
	if(selected == NULL)
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