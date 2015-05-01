#include "TextureHandler.h"
const float ESCALA = 2;

TextureHandler::TextureHandler()
{
	//Initialize
	mRenderer = NULL;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

TextureHandler::TextureHandler(SDL_Renderer* mRenderer)
{
	//Initialize
	mTexture = NULL;
	this->mRenderer = mRenderer;
	mWidth = 0;
	mHeight = 0;
}

TextureHandler::~TextureHandler()
{
	//Deallocate
	free();
}

void TextureHandler::setearNuevoRGB(SDL_Surface* loadedSurface, vector<float>* hue, vector<float>* saturation, vector<float>* value)
{
	uint32_t* pixelsDeSurface = ( uint32_t * )loadedSurface->pixels;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;

	for (int i=0; i<hue->size(); i++) {
		
		float Hi = floor(hue->at(i) / 60);
		float Vmin = (100 - saturation->at(i)) * (value->at(i)) / 100;
		float num = (value->at(i) - Vmin) * fmod(hue->at(i),60) / 60;
		float Vinc = Vmin + num;
		float Vdec = value->at(i) - num;

		float red;
		float green;
		float blue;

		switch ((int)Hi) {
		case 0:	red = value->at(i);
				green = Vinc;
				blue = Vmin;
				break;
		case 1:	red = Vdec;
				green = value->at(i);
				blue = Vmin;
				break;
		case 2:	red = Vmin;
				green = value->at(i);
				blue = Vinc;
				break;
		case 3:	red = Vmin;
				green = Vdec;
				blue = value->at(i);
				break;
		case 4: red = Vinc;
				green = Vmin;
				blue = value->at(i);
				break;
		case 5: red = value->at(i);
				green = Vmin;
				blue = Vdec;
				break;
		}

		SDL_GetRGBA(pixelsDeSurface[i], loadedSurface->format, &r, &g, &b, &a);
		r = (Uint8) floor(red * 255 / 100 + 0.5);
		g = (Uint8) floor(green * 255 / 100 + 0.5);
		b = (Uint8) floor(blue * 255 / 100 + 0.5);

		pixelsDeSurface[i] = SDL_MapRGBA(loadedSurface->format, r, g, b, a);
	}
}

void TextureHandler::setearHSVPorPixel(SDL_Surface* loadedSurface, vector<float>* hue, vector<float>* saturation, vector<float>* value)
{
	uint32_t* pixelsDeSurface = ( uint32_t * )loadedSurface->pixels;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;

	for (int i=0; i<hue->size(); i++) {
		SDL_GetRGBA(pixelsDeSurface[i], loadedSurface->format, &r, &g, &b, &a);
		float normR = ((float)r)/255;
		float normG = ((float)g)/255;
		float normB = ((float)b)/255;

		float rgb_max = std::max(normR, std::max(normG, normB));
		float rgb_min = std::min(normR, std::min(normG, normB));
		float delta = rgb_max - rgb_min;

		value->at(i) = rgb_max*100;

		if (rgb_max == 0)	saturation->at(i) = 0;
		else	saturation->at(i) = delta*100 / rgb_max;

		float hueLocal;
		if (rgb_min == rgb_max)		hueLocal = 0;
		else	if (rgb_max == normG)	hueLocal = 60 * (normB-normR) / delta + 120;
		else	if (rgb_max == normB)	hueLocal = 60 * (normR-normG) / delta + 240;
		else	if (normG < normB)	hueLocal = 60 * (normG-normB) / delta + 360; //rgb_max=normR
		else	hueLocal = 60 * (normG-normB) / delta;
		hue->at(i) = hueLocal;
	}
}

void TextureHandler::alterarHue(vector<float>* hue, float h_inicial, float h_final, float desplazamiento) {
	if (h_inicial < h_final) {
		for (int i=0; i<hue->size(); i++) {
			if ((h_inicial <= hue->at(i)) && (h_final >= hue->at(i))) {
				hue->at(i) += desplazamiento;
				while (!(hue->at(i) < 360))		{ hue->at(i) -= 360; }
				while (hue->at(i) < 0)	{ hue->at(i) += 360; }
			}
		}
	}

	if (h_inicial > h_final) {
		for (int i=0; i<hue->size(); i++) {
			if ((h_inicial <= hue->at(i)) || (h_final >= hue->at(i))) {
				hue->at(i) += desplazamiento;
				while (!(hue->at(i) < 360))		{ hue->at(i) -= 360; }
				while (hue->at(i) < 0)	{ hue->at(i) += 360; }
			}
		}
	}
}

void TextureHandler::cambiarColor(SDL_Surface* loadedSurface, float h_inicial, float h_final, float desplazamiento)
{
	vector<float> hue((loadedSurface->w)*(loadedSurface->h));
	vector<float> saturation((loadedSurface->w)*(loadedSurface->h));
	vector<float> value((loadedSurface->w)*(loadedSurface->h));

	setearHSVPorPixel(loadedSurface, &hue, &saturation, &value);

	alterarHue(&hue, h_inicial, h_final, desplazamiento);

	setearNuevoRGB(loadedSurface, &hue, &saturation, &value);
}

bool TextureHandler::loadFromFile( std::string path, bool cambiarColor, float h_inicial, float h_final, float desplazamiento, bool img_PNG)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface;
	if(img_PNG){ 
		loadedSurface = IMG_Load( path.c_str() );
		if( loadedSurface == NULL ){
			printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
		}
	}else{ 
		loadedSurface = SDL_LoadBMP( path.c_str() );
		if( loadedSurface == NULL ){
			printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
	}

	if( loadedSurface != NULL )
	{
		//Color key image
		//SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		if (cambiarColor) {
			this->cambiarColor(loadedSurface,h_inicial,h_final,desplazamiento);
		}

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( this->mRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool TextureHandler::loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( this->mRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
}

void TextureHandler::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void TextureHandler::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void TextureHandler::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void TextureHandler::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void TextureHandler::render( int x, int y)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy( this->mRenderer, mTexture, NULL, &renderQuad );
}

void TextureHandler::renderCut(int x, int y, int width, int height)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopy( this->mRenderer, mTexture, NULL, &renderQuad );
}

void TextureHandler::renderScaled( int x, int y, int width, int height )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopy( this->mRenderer, mTexture, NULL, &renderQuad );
}

void TextureHandler::renderAnimation(bool flip, int x, int y, int width, int height, SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		//renderQuad.w = clip->w ;
		//renderQuad.h = clip->h ;
	}

	//Render to screen
	//SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
	if(flip)
		SDL_RenderCopyEx( this->mRenderer, mTexture, clip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL );
	else
		SDL_RenderCopyEx( this->mRenderer, mTexture, clip, &renderQuad, 0, NULL, SDL_FLIP_NONE );
}

int TextureHandler::getWidth()
{
	return mWidth;
}

int TextureHandler::getHeight()
{
	return mHeight;
}
