#include <SDL.h>
#include <SDL_image.h>
#include <Ventana.h>

class Arma
{
	private:
		TextureHandler* _handler;
		Ventana* _ventana;
		SDL_Rect boundingBox;
		float _pos_x;
		float _pos_y;
		float _alto_px;
		float _ancho_px;
		float _alto_log;
		float _ancho_log;
		int _zIndex;
		float _factor_escala;
		bool _orientacion;
		void setBoundingBox();
		int getHeight(Ventana* ventana, float alto_log_capa);
		int getWidth(Ventana* ventana, float ancho_log_capa);
		int get_x_px();
		int get_y_px();

	public:
		void viewLanzar();
		Arma(string path, bool _orientacion, float _alto_log, float _ancho_log, float _escala, Ventana* _ventana, int zIndex, float pos_y, float pos_incial_x);
		~Arma();	
};