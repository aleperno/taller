/*
 * Personaje.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: neo
 */
#include <Personaje.h>

Personaje::Personaje(Ventana* ventana, PersonajeData data, EscenarioData escenario, bool pers_ppal, bool cambiarColor)
{
	Logger::Instance()->log(DEBUG,"Se crea personaje");
	this->_ventana = ventana;
	this->_handler = new TextureHandler(ventana->_gRenderer);
	this->_alto_log = data.alto;
	this->_ancho_log = data.ancho;
	string path = data.imgPath;
	this->_handler->loadFromFile(path,cambiarColor,data.h_inicial,data.h_final,data.desplazamiento,true);
	this-> _escenario = escenario;
	this-> _factor_escala = escenario.ancho / this->_ancho_log;
	this->_pos_y = escenario.y_piso;
	if (pers_ppal)
		this->_pos_x = (escenario.ancho - _ancho_log) /2 - (ventana->_ancho_log/4);
	else
		this->_pos_x = (escenario.ancho - _ancho_log) /2 + (ventana->_ancho_log/4);
	this->_ancho_px = Personaje::getWidth(ventana,_ancho_log);
	this->_alto_px = Personaje::getHeight(ventana,_alto_log);
	this->_zIndex = escenario.z_index;
	this->_personajeData = data;
	this->vectorSprites = Personaje::loadVectorMedia(data);
	this->_lastFrame = 0;
	this->healthPoints = 100;
	this->_isWalking = false;
	this->_isDucking = false;
	this->_isJumping = false;
	this->_isJumpingRight = false;
	this->_isJumpingLeft = false;
	this->_isFalling = false;
	this->_isFallingRight = false;
	this->_isFallingLeft = false;
	
	this->_isThrowing = false;
	this->_weaponInAir = false;
	this->_timesThrow = 0;

	this->_isBlocking = false;
	this->_isDizzy = false;
	this->_canMove = true;
	this->_beingHit = false;
	this->_isHiKicking = false;
	this->_isLoKicking = false;
	this->_isHiPunching = false;
	this->_isLoPunching = false;

	this->pos_last_action = 0;

	this->_orientacion = _personajeData.orientacion;
	this->setBoundingBox();

	this-> _data = data;

	//TODO: Estoy hardcodeando el ancho y alto del arma, a un sexto de lo que mide el personaje
	this->arma = new Arma("Images/characters/Fireball.png", _alto_log/6, _alto_log/6, _factor_escala, _ventana, _zIndex);
	//cout << _pos_x << endl;
}

PersonajeData* Personaje::getData() {
	return &this->_data;
}

bool Personaje::hayColision( SDL_Rect boundingBox_1, SDL_Rect boundingBox_2 )
{
	float factor_cercania = 0.12f;
	bool colision = true;

	//Calculo los lados de cada bounding box, izquierda, derecha, arriba y abajo
    int izqBB1 = boundingBox_1.x;
	int izqBB2 = boundingBox_2.x;
    int rgtBB1 = boundingBox_1.x + boundingBox_1.w;
	int rgtBB2 = boundingBox_2.x + boundingBox_2.w;;
    
	int upBB1 = boundingBox_1.y;
	int upBB2 = boundingBox_2.y;
	int downBB1 = boundingBox_1.y + boundingBox_1.h;
    int downBB2 = boundingBox_2.y + boundingBox_2.h;

	//Chequeo las condiciones de no-colision
    if( downBB1 <= upBB2 + factor_cercania * boundingBox_2.h )
        colision = false;
    if( upBB1 + factor_cercania * boundingBox_2.h >= downBB2 )
        colision = false;
    if( rgtBB1 <= izqBB2 + factor_cercania * boundingBox_2.w )
        colision = false;
    if( izqBB1 + factor_cercania * boundingBox_2.w >= rgtBB2 )
        colision = false;

	//Si no se da ninguna condicion de no-colision, entonces hay colision
    return colision;
}

void Personaje::lanzarArma()
{
	//TODO: Chequear lanzamiento en salto y agachado
	// TODO: Agregar otras acciones.
	if ( !this->_isBlocking && !this->_isHiKicking && !this->_isLoKicking && !this->_isHiPunching && !this->_isLoPunching) 
	{
		if (!this->_weaponInAir)   //( !this->isFalling() && !this->isJumping() && !this->_isThrowing )
		{
			this->_isThrowing = true;
			this->_weaponInAir = true;
			this->resetearArma();
		}
	}
}

