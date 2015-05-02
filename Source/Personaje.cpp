/*
 * Personaje.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: neo
 */
#include <Personaje.h>

Personaje::Personaje(Ventana* ventana, PersonajeData data, EscenarioData escenario, bool pers_ppal)
{
	Logger::Instance()->log(DEBUG,"Se crea personaje");
	this->_ventana = ventana;
	this->_handler = new TextureHandler(ventana->_gRenderer);
	this->_alto_log = data.alto;
	this->_ancho_log = data.ancho;
	string path = data.imgPath;
	this->_handler->loadFromFile(path,!pers_ppal,data.h_inicial,data.h_final,data.desplazamiento,true);
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
	this->sprites = Personaje::loadMedia(data);
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

	this->_orientacion = _personajeData.orientacion;
	this->setBoundingBox();
	//cout << _pos_x << endl;
}

void Personaje::setBoundingBox()
{
	boundingBox.x = this->get_x_px();
	boundingBox.y = this->get_y_px();
	boundingBox.w = this->_ancho_px;
	boundingBox.h = this->_alto_px;
}

SDL_Rect* Personaje::loadMedia(PersonajeData data)
{
	SDL_Rect* media = new SDL_Rect[data.size];
	for (int i=0; i<data.size; i++)
	{
		media[i].x = i * data.width;
		media[i].y = 0;
		media[i].w = data.width;
		media[i].h = data.height;
	}
	return media;
}

vector<SDL_Rect*> Personaje::loadVectorMedia(PersonajeData data)
{
	vector<SDL_Rect*> media;
	unsigned int cantSprites = 6;
	//const char* acciones[] = { "WALK","IDLE","JUMPUP","JUMPFWD","JUMPBWD", "DUCK" };
	for (unsigned int i=0; i < cantSprites; i++)
	{
		//cout << acciones[i] << endl;
		//cout << "	Fila: " << i << " - Cantidada de sprites: " << data.cantSprites[i] << endl;
		SDL_Rect* rectMedia = new SDL_Rect[data.cantSprites[i] + 1];
		for (int j=0; j <= data.cantSprites[i]; j++)
		{
			rectMedia[j].x = j * data.width;
			rectMedia[j].y = i * data.height;
			rectMedia[j].w = data.width;
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
	Logger::Instance()->log(DEBUG,"Destruyo personaje");
}

void Personaje::view(Personaje* otherPlayer)
{
	this->_orientacion = (this->_pos_x > otherPlayer->_pos_x );
	this->setBoundingBox();
	//printf("El personaje esta en %0.2f\n",_pos_x);
	//cout << this->_pos_y << endl;
	if ( this->isJumping() || this->isFalling() )
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
	else if (this->_isDucking)
	{
		this->viewDuck();
	}
	else if (this->_isWalking)
	{
		this->viewWalking();
	}
	else
	{
		this->showIdle();
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

bool Personaje::isJumping()
{
	return ( this->_isJumping || this->_isJumpingRight || this->_isJumpingLeft );
}

bool Personaje::isFalling()
{
	return ( this->_isFalling || this->_isFallingRight || this->_isFallingLeft );
}

void Personaje::showIdle()
{
	++_lastFrame;
	int aux = _lastFrame / SPEED;
	if ( aux < 0 || aux > this->_personajeData.cantSprites[POS_FILA_IDLE] )
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/SPEED;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_IDLE][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);
/*
	++_lastFrame;
	int aux = _lastFrame / SPEED;
	if ( aux < this->_personajeData.idle[0] || aux > this->_personajeData.idle[1])
	{
		_lastFrame = this->_personajeData.idle[0] * SPEED;
	}
	int frame = _lastFrame/SPEED;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->sprites[frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_personajeData.orientacion,x,y,_ancho_px,_alto_px,currentClip);
*/
}

void Personaje::viewWalking()
{
	++_lastFrame;
	int aux = _lastFrame / SPEED;
	if ( aux < 0 || aux > this->_personajeData.cantSprites[POS_FILA_WALK] )
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/SPEED;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_WALK][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);

/*
	++_lastFrame;
	int aux = _lastFrame / SPEED;
	if ( aux < this->_personajeData.walk[0] || aux > this->_personajeData.walk[1])
	{
		_lastFrame = this->_personajeData.walk[0] * SPEED;
	}
	int frame = _lastFrame/SPEED;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->sprites[frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_personajeData.orientacion,x,y,_ancho_px,_alto_px,currentClip);
*/
}

void Personaje::viewDuck()
{
	int aux = _lastFrame / SPEED;
	if ( aux < this->_personajeData.cantSprites[POS_FILA_DUCK] )
	{
		++_lastFrame;
		aux = _lastFrame / SPEED;
		if ( aux < 0 || aux > this->_personajeData.cantSprites[POS_FILA_DUCK]+1)
		{
			_lastFrame = 0;
		}
	}
	else if ( aux > this->_personajeData.cantSprites[POS_FILA_DUCK] )
	{
		_lastFrame = this->_personajeData.cantSprites[POS_FILA_DUCK];
	}

	int frame = _lastFrame/SPEED;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_DUCK][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);

/*
	int aux = _lastFrame / SPEED;
	if ( aux < this->_personajeData.duck[1] )
	{
		++_lastFrame;
		aux = _lastFrame / SPEED;
		if ( aux < this->_personajeData.duck[0] || aux > this->_personajeData.duck[1])
		{
			_lastFrame = this->_personajeData.duck[0] * SPEED;
		}
	}
	else
	{
		_lastFrame = this->_personajeData.duck[1] * SPEED;
	}
	int frame = _lastFrame/SPEED;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->sprites[frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_personajeData.orientacion,x,y,_ancho_px,_alto_px,currentClip);

*/
}

void Personaje::viewJump()
{
	++_lastFrame;
	int aux = _lastFrame / JMP_SPEED;
	if ( aux < 0 || aux > this->_personajeData.cantSprites[POS_FILA_JUMP])
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/JMP_SPEED;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_JUMP][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);

	
/*
	//cout << "Muestro salto" << endl;
	++_lastFrame;
	int aux = _lastFrame / JMP_SPEED;
	if ( aux < this->_personajeData.jumpUp[0] || aux > this->_personajeData.jumpUp[1])
	{
		_lastFrame = this->_personajeData.jumpUp[0] * JMP_SPEED;
	}
	int frame = _lastFrame/JMP_SPEED;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->sprites[frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_personajeData.orientacion,x,y,_ancho_px,_alto_px,currentClip);
*/
}

void Personaje::viewJumpRight()
{
	++_lastFrame;
	int aux = _lastFrame / JMP_SPEED2;
	if ( aux < 0 || aux > this->_personajeData.cantSprites[POS_FILA_JMPF])
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/JMP_SPEED2;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_JMPF][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);

/*
	++_lastFrame;
	int aux = _lastFrame / JMP_SPEED2;
	if ( aux < this->_personajeData.jumpFwd[0] || aux > this->_personajeData.jumpFwd[1])
	{
		_lastFrame = this->_personajeData.jumpFwd[0] * JMP_SPEED2;
	}
	int frame = _lastFrame/JMP_SPEED2;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->sprites[frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_personajeData.orientacion,x,y,_ancho_px,_alto_px,currentClip);
*/
}

void Personaje::viewJumpLeft()
{
	++_lastFrame;
	int aux = _lastFrame / JMP_SPEED2;
	if ( aux < 0 || aux > this->_personajeData.cantSprites[POS_FILA_JMPB])
	{
		_lastFrame = 0;
	}
	int frame = _lastFrame/JMP_SPEED2;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->vectorSprites[POS_FILA_JMPB][frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_orientacion,x,y,_ancho_px,_alto_px,currentClip);

/*
	++_lastFrame;
	int aux = _lastFrame / JMP_SPEED2;
	if ( aux < this->_personajeData.jumpBwd[0] || aux > this->_personajeData.jumpBwd[1])
	{
		_lastFrame = this->_personajeData.jumpBwd[0] * JMP_SPEED2;
	}
	int frame = _lastFrame/JMP_SPEED2;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->sprites[frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(this->_personajeData.orientacion,x,y,_ancho_px,_alto_px,currentClip);
*/
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
	if( ( !this->_isJumping && !this->_isFalling ) || ( this->_isJumpingLeft ))
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

void Personaje::duck()
{
	if (  !this->isFalling() && !this->isJumping()  )
	{
		this->_isDucking = true;
		this->_isWalking = false;
	}
}

void Personaje::jump(float factor)
{
	//cout << "salto" << endl;
	if (  !this->isFalling() && !this->isJumping()  )
	{
		this->_isJumping = true;
		this->_isWalking = false;
	}
}

void Personaje::jumpRight(float factor){
	if ( !this->isFalling() && !this->isJumping() )
	{
		this->_isJumpingRight = true;
		this->_isWalking = false;
	}
}

void Personaje::jumpLeft(float factor){
	if ( !this->isFalling() && !this->isJumping() )
	{
		this->_isJumpingLeft = true;
		this->_isWalking = false;
	}
}

void Personaje::continueAction(float factor_x, float factor_y, Personaje* otherPers)
{
	float new_x;
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

void Personaje::moveRight(float factor)
{
	this->_isDucking = false;
	if(!( this->_isJumping ) && !( this->_isFalling ))
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
	this->_isDucking = false;
	this->_isWalking = false;
}
