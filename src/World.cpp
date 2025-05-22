#include "World.h"

SDL_Window* World::m_window = nullptr;
SDL_Renderer* World::m_renderer = nullptr;
const Uint8* World::m_keyboardState = nullptr;

World::World()
{
}

World::~World()
{
}

void World::init()
{
    m_run = true;

    initSDL();
    
    int seed = time(NULL);

    srand(seed);

    cout << seed << "\n";


    m_dungeon.init();
}

void World::update()
{
    m_dungeon.update();

    SDL_Event m_event;

    SDL_PollEvent(&m_event);

    if (m_event.button.button == SDL_BUTTON_LEFT)
    {

        int x;
        int y;
        
        SDL_GetMouseState(&x, &y);
        
        cout << "mouse coord is: " << x*mouseMultiply.x <<" " << y*mouseMultiply.y << "\n";
    }

    m_keyboardState = SDL_GetKeyboardState(NULL);

    int2 dir;
    if (m_keyboardState[SDL_SCANCODE_W])
    {
        dir.y += -10;
    }
    if (m_keyboardState[SDL_SCANCODE_S])
    {
        dir.y += 10;
    }
    if (m_keyboardState[SDL_SCANCODE_A])
    {
        dir.x += -10;
    }
    if (m_keyboardState[SDL_SCANCODE_D])
    {
        dir.x += 10;
    }
    
    m_dungeon.move(dir);
}

void World::draw()
{
    SDL_RenderClear(m_renderer);

    m_dungeon.draw();

    SDL_RenderPresent(m_renderer);
}

void World::exit()
{
}

bool World::run()
{
	return m_run;
}


void World::initSDL()
{
    m_window = SDL_CreateWindow("Procedural Dungeon Generation",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
        , 1920, 1080, 0);

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);

    SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    improveRenderer();
}

void World::improveRenderer()
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    auto desktopWidth = DM.w;
    auto desktopHeight = DM.h;

    mouseMultiply.x = (double)m_SCREEN_WIDTH / (double)desktopWidth;
    mouseMultiply.y = (double)m_SCREEN_HEIGHT / (double)desktopHeight;

    if (SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
    {
        cout << "SDL_IMPROVE_RENDERER FAILED: %s\n" << SDL_GetError() << endl;
    }

    SDL_RenderSetLogicalSize(m_renderer, m_SCREEN_WIDTH, m_SCREEN_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

SDL_Texture* World::loadTexture(string path)
{
   
        SDL_Texture* temp;

        SDL_Surface* loadingSurface = SDL_LoadBMP(path.c_str());
        temp = SDL_CreateTextureFromSurface(m_renderer, loadingSurface);
        SDL_FreeSurface(loadingSurface);

        return temp;
}

void World::drawObject(Drawable obj)
{
    SDL_RenderCopy(m_renderer,obj.texture,NULL,&obj.rect);
}

void World::drawObject(DrawableF obj)
{
    SDL_RenderCopyF(m_renderer, obj.texture, NULL, &obj.rect);
}

void World::drawLine(line line)
{
    SDL_RenderDrawLineF(m_renderer, line.start.x, line.start.y, line.finish.x, line.finish.y);
}

void World::drawLine(line line, int r, int g, int b, int opacity)
{
    SDL_SetRenderDrawColor(m_renderer, r, g, b, opacity);
    SDL_RenderDrawLineF(m_renderer, line.start.x, line.start.y, line.finish.x, line.finish.y);
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
}

void World::drawCircle(int32_t centreX, int32_t centreY, int32_t radius)
{

    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);

    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(m_renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(m_renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(m_renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(m_renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(m_renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(m_renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(m_renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(m_renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
}