void Personaje::resetearArma()
{
	//Seteo la posción inicial del arma y la orientación
	if(!this->_orientacion)
		arma->_pos_x = this->_pos_x + this->_ancho_log/2;
	else
		arma->_pos_x = this->_pos_x;
	if (this->_isDucking)
		arma->_pos_y = this->_pos_y + this->_alto_log * 0.25;
	else
		arma->_pos_y = this->_pos_y + this->_alto_log * 0.5;
	arma->_orientacion = this->_orientacion;
	arma->setBoundingBox();
}

void Personaje::setBoundingBox()
{
	float factor_centrado = 50;
	if(!this->_orientacion)
		factor_centrado = -factor_centrado;

	boundingBox.x = this->get_x_px() + 50;
	boundingBox.y = this->get_y_px() * 1.2;
	boundingBox.w = this->_ancho_px / 1.8;
	boundingBox.h = this->_alto_px / 1.2;

	if((this->_isDucking) || (this->_isJumpingLeft) || (this->_isJumpingRight))
	{
		boundingBox.y = boundingBox.y * 1.35;
		boundingBox.h = boundingBox.h / 2;
	}

	if(this->_isJumping)
	{
		boundingBox.y = boundingBox.y * 1.35;
		boundingBox.h = boundingBox.h / 2;
	}

	if( ( this->_isHiKicking || this->_isHiPunching ) && (!this->_isDucking))
	{
		// TODO: Alargar bounding box, el pie queda afuera
		boundingBox.w = this->_ancho_px / 1.4;
		boundingBox.h = boundingBox.h / 3;
	}

	if( ( this->_isLoKicking || this->_isLoPunching ) && (!this->_isDucking))
	{
		// TODO: Alargar bounding box, el pie queda afuera
		boundingBox.w = this->_ancho_px / 1.4;
		boundingBox.h = boundingBox.h / 3;
	}

	//Renderiza el boundingbox - solo para pruebas
	SDL_RenderDrawRect( this->_ventana->_gRenderer, &boundingBox );
}

vector<SDL_Rect*> Personaje::loadVectorMedia(PersonajeData data)
{
	vector<SDL_Rect*> media;
	//const char* acciones[] = { "WALK","IDLE","JUMPUP","JUMPFWD","JUMPBWD", "DUCK", "BLOCK", "BLOCKDUCK", "DIZZY" };
	for (unsigned int i=0; i < data.cantSprites.size(); i++)
	{
		//cout << acciones[i] << endl;
		//cout << "	Fila: " << i << " - Cantidada de sprites: " << data.cantSprites[i] << endl;
		SDL_Rect* rectMedia = new SDL_Rect[data.cantSprites[i]];
		for (int j=0; j < data.cantSprites[i]; j++)
		{
			rectMedia[j].x = j * data.anchoSprites[i];
			rectMedia[j].y = i * data.height;
			rectMedia[j].w = data.anchoSprites[i];
			rectMedia[j].h = data.height;
		}
		media.push_back(rectMedia);
	}
	return media;
}

Personaje::~Personaje()
{
	//delete this;
	//cout << "destruyo capa" << endl;
	delete _handler;
	arma->~Arma();
	Logger::Instance()->log(DEBUG,"Destruyo personaje");
}

