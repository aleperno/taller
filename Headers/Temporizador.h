#ifndef HEADERS_TEMPORIZADOR_H_
#define HEADERS_TEMPORIZADOR_H_

#include <SDL.h>

class Temporizador{
public:
	Temporizador();
	~Temporizador();
	void reset();
	void pause();
	void unpause();
	Uint32 getTimeInTicks();

private:
	Uint32 startTick;
	Uint32 pauseTick;
	Uint32 pauseTicksAccumulator;
};

#endif /* HEADERS_TEMPORIZADOR_H_ */