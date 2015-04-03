// Texture wrapper class

//Using SDL, SDL_image, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class LTexture
{
	public:
		//Initializes variables
		LTexture();
		LTexture(SDL_Renderer* mRenderer);

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path, bool img_PNG = false );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y );

		//Renders texture at given point, cuted image
		void renderCut(int x, int y, int width, int height);

		//Renders texture at given point, scaled
		void renderScaled( int x, int y, int width, int height );


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
