#include <SDL.h>
#include <stdio.h>
#include <windows.h>
#include "queue.h"
#include "coord.h"

void draw_rectangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
int snake(SDL_Renderer* renderer);
void renderSnake(SDL_Renderer* renderer, QUEUE snake);
void frame(SDL_Renderer* renderer, QUEUE snake);
void clear(SDL_Renderer* renderer);
void clear_buffer(void);

#define GAME_WIDTH 60
#define GAME_HEIGHT 40
#define PIXEL_SIZE 10

#define WIDTH (GAME_WIDTH * PIXEL_SIZE)
#define HEIGHT (GAME_HEIGHT * PIXEL_SIZE)

#define FPS 12
#define SLEEP_TIME (1.0/FPS)*1000

int main(int argc, char* argv[]) {
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Canvas Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, // Width
        HEIGHT, // Height
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop flag
    int quit = 0;

    // Event handler
    SDL_Event e;

    // Main loop
    while (!quit) {
        QUEUE snake = makeQueue(1);
        int x = GAME_WIDTH / 2;
        int y = GAME_HEIGHT / 2;
        addToQueue(&snake, x, y);

        COORDINATE dir;
        dir.x = 0;
        dir.y = 0;

        int gameover = 0;
        while (!gameover) {
            clear(renderer);
            // Handle events on the queue
            while (SDL_PollEvent(&e) != 0) {
                // User requests quit
                if (e.type == SDL_QUIT) {
                    quit = 1;
                }
                if (e.type == SDL_KEYDOWN)
                {
                    //Set the proper message surface
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_UP:
                        dir.x = 0;
                        dir.y = -1;
                        break;
                    case SDLK_DOWN:
                        dir.x = 0;
                        dir.y = 1;
                        break;
                    case SDLK_RIGHT:
                        dir.x = 1;
                        dir.y = 0;
                        break;
                    case SDLK_LEFT:
                        dir.x = -1;
                        dir.y = 0;
                        break;
                    default:
                        dir.x = 0;
                        dir.y = 0;
                        break;
                    }
                }
            }

            int newX = snake.data[snake.size - 1].x+ dir.x;
            int newY = snake.data[snake.size - 1].y+dir.y;
            addToQueue(&snake, newX, newY);
            COORDINATE last = getItem(&snake);
            frame(renderer, snake);
            Sleep(SLEEP_TIME);
        }
        delete_queue(&snake);
        // Update screen
        SDL_RenderPresent(renderer);
        
        int NOC;
        char cont;

        printf("\n");

        do
        {
            printf("Would you like to continue? (y/n): ");
            NOC = scanf("%c", &cont);
            clear_buffer();
        } while (NOC != 1 || cont != 'y' && cont != 'n');

        switch (cont) {
            case 'y':
                continue;
                break;
            case 'n':
                quit = 1;
                break;
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int snake(SDL_Renderer* renderer)
{
    
}

void frame(SDL_Renderer* renderer, QUEUE snake)
{
    renderSnake(renderer, snake);
    // Update screen
    SDL_RenderPresent(renderer);
}

void renderSnake(SDL_Renderer* renderer, QUEUE snake)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
    for (int i = 0; i < snake.size; i++)
    {
        draw_rectangle(renderer, snake.data[i].x * PIXEL_SIZE, snake.data[i].y * PIXEL_SIZE, PIXEL_SIZE,PIXEL_SIZE);
    }
}

void clear(SDL_Renderer* renderer)
{
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderClear(renderer);
}

void draw_rectangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2)
{
    SDL_Rect fillRect = { x1, y1, x2, y2 };
    SDL_RenderFillRect(renderer, &fillRect);
}

void clear_buffer(void) {
    char c;
    scanf("%c", &c);
    while (c != '\n')
    {
        scanf("%c", &c);
    }
}