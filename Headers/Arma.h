#include <SDL.h>
#include <SDL_image.h>
#include <Ventana.h>

class Arma
{
	private:
		TextureHandler* _handler;
		Ventana* _ventana;

		float _alto_log;
		float _ancho_log;
		int _zIndex;
		float _factor_escala;
		int getHeight(Ventana* ventana, float alto_log_capa);
		int getWidth(Ventana* ventana, float ancho_log_capa);
		int get_x_px();
		int get_y_px();

	public:
		float _alto_px;
		float _ancho_px;
		SDL_Rect boundingBox;
		void setBoundingBox();
		float _pos_x;
		float _pos_y;
		bool _orientacion;
		void viewLanzar();
		Arma(string path,float _alto_log, float _ancho_log, float _escala, Ventana* _ventana, int zIndex);
		~Arma();	
};