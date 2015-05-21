#include "Temporizador.h"

Temporizador::Temporizador() {
}

Temporizador::~Temporizador() {
}

void Temporizador::reset() {
	this->startTick = SDL_GetTicks();
	this->pauseTick = this->startTick;
	this->pauseTicksAccumulator = 0;
}

void Temporizador::pause() {
	this->pauseTick = SDL_GetTicks();
}

void Temporizador::unpause() {
	Uint32 pauseTime = SDL_GetTicks() - this->pauseTick;	//tiempo en pausa
	this->pauseTicksAccumulator = this->pauseTicksAccumulator + pauseTime;
}

Uint32 Temporizador::getTimeInTicks() {
	return SDL_GetTicks() - this->startTick - this->pauseTicksAccumulator;
}