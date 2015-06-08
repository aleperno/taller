/*
 * Toasty.h
 *
 *  Created on: Jun 8, 2015
 *      Author: neo
 */

#ifndef TOASTY_H_
#define TOASTY_H_

#include <Parser.h>
#include <Ventana.h>
#include <SoundHandler.h>
#include <ToastyData.h>
#include <string>
#include <stdlib.h>

#define TOASTY_PROB 1

class Toasty
{
	public:
		Toasty(Ventana* ventana, ToastyData data);
		bool active;
		void setActive();
		bool view();

	private:
		int get_x_px();
		int get_y_px();
		int getWidth(Ventana* ventana, float ancho_log_capa);
		int getHeight(Ventana* ventana, float ancho_log_capa);
		TextureHandler* _handler;
		bool rotate;
		float _pos_x;
		float _pos_y;
		float _ancho_log;
		float _alto_log;
		int _ancho_px;
		int _alto_px;
		float rate;
		Ventana* _ventana;
};



#endif /* TOASTY_H_ */