void Personaje::view(Personaje* otherPlayer)
{
	this->_orientacion = (this->_pos_x > otherPlayer->_pos_x );
	this->setBoundingBox();
	//printf("El personaje esta en %0.2f\n",_pos_x);
	//cout << this->_pos_y << endl;
	if ( this->_weaponInAir ) this->arma->viewLanzar();
	if ( this->_isThrowing )
	{
		if ( this->_isDucking )
		{
			// TODO: Mostrar disparo agachado
			this->viewShotWeapon(0);
			this->_timesThrow ++;
			if ( this->_timesThrow >= 10)
			{
				this->_isThrowing = false;
				this->_timesThrow = 0;
			}
		}
		else if ( this->_isJumping || this->_isFalling)
		{
			// TODO: Mostrar disparo en el aire
			this->viewShotWeapon(2);
			this->_timesThrow ++;
			if ( this->_timesThrow >= 10)
			{
				this->_isThrowing = false;
				this->_timesThrow = 0;
			}
		}
		else
		{
			// TODO: Mostrar disparo parado
			this->viewShotWeapon(1);
			this->_timesThrow ++;
			if ( this->_timesThrow >= 10)
			{
				this->_isThrowing = false;
				this->_timesThrow = 0;
			}
		}
	}
	else
	{
		if ( this->isJumping() || this->isFalling() )
		{
			if ( this->isHitting() )
			{
				if ( this->_isHiKicking || this->_isLoKicking )
				{
					this->viewKickAir();
				}
				else
				{
					this->viewPunchAir();
				}
			}
			else
			{
				if ( this->_isJumpingRight || this->_isFallingRight)
				{
					//cout << "SALTO DERECHA" << endl;
					this->viewJumpRight();
				}
				else if ( this->_isJumpingLeft || this->_isFallingLeft)
				{
					//cout << "SALTO IZQUIERDA" << endl;
					this->viewJumpLeft();
				}
				else
				{
					this->viewJump();
				}
			}
		}
		else if (this->_isDucking && this->_isBlocking)
		{
			this->viewBlockDuck();
		}
		else if (this->_isDucking && this->_isHiKicking)
		{
			this->viewHiKick();
		}
		else if (this->_isDucking && this->_isHiPunching)
		{
			this->viewHiPunch();
		}
		else if (this->_isDucking && this->_isLoPunching)
		{
			this->viewLoPunch();
		}
		else if (this->_isDucking && this->_isLoKicking)
		{
			this->viewLoKick();
		}
		else if (this->_isDucking && this->_beingHit)
		{
			this->viewHit();
		}
		else if (this->_isDucking)
		{
			this->viewDuck();
		}
		else if (this->_isDizzy)
		{
			this->viewDizzy();
		}
		else if (this->_isBlocking)
		{
			this->viewBlock();
		}
		else if (this->_isWalking)
		{
			this->viewWalking();
		}
		else if (this->_isHiKicking)
		{
			this->viewHiKick();
		}
		else if (this->_isHiPunching)
		{
			this->viewHiPunch();
		}
		else if (this->_isLoKicking)
		{
			this->viewLoKick();
		}
		else if (this->_isLoPunching)
		{
			this->viewLoPunch();
		}
		else if (this->_beingHit)
		{
				this->viewHit();
		}
		else
		{
			this->showIdle();
		}
	}
	//int x = get_x_px();
	//int y = get_y_px();
		//cout << "La posicion logica en x es " << _pos_x << " y en px es " << x << endl;
		//cout << "La posicion logica en y es " << _pos_y << " y en px es " << y << endl;
		//_handler->renderCut(x,y,_ancho_px * _factor_escala,_alto_px);
	//_handler->renderCut(x,y,_ancho_px,_alto_px);
		//_handler->render(0,0);
		//cout << "Se intenta dibujar capa " << endl;
}

bool Personaje::canMove() {
	return this->_canMove;
}

void Personaje::freeze() {
	this->_canMove = false;
}

void Personaje::unFreeze() {
	this->_canMove = true;
}

bool Personaje::isJumping()
{
	return ( this->_isJumping || this->_isJumpingRight || this->_isJumpingLeft );
}

bool Personaje::isBlocking()
{
	return (this->_isBlocking);
}

bool Personaje::isFalling()
{
	return ( this->_isFalling || this->_isFallingRight || this->_isFallingLeft );
}

void Personaje::showIdle()
{
	int delay = _data.velSprites[POS_FILA_IDLE];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[POS_FILA_IDLE] || pos_last_action != POS_FILA_IDLE)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_IDLE][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	pos_last_action = POS_FILA_IDLE;
}

void Personaje::viewWalking()
{
	int delay = _data.velSprites[POS_FILA_WALK];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[POS_FILA_WALK] || pos_last_action != POS_FILA_WALK)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_WALK][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	pos_last_action = POS_FILA_WALK;
}

