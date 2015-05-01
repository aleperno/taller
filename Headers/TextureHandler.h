// Texture wrapper class

//Using SDL, SDL_image, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

class TextureHandler
{
	public:
		//Initializes variables
		TextureHandler();
		TextureHandler(SDL_Renderer* mRenderer);

		//Deallocates memory
		~TextureHandler();

		void setearHSVPorPixel(SDL_Surface* loadedSurface, vector<float>* hue, vector<float>* saturation, vector<float>* value);
		void setearNuevoRGB(SDL_Surface* loadedSurface, vector<float>* hue, vector<float>* saturation, vector<float>* value);
		void alterarHue(vector<float>* hue, float h_inicial, float h_final, float desplazamiento);
		void cambiarColor(SDL_Surface* loadedSurface, float h_inicial, float h_final, float desplazamiento);

		//Loads image at specified path
		bool loadFromFile( std::string path, bool cambiarColor, float h_inicial, float h_final, float desplazamiento, bool img_PNG = false );

		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y );

		//Renders texture at given point, cuted image
		void renderCut(int x, int y, int width, int height);

		//Renders texture at given point, scaled
		void renderScaled( int x, int y, int width, int height );

		//Renders texture at given point, cuted image
		void renderAnimation(bool flip, int x, int y, int width, int height, SDL_Rect* clip);

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		
		//The actual window
		SDL_Renderer* mRenderer;
		
		//Image dimensions
		int mWidth;
		int mHeight;
};

