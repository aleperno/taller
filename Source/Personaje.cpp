/*
 * Personaje.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: neo
 */
#include <Personaje.h>

Personaje::Personaje(Ventana* ventana, PersonajeData data, EscenarioData escenario)
{
	this->_ventana = ventana;
	this->_handler = new TextureHandler(ventana->_gRenderer);
	this->_alto_log = data.alto;
	this->_ancho_log = data.ancho;
	string path = data.imgPath;
	this->_handler->loadFromFile(path,true);
	this-> _escenario = escenario;
	this-> _factor_escala = escenario.ancho / this->_ancho_log;
	this->_pos_y = escenario.y_piso;
	this->_pos_x = ((- _ancho_log)+ventana->_ancho_log) /2;
	this->_ancho_px = Personaje::getWidth(ventana,_ancho_log);
	this->_alto_px = Personaje::getHeight(ventana,_alto_log);
	this->_zIndex = data.z_index;
	this->_personajeData = data;
	this->sprites = Personaje::loadMedia(data);
	this->_lastFrame = 0;
	this->_isWalking = false;
	this->_isJumping = false;
	this->_isJumpingRight = false;
	this->_isJumpingLeft = false;
	this->_isFalling = false;
	this->_isFallingRight = false;
	this->_isFallingLeft = false;
	//cout << _pos_x << endl;
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

Personaje::~Personaje()
{
	//delete this;
	cout << "destruyo capa" << endl;
	delete _handler;
}

void Personaje::view()
{
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
	if ( aux < this->_personajeData.idle[0] || aux > this->_personajeData.idle[1])
	{
		_lastFrame = this->_personajeData.idle[0] * SPEED;
	}
	int frame = _lastFrame/SPEED;
	//cout << frame << endl;
	SDL_Rect* currentClip = &(this->sprites[frame]);
	int x = get_x_px();
	int y = get_y_px();
	this->_handler->renderAnimation(false,x,y,_ancho_px,_alto_px,currentClip);
}

void Personaje::viewWalking()
{
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
	this->_handler->renderAnimation(false,x,y,_ancho_px,_alto_px,currentClip);
}

void Personaje::viewJump()
{
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
	this->_handler->renderAnimation(false,x,y,_ancho_px,_alto_px,currentClip);
}

void Personaje::viewJumpRight()
{
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
	this->_handler->renderAnimation(false,x,y,_ancho_px,_alto_px,currentClip);
}

void Personaje::viewJumpLeft()
{
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
	this->_handler->renderAnimation(false,x,y,_ancho_px,_alto_px,currentClip);
}

int Personaje::get_x_px()
{
	float pos_r = 0; //La posicion en pixeles en reales
	int pos_e = 0; //La posicion en pixeles verdadera (entero)
	pos_r = ( _pos_x * _ancho_px) / _ancho_log;
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
	if( ( !this->_isJumping && !this->_isFalling ) || ( this->_isJumpingLeft ))
	{
		this->_isWalking = true;
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
	return (_pos_x == 0);
}

void Personaje::jump(float factor)
{
	//cout << "salto" << endl;
	if (  !this->isFalling() && !this->isJumping()  )
	{
		this->_isJumping = true;
	}
}

void Personaje::jumpRight(float factor){
	if ( !this->isFalling() && !this->isJumping() )
	{
		this->_isJumpingRight = true;
	}
}

void Personaje::jumpLeft(float factor){
	if ( !this->isFalling() && !this->isJumping() )
	{
		this->_isJumpingLeft = true;
	}
}

void Personaje::continueAction(float factor_x, float factor_y)
{
	float new_x;
	if ( this->isFalling() )
	{
		float new_y = _pos_y - factor_y; // DIVIDO EL FACTOR POR 2 PORQUE SINO
											 // LLEGA AL PISO ANTES DE MOSTRAR TODOS
											 // LOS SPRITES
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

		if ( this->_isFallingRight )
		{
			new_x = _pos_x + (factor_x + getBeta(factor_x))*JMP_SPEED_X;
			if (new_x + this->_ancho_log <= this->_ventana->_ancho_log)
				_pos_x = new_x;
			else
				_pos_x = this->_ventana->_ancho_log - this->_ancho_log;
		}
		else if ( this->_isFallingLeft )
		{
			new_x = _pos_x - ((factor_x) + getBeta(factor_x))*JMP_SPEED_X;
			if (new_x >= 0)
				_pos_x = new_x;
			else
				_pos_x = 0;
		}
	}
	else if ( this->isJumping() )
	{
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

		if ( this->_isJumpingRight )
		{
			new_x = _pos_x + (factor_x + getBeta(factor_x))*JMP_SPEED_X;
			if (new_x + this->_ancho_log <= this->_ventana->_ancho_log)
				_pos_x = new_x;
			else
				_pos_x = this->_ventana->_ancho_log - this->_ancho_log;

		}
		else if ( this->_isJumpingLeft )
		{
			new_x = _pos_x - ((factor_x) + getBeta(factor_x))*JMP_SPEED_X;
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
	if(!( this->_isJumping ) && !( this->_isFalling ))
	{
		this->_isWalking = true;
		//cout << "Me intento mover " << factor << "y beta vale "<< getBeta(factor) << endl;
		//float new_x = _pos_x + (_ancho_log / factor) + getAlpha(factor); //viejo
		float new_x = _pos_x + factor + getBeta(factor);
		if (new_x + this->_ancho_log <= this->_ventana->_ancho_log)
		{
			_pos_x = new_x;
		}
		else
		{
			_pos_x = this->_ventana->_ancho_log - this->_ancho_log;
		}
	}
}

bool Personaje::isRightMargin()
{
	return (_pos_x + this->_ancho_log == this->_ventana->_ancho_log );
}

float Personaje::getBeta(float factor)
{
	float ancho_escenario = this->_escenario.ancho;
	float ancho_capa = this->_ancho_log;
	float ancho_ventana = this->_ventana->_ancho_log;
	float beta = factor * (((ancho_capa - ancho_ventana)/(ancho_escenario - ancho_ventana) ) -1);
	beta = 0;
	return 0;
}

void Personaje::idle()
{
	this->_isWalking = false;
}