void Personaje::viewDuck()
{
	int delay = _data.velSprites[POS_FILA_DUCK];
	++_lastFrame;
	int aux = _lastFrame / delay;
	int frame = aux;
	if ( aux < 0 || pos_last_action != POS_FILA_DUCK)
	{
		_lastFrame = 0;
	}else if(aux >= _data.cantSprites[POS_FILA_DUCK])
	{
		frame = _data.cantSprites[POS_FILA_DUCK]-1;
	}
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_DUCK][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	pos_last_action = POS_FILA_DUCK;
}

void Personaje::viewDizzy()
{
	int delay = _data.velSprites[POS_FILA_DIZZY];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[POS_FILA_DIZZY] || pos_last_action != POS_FILA_DIZZY )
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_DIZZY][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	pos_last_action = POS_FILA_DIZZY;
}

void Personaje::viewHiKick()
{
	int accion = 0;
	if (this->_isDucking){
		accion = POS_FILA_HIKICK_DUCK;
	}else{
		accion = POS_FILA_HIKICK;
	}
	int delay = _data.velSprites[accion];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[accion] || pos_last_action != accion)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[accion][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	if (aux == this->_personajeData.cantSprites[accion]){
		this->_isHiKicking = false;
	}
	pos_last_action = accion;
}

void Personaje::viewHiPunch()
{
	int accion = 0;
	if (this->_isDucking){
		accion = POS_FILA_HIPUNCH_DUCK;
	}else{
		accion = POS_FILA_HIPUNCH;
	}
	int delay = _data.velSprites[accion];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[accion] || pos_last_action != accion)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[accion][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	if (aux == this->_personajeData.cantSprites[accion]){
		this->_isHiPunching = false;
	}
	pos_last_action = accion;
}

void Personaje::viewLoKick()
{
	int accion = 0;
	if (this->_isDucking){
		accion = POS_FILA_LOKICK_DUCK;
	}else{
		accion = POS_FILA_LOKICK;
	}
	int delay = _data.velSprites[accion];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[accion] || pos_last_action != accion)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[accion][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	if (aux == this->_personajeData.cantSprites[accion]){
		this->_isLoKicking = false;
	}
	pos_last_action = accion;
}

void Personaje::viewLoPunch()
{
	int accion = 0;
	if (this->_isDucking){
		accion = POS_FILA_LOPUNCH_DUCK;
	}else{
		accion = POS_FILA_LOPUNCH;
	}
	int delay = _data.velSprites[accion];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[accion] || pos_last_action != accion)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[accion][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	if (aux == this->_personajeData.cantSprites[accion]){
		this->_isLoPunching = false;
	}
	pos_last_action = accion;
}

void Personaje::viewPunchAir()
{
	int accion = POS_FILA_AIRPUNCH;
	int delay = _data.velSprites[accion];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[accion] || pos_last_action != accion)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[accion][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	if (aux == this->_personajeData.cantSprites[accion]){
		this->_isLoPunching = false;
		this->_isHiPunching = false;
	}
	pos_last_action = accion;
}

void Personaje::viewKickAir(){
	int accion = POS_FILA_AIRKICK;
	int delay = _data.velSprites[accion];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[accion] || pos_last_action != accion)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[accion][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	if (aux == this->_personajeData.cantSprites[accion]){
		this->_isLoKicking = false;
		this->_isHiKicking = false;
	}
	pos_last_action = accion;
}

void Personaje::viewHit()
{
	int accion = 0;
	if (this->_isDucking){
		accion = POS_FILA_HITTED_DUCK;
	}else{
		accion = POS_FILA_HITTED;
	}
	int delay = _data.velSprites[accion];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[accion] || pos_last_action != accion)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[accion][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	if (aux == this->_personajeData.cantSprites[accion]){
		this->_beingHit = false;
	}
	pos_last_action = accion;
}

void Personaje::viewBlock()
{
	int delay = _data.velSprites[POS_FILA_BLOCK];
	++_lastFrame;
	int aux = _lastFrame / delay;
	int frame = aux;
	if ( aux < 0 || pos_last_action != POS_FILA_BLOCK)
	{
		_lastFrame = 0;
	}
	else if(aux >= _data.cantSprites[POS_FILA_BLOCK] - 1)
	{
		frame = _data.cantSprites[POS_FILA_BLOCK]-2;
	}
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_BLOCK][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	pos_last_action = POS_FILA_BLOCK ;
}

void Personaje::viewBlockDuck()
{
	int delay = _data.velSprites[POS_FILA_BLOCKDUCK];
	++_lastFrame;
	int aux = _lastFrame / delay;
	int frame = aux;
	if ( aux < 0 || pos_last_action != POS_FILA_BLOCKDUCK)
	{
		_lastFrame = 0;
	}
	else if(aux >= _data.cantSprites[POS_FILA_BLOCKDUCK] - 1)
	{
		frame = _data.cantSprites[POS_FILA_BLOCKDUCK]-2;
	}
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_BLOCKDUCK][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	pos_last_action = POS_FILA_BLOCKDUCK;
}

void Personaje::viewJump()
{
	int delay = _data.velSprites[POS_FILA_JUMP];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[POS_FILA_JUMP] || pos_last_action != POS_FILA_JUMP)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_JUMP][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	pos_last_action = POS_FILA_JUMP;
}

void Personaje::viewJumpRight()
{
	int pos_fil = 0;
	if (this->_orientacion){
		pos_fil = POS_FILA_JMPB;
	}else{
		pos_fil = POS_FILA_JMPF;
	}
	int delay = _data.velSprites[pos_fil];
	++_lastFrame;
	int aux = _lastFrame / delay;

	if ( aux < 0 || aux >= this->_personajeData.cantSprites[pos_fil] || pos_last_action != pos_fil)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[pos_fil][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	pos_last_action = pos_fil;
}

void Personaje::viewJumpLeft()
{
	int pos_fil = 0;
	if (this->_orientacion){
		pos_fil = POS_FILA_JMPF;
	}else{
		pos_fil = POS_FILA_JMPB;
	}
	int delay = _data.velSprites[pos_fil];
	++_lastFrame;
	int aux = _lastFrame / delay;
	if ( aux < 0 || aux >= this->_personajeData.cantSprites[pos_fil] || pos_last_action != pos_fil)
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/delay;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[pos_fil][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
	pos_last_action = pos_fil;
}

void Personaje::viewShotWeapon(size_t posicion)
{
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_ARMA][posicion]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
}

int Personaje::get_x_px()
{
	float pos_r = 0; //La posicion en pixeles en reales
	int pos_e = 0; //La posicion en pixeles verdadera (entero)
	float pos_rel = _pos_x - _ventana->_pos_log_x; //Posicion logica relativa a la ventana
	pos_r = ( pos_rel * _ancho_px) / _ancho_log;
	//pos_r = ( _pos_x * _ancho_px) / _ancho_log;
	pos_e = (int) pos_r;
	return pos_e;
}

int Personaje::get_y_px()
{
	float pos_r = 0; //La posicion en pixeles en reales
	int pos_e = 0; //La posicion en pixeles verdadera (entero)
	pos_r = ( (_ventana->_alto_log - _pos_y) * _ventana->_alto_px) / _ventana->_alto_log;
	pos_e = (int) pos_r;
	//cout << pos_r << endl;
	pos_e =  pos_e -_alto_px;
	return pos_e;
}

int Personaje::getWidth(Ventana* ventana, float ancho_log_capa)
{
	int ancho = 0;
	ancho = (float)(ancho_log_capa * ventana->_ancho_px) / ventana->_ancho_log ;
	return ancho;
}

int Personaje::getHeight(Ventana* ventana, float alto_log_capa)
{
	int alto = 0;
	alto = (float)(alto_log_capa * ventana->_alto_px) / ventana->_alto_log ;
	//cout << alto << endl;
	return alto;
}

void Personaje::moveLeft(float factor)
{
	this->_isDucking = false;
	this->_isBlocking = false;
	this->_isDizzy = false;
	if( ( !this->_isJumping && !this->_isFalling  && !this->isHitting() ) || ( this->_isJumpingLeft ))
	{
		this->_isWalking = true;
		Logger::Instance()->log(DEBUG,"Camino a la Izquierda");
		//cout << "Me intento mover " << factor << "y beta vale "<< getBeta(factor) << endl;
		//cout << "La pos actual es " << _pos_x << " el ancho logico es " << _ancho_log;
		//float new_x = _pos_x - ((_ancho_log / factor) + getAlpha(factor)) ; //viejo
		//cout << getBeta(factor) << endl;
		float new_x = _pos_x - ((factor) + getBeta(factor));
		//cout << " me intento mover a " << new_x << endl;
		if (new_x >= 0)
		{
			_pos_x = new_x;
		}else{
			_pos_x = 0;
		}
	}
}

bool Personaje::isLeftMargin()
{
	return (_pos_x - _ventana->_pos_log_x <= WINDOW_MARGIN_TOLERANCE);
}

//Se llama una vez que la vida de uno de los dos personajes llega a cero
void Personaje::winingPosition() {

}

void Personaje::golpeBajo() {

	if ( !this->_isHiPunching && !this->_isLoKicking && !this->_isHiKicking && !this->_isBlocking ) 
	{
		this->_isLoPunching = true;
		this->_isWalking = false;
	}
}

void Personaje::golpeAlto() {
	if ( !this->_isLoPunching && !this->_isLoKicking && !this->_isHiKicking && !this->_isBlocking )
	{ 
		this->_isHiPunching = true;
		this->_isWalking = false;
	}
}

void Personaje::patadaBaja() {
	if ( !this->_isLoPunching && !this->_isHiPunching && !this->_isHiKicking && !this->_isBlocking )
	{
		this->_isLoKicking = true;
		this->_isWalking = false;
	}
}

void Personaje::patadaAlta() {
	if ( !this->_isLoPunching && !this->_isHiPunching && !this->_isLoKicking && !this->_isBlocking )
	{
		this->_isHiKicking = true;
		this->_isWalking = false;
	}
}

void Personaje::evaluarAccion(int accion) {
	if (accion == this->getData()->getAR()) {
		this->arrojarArma();
	}
	else if (accion == this->getData()->getGA()) {
		this->golpeAlto();
	}
	else if (accion == this->getData()->getGB()) {
		this->golpeBajo();
	}
	else if (accion == this->getData()->getPA()) {
		this->patadaAlta();
	}
	else if (accion == this->getData()->getPB()) {
		this->patadaBaja();
	}
}

void Personaje::arrojarArma() {

}

void Personaje::duck()
{
	if (  !this->isFalling() && !this->isJumping() && !this->isHitting() )
	{
		if (!this->_isDucking) this->_lastFrame = 0;
		this->_isDucking = true;
		this->_isBlocking = false;
		this->_isWalking = false;
		this->_isDizzy = false;
	}
}

void Personaje::dizzy()
{
	if (  !this->isFalling() && !this->isJumping()  )
	{
		if (!this->_isDizzy) this->_lastFrame = 0;
		this->_isDizzy = true;
		this->_isDucking = false;
		this->_isWalking = false;
		this->_isBlocking = false;
	}
}

void Personaje::block()
{
	if (  !this->isFalling() && !this->isJumping() && !this->isHitting() )
	{
		if (!this->_isBlocking) this->_lastFrame = 0;
		this->_isBlocking = true;
		this->_isDucking = false;
		this->_isWalking = false;
		this->_isDizzy = false;
	}
}

void Personaje::blockDuck()
{
	if (  !this->isFalling() && !this->isJumping() && !this->isHitting() )
	{
		if (!this->_isBlocking) this->_lastFrame = 0;
		this->_isDucking = true;
		this->_isBlocking = true;
		this->_isWalking = false;
		this->_isDizzy = false;
	}
}

void Personaje::jump(float factor)
{
	//cout << "salto" << endl;
	if (  !this->isFalling() && !this->isJumping() && !this->isHitting() )
	{
		if (!this->_isJumping) this->_lastFrame = 0;
		this->_isJumping = true;
		this->_isWalking = false;
		this->_isDucking = false;
		this->_isBlocking = false;
		this->_isDizzy = false;
	}
}

void Personaje::jumpRight(float factor){
	if ( !this->isFalling() && !this->isJumping() && !this->isHitting() )
	{
		if (!this->_isJumpingRight) this->_lastFrame = 0;
		this->_isJumpingRight = true;
		this->_isWalking = false;
		this->_isDucking = false;
		this->_isBlocking = false;
		this->_isDizzy = false;
	}
}

void Personaje::jumpLeft(float factor){
	if ( !this->isFalling() && !this->isJumping() && !this->isHitting() )
	{
		if (!this->_isJumpingLeft) this->_lastFrame = 0;
		this->_isJumpingLeft = true;
		this->_isWalking = false;
		this->_isDucking = false;
		this->_isBlocking = false;
		this->_isDizzy = false;
	}
}

void Personaje::downLife(int cantidad)
{
	this->healthPoints -= cantidad;
}

void Personaje::continueAction(float factor_x, float factor_y, Personaje* otherPers)
{
	float new_x;
	float new_x_arma = arma->_pos_x;
	SDL_Rect between_frames;
	if ( this->isFalling() )
	{
		Logger::Instance()->log(DEBUG,"El personaje esta cayendo");
		float new_y = _pos_y - factor_y;
		if (new_y <= this->_escenario.y_piso)
		{
			// Lleg� al piso.
			_pos_y = _escenario.y_piso;
			this->_isFalling = false;
			this->_isFallingRight = false;
			this->_isFallingLeft = false;
		}
		else
		{
			_pos_y = new_y;
		}

		if ( this->_isFallingRight && !(otherPers->isLeftMargin() && this->isRightMargin()))
		{
			new_x = _pos_x + (factor_x + getBeta(factor_x));
			//cout << new_x - _pos_x << endl;
			if (new_x + this->_ancho_log <= _escenario.ancho)
				_pos_x = new_x;
			else
				_pos_x = _escenario.ancho - this->_ancho_log;
		}
		else if ( this->_isFallingLeft && !(otherPers->isRightMargin() && this->isLeftMargin()))
		{
			new_x = _pos_x - ((factor_x) + getBeta(factor_x));
			if (new_x >= 0)
				_pos_x = new_x;
			else
				_pos_x = 0;
		}
	}
	else if ( this->isJumping() )
	{
		Logger::Instance()->log(DEBUG,"El personaje esta saltando");
		float new_y = _pos_y + factor_y; // DIVIDO EL FACTOR POR 2 PORQUE SINO
											 // LLEGA AL PISO ANTES DE MOSTRAR TODOS
											 // LOS SPRITES
		if (new_y >= _alto_log)
		{
			// Estoy en altura m�xima.
			if ( this->_isJumpingRight )
				this->_isFallingRight = true; 
			else if ( this->_isJumpingLeft ) 
				this->_isFallingLeft = true;  
			else 
				this->_isFalling = true;

			this->_isJumping = false;
			this->_isJumpingRight = false;
			this->_isJumpingLeft = false;
			_pos_y = _alto_log;
		}
		else
		{
			_pos_y = new_y;
		}

		if ( this->_isJumpingRight && !(otherPers->isLeftMargin() && this->isRightMargin()))
		{
			new_x = _pos_x + (factor_x + getBeta(factor_x));
			if (new_x + this->_ancho_log <= _escenario.ancho)
				_pos_x = new_x;
			else
				_pos_x = _escenario.ancho - this->_ancho_log;

		}
		else if ( this->_isJumpingLeft && !(otherPers->isRightMargin() && this->isLeftMargin()))
		{
			new_x = _pos_x - ((factor_x) + getBeta(factor_x));
			if (new_x >= 0)
				_pos_x = new_x;
			else
				_pos_x = 0;
		}
	}

	if(this->_weaponInAir)
	{
		if(!arma->_orientacion)
		{
			//TODO: Está en el #define la velocidad del arma -> hay que entrar por json
			//TODO: Hay que cambiar los límites del arma para que no haya error - _escenario.ancho y 0 no van
			if ((new_x_arma >= (this->_ventana->_pos_log_x + this->_ventana->_ancho_log) ) || (this->hayColision(otherPers->boundingBox, arma->boundingBox)))
			{
				if (this->hayColision(otherPers->boundingBox, arma->boundingBox))
				{
					// TODO: Quizas debería estar en otro lado, por ahora funciona
					this->downLife(10);
				}
				this->_weaponInAir = false;
				this->resetearArma();
			}
			else
			{
				new_x_arma += ARMA_SPEED;
				if((arma->_pos_x <= otherPers->_pos_x) && (new_x_arma >= otherPers->_pos_x))
				{
					between_frames.x = arma->_pos_x;
					between_frames.y = arma->_pos_y;
					between_frames.h = arma->_alto_px;
					between_frames.w = new_x_arma - arma->_pos_x;
					if(this->hayColision(otherPers->boundingBox, between_frames))
					{
						//Hay hit entre 2 frames
						this->_weaponInAir = false;
						this->resetearArma();
					}
					else
					{
						arma->_pos_x = new_x_arma;
					}
				}
				else
				{
					arma->_pos_x = new_x_arma;
				}
			}
		}
		else
		{
			if ((new_x_arma <= this->_ventana->_pos_log_x) || (this->hayColision(otherPers->boundingBox, arma->boundingBox)))
			{
				if (this->hayColision(otherPers->boundingBox, arma->boundingBox))
				{
					// TODO: Quizas debería estar en otro lado, por ahora funciona
					this->downLife(10);
				}
				this->_weaponInAir = false;
				this->resetearArma();
			}
			else
			{
				new_x_arma -= ARMA_SPEED;
				if((arma->_pos_x >= otherPers->_pos_x) && (new_x_arma <= otherPers->_pos_x))
				{
					between_frames.x = new_x_arma;
					between_frames.y = arma->_pos_y;
					between_frames.h = arma->_alto_px;
					between_frames.w = arma->_pos_x - new_x_arma;
					if(this->hayColision(otherPers->boundingBox, between_frames))
					{
						//Hay hit entre 2 frames
						this->_weaponInAir = false;
						this->resetearArma();
					}
					else
					{
						arma->_pos_x = new_x_arma;
					}
				}
				else
				{
					arma->_pos_x = new_x_arma;
				}
			}
		}
	}
}

bool Personaje::isMovingInJump()
{
	return ( this->_isJumpingLeft || this->_isJumpingRight 
			|| this->_isFallingRight || this->_isFallingLeft );
}

bool Personaje::isWalking()
{
	return this->_isWalking;
}

bool Personaje::isJumpingRight()
{
	return (_isJumpingRight || _isFallingRight);
}

bool Personaje::isJumpingLeft()
{
	return (_isJumpingLeft || _isFallingLeft);
}

bool Personaje::isDucking() {
	return this->_isDucking;
}

bool Personaje::isHitting()
{
	return (this->_isHiPunching || this->_isHiKicking || this->_isLoPunching || this->_isLoKicking);
}

void Personaje::moveRight(float factor)
{
	this->_isDucking = false;
	this->_isBlocking = false;
	this->_isDizzy = false;
	if(!( this->_isJumping ) && !( this->_isFalling ) && !this->isHitting() )
	{
		this->_isWalking = true;
		Logger::Instance()->log(DEBUG,"Camino a la derecha");
		//cout << "Me intento mover " << factor << "y beta vale "<< getBeta(factor) << endl;
		//float new_x = _pos_x + (_ancho_log / factor) + getAlpha(factor); //viejo
		float new_x = _pos_x + factor + getBeta(factor);
		if (new_x + this->_ancho_log <= _escenario.ancho)
		{
			_pos_x = new_x;
		}
		else
		{
			_pos_x = this->_escenario.ancho - this->_ancho_log;
		}
	}
}

bool Personaje::isRightMargin()
{
	return (_ventana->_pos_log_x + _ventana->_ancho_log - (_pos_x + this->_ancho_log) <= WINDOW_MARGIN_TOLERANCE);
}

float Personaje::getBeta(float factor)
{
	float ancho_escenario = this->_escenario.ancho;
	float ancho_capa = this->_ancho_log;
	float ancho_ventana = this->_ventana->_ancho_log;
	float beta = factor * (((ancho_capa - ancho_ventana)/(ancho_escenario - ancho_ventana) ) -1);
	beta = 0;
	return beta;
}

void Personaje::idle()
{
	this->_isBlocking = false;
	this->_isDucking = false;
	this->_isWalking = false;
	this->_isDizzy = false;
}

void Personaje::hit()
{
	this->_beingHit = true;
}
