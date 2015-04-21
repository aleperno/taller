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

bool TextureHandler::loadFromFile( std::string path, bool img_PNG)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	//SDL_Surface* loadedSurface;
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
