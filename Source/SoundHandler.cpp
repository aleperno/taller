#include <SoundHandler.h>
#include <stdio.h>
#include <Logger.h>

SoundHandler::SoundHandler()
{
	this->musicaMenu = Mix_LoadMUS("Sound/mk3-menu_music.mp3");
	if(musicaMenu == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de m�sica");

	this->musicaPelea = Mix_LoadMUS("Sound/mk3-fight_music.mp3");
	if(musicaPelea == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de m�sica");

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

	this->aleluya = Mix_LoadWAV("Sound/Aleluya.wav");
	if(aleluya == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido Aleluya");

	this->toasty = Mix_LoadWAV("Sound/toasty.wav");
	if(toasty == NULL)
		Logger::Instance()->log(ERROR,"No se pudo cargar archivo de sonido toasty");
	
	this->fight = Mix_LoadWAV("Sound/shaokahn/fight.wav");
	this->roundOne = Mix_LoadWAV("Sound/shaokahn/roundOne.wav");
	this->roundTwo = Mix_LoadWAV("Sound/shaokahn/roundTwo.wav");
	this->roundThree = Mix_LoadWAV("Sound/shaokahn/roundThree.wav");
	this->scorpion_wins = Mix_LoadWAV("Sound/shaokahn/scorpion_wins.wav");
	this->liukang_wins = Mix_LoadWAV("Sound/shaokahn/liukang_wins.wav");
	this->fatality = Mix_LoadWAV("Sound/shaokahn/fatality.wav");
	this->finish_him = Mix_LoadWAV("Sound/shaokahn/finish_him.wav");
	this->excellent = Mix_LoadWAV("Sound/shaokahn/excellent.wav");
	this->outstanding = Mix_LoadWAV("Sound/shaokahn/outstanding.wav");
	this->babality = Mix_LoadWAV("Sound/shaokahn/babality.wav");
	this->flawless_victory = Mix_LoadWAV("Sound/shaokahn/flawless_victory.wav");

}

SoundHandler::~SoundHandler()
{	
	Mix_FreeChunk(fight); fight = NULL;
	Mix_FreeChunk(roundOne); roundOne = NULL;
	Mix_FreeChunk(roundTwo); roundTwo = NULL;
	Mix_FreeChunk(roundThree); roundThree = NULL;
	Mix_FreeChunk(scorpion_wins); scorpion_wins = NULL;
	Mix_FreeChunk(liukang_wins); liukang_wins = NULL;
	Mix_FreeChunk(fatality); fatality = NULL;
	Mix_FreeChunk(finish_him); finish_him = NULL;
	Mix_FreeChunk(excellent); excellent = NULL;
	Mix_FreeChunk(outstanding); outstanding = NULL;
	Mix_FreeChunk(babality); babality = NULL;
	Mix_FreeChunk(flawless_victory); flawless_victory = NULL;
	Mix_FreeChunk( miss );
	Mix_FreeChunk( hit );
	Mix_FreeChunk( gancho );
	Mix_FreeChunk( fire );
	Mix_FreeChunk( caida );
	Mix_FreeChunk( jump );


	Mix_FreeChunk( select );
	Mix_FreeChunk( selected );
	Mix_FreeChunk( inicio );
	Mix_FreeChunk(aleluya);

	Mix_FreeChunk(toasty);

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
	aleluya = NULL;

	musicaMenu = NULL;
	musicaPelea = NULL;
	toasty = NULL;
}
