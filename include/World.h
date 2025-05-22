#pragma once


#include "defines.h"
#include "Dungeon.h"

class World
{
public:
	World();
	~World();

	void init();
	void update();
	void draw();
	void exit();

	bool run();

	static SDL_Texture* loadTexture(string path);
	static void drawObject(Drawable obj);
	static void drawObject(DrawableF obj);
	static void drawLine(line line);
	static void drawLine(line line, int r, int g, int b, int opacity);
	static void drawCircle(int32_t centreX, int32_t centreY, int32_t radius);

	static SDL_Renderer* m_renderer;
private:

	bool m_run;
	const int m_SCREEN_WIDTH = 1920;
	const int m_SCREEN_HEIGHT = 1080;

	static const Uint8* m_keyboardState;


	static SDL_Window* m_window;

	float2 mouseMultiply;


	Dungeon m_dungeon;


	void initSDL();

	void improveRenderer();

};
