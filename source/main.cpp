/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL OpenGL, standard IO, and, strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include "temporizador.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

//Input handler
void handleKeys( unsigned char key, int x, int y );

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderQuad = true;

// Image to render
GLuint TextureID = 0;
SDL_Surface *surface = nullptr;

SDL_Surface *cargarSuperficie( const char *archivo );

int absEnteros( int n );
int potenciaDos( int n );

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 && !( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL )
			{
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				surface = cargarSuperficie( "Stonehenge.png" );
				if( surface == nullptr ){
					success = false;
				}
				else{
					glGenTextures( 1, &TextureID );
					glBindTexture(GL_TEXTURE_2D, TextureID);
					int mode = GL_RGBA;
					
					glTexImage2D( GL_TEXTURE_2D, 0, mode, surface -> w, surface -> h, 0, mode, GL_UNSIGNED_BYTE, surface -> pixels );
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
				
				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				{
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}
				
				//Initialize OpenGL
				if( !initGL() )
				{
					printf( "Unable to initialize OpenGL!\n" );
					success = false;
				}
			}
		}
	}

	return success;
}

bool initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}

	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}
	
	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	
	//Enable texturing
    glEnable( GL_TEXTURE_2D );
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		success = false;
	}
	
	return success;
}

void handleKeys( unsigned char key, int x, int y )
{
	//Toggle quad
	if( key == 'q' )
	{
		gRenderQuad = !gRenderQuad;
	}
}

void update()
{
	//No per frame update needed
}

void render()
{
	//Clear color buffer
	glClear( GL_COLOR_BUFFER_BIT );
	glBindTexture( GL_TEXTURE_2D, TextureID );
	
	//Render quad
	if( gRenderQuad )
	{
		glBegin( GL_QUADS );
			glTexCoord2f( 0, 0 ); glVertex2f( -1.f, -1.f );
			glTexCoord2f( 1, 0 ); glVertex2f( 1.f, -1.f );
			glTexCoord2f( 1, 1 ); glVertex2f( 1.f, 1.f );
			glTexCoord2f( 0, 1 ); glVertex2f( -1.f, 1.f );
		glEnd();
	}
}

void close()
{
	//Destroy window	
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;
		
		Temporizador tmp;
		tmp.iniciar();
		int fps = 0;

		//While application is running
		while( !quit )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
			}

			//Render quad
			render();
			
			//Update screen
			SDL_GL_SwapWindow( gWindow );
			fps++;
			if( tmp.obtenerTicks() > 1000 ){
				printf( "FPSs: %d\n", fps );
				tmp.reiniciar();
				fps = 0;
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

SDL_Surface *cargarSuperficie( const char *archivo )
{
	// Carga la textura
	SDL_Surface *superficie = IMG_Load("Stonehenge.png");
	if( superficie == NULL ){
		return nullptr;
	}
	
	// Obtiene las nuevas dimensiones de la superficie
	int anchura = potenciaDos( superficie -> w );
	int altura = potenciaDos( superficie -> h );
					
	// Establece las máscaras de color
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		Uint32 rmask = 0xff000000;
		Uint32 gmask = 0x00ff0000;
		Uint32 bmask = 0x0000ff00;
		Uint32 amask = 0x000000ff;
	#else
		Uint32 rmask = 0x000000ff;
		Uint32 gmask = 0x0000ff00;
		Uint32 bmask = 0x00ff0000;
		Uint32 amask = 0xff000000;
	#endif
	
	// Crea una nueva superficie con las nuevas dimensiones
	SDL_Surface *ssuperficie = SDL_CreateRGBSurface( 0, anchura, altura, 32, rmask, gmask, bmask, amask );
	
	// Copia el contenido de la imagen cargada hacia la nueva superficie
	SDL_Rect src; src.x = 0; src.y = 0; src.w = superficie -> w; src.h = superficie -> h;
	SDL_Rect dst; dst.x = 0; dst.y = 0; dst.w = anchura; dst.h = altura;
	SDL_BlitScaled( superficie, &src, ssuperficie, &dst );
					
	SDL_FreeSurface( superficie );
	
	return ssuperficie;
}

int potenciaDos( int n )
{
	int i = 1;
	
	while( i < n ){
		i *= 2;
	}
	
	// Define qué está más cerca
	if( absEnteros( ( i / 2 ) - n ) < ( i - n ) ){
		return i / 2;
	}
	
	return i;
}

int absEnteros( int n )
{
	if( n < 0 ){
		return n * -1;
	}
	
	return n;
}
