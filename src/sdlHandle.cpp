#include "sdlHandle.h"
#include <time.h>

sdlHandle::sdlHandle(int Vcells, int Hcells, int cellSize)
{
    cell_size = cellSize;
    V_cells = Vcells;
    H_cells = Hcells;
    SCREEN_HEIGHT = V_cells * cell_size;
    SCREEN_WIDTH = H_cells * cell_size;
}

// Starts up SDL and creates window
bool sdlHandle::init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        gWindow = SDL_CreateWindow("Maze collapse", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }

    return success;
}

// Frees media and shuts down SDL
void sdlHandle::close()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

void sdlHandle::clear()
{
    SDL_SetRenderDrawColor(gRenderer, 0xAA, 0x80, 0xAA, 0xFF);
    SDL_RenderClear(gRenderer);
}

void sdlHandle::updateScreen()
{
    SDL_RenderPresent(gRenderer);
}

/* Wait for close signal and close window, return 0 if closed by user, 1 if timeout exceeded */
int sdlHandle::waitAndClose(int timeout)
{
    double endTime = time(NULL) + timeout;
    float sleepTime = .5;
    // While application is running
    while (time(NULL) < endTime)
    {
        if (checkForClose())
        {
            return 0;
        }
        sleep(sleepTime);
    }
    close();
    return 1;
}

/* Check events for close signal, close window and return 1 if necessary, 0 otherwise */
int sdlHandle::checkForClose()
{
    // Main loop flag
    bool quit = false;
    // Event handler
    SDL_Event e;
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        // User requests quit
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
    }
    if (quit)
    {
        close();
        return 1;
    }
    return 0;
}

void sdlHandle::drawCellGreyScale(int x, int y, float color)
{
    SDL_Rect fillRect = {y * cell_size, x * cell_size, cell_size, cell_size};
    if (color >= 0 && color <= 1)
    {
        int trueColor = 0xFF * (1 - color);
        SDL_SetRenderDrawColor(gRenderer, trueColor, trueColor, trueColor, 0xFF);
    }
    else
    {
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
    }

    SDL_RenderFillRect(gRenderer, &fillRect);
}
