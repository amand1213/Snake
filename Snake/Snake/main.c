#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "queue.h"
#include "coord.h"

void draw_rectangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
void renderSnake(SDL_Renderer* renderer, QUEUE snake);
void renderApples(SDL_Renderer* renderer, QUEUE apples);
void frame(SDL_Renderer* renderer, QUEUE snake, QUEUE apples);
void newApple(QUEUE snake, QUEUE* apples, COORDINATE oldApple);
void clear(SDL_Renderer* renderer);
void clear_buffer(void);

#define GAME_WIDTH 60
#define GAME_HEIGHT 40
#define PIXEL_SIZE 10

#define WIDTH (GAME_WIDTH * PIXEL_SIZE)
#define HEIGHT (GAME_HEIGHT * PIXEL_SIZE)

#define FPS 10
#define SLEEP_TIME (1.0/FPS)*1000

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
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
        QUEUE snake = makeQueue();
        int x = GAME_WIDTH / 2;
        int y = GAME_HEIGHT / 2;
        addToQueue(&snake, x, y);
        
        int snakeSize = 1;
        int growth = 3;

        QUEUE apples = makeQueue();
        x = 0;
        y = 0;
        addToQueue(&apples, x, y);
        COORDINATE apple;
        apple.x = x;
        apple.y = y;
        newApple(snake, &apples, apple);

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
                        if (dir.y == 1) break;
                        dir.x = 0;
                        dir.y = -1;
                        break;
                    case SDLK_DOWN:
                        if (dir.y == -1) break;
                        dir.x = 0;
                        dir.y = 1;
                        break;
                    case SDLK_RIGHT:
                        if (dir.x == -1) break;
                        dir.x = 1;
                        dir.y = 0;
                        break;
                    case SDLK_LEFT:
                        if (dir.x == 1) break;
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
            COORDINATE newCoord;

            newCoord.x = newX;
            newCoord.y = newY;

            // Collision with apple
            if (xyInQueue(apples, newX, newY)) {
                snakeSize += growth;
                newApple(snake, &apples, newCoord);
            }
            
            // Collision with self
            if (xyInQueue(snake, newX, newY) && snakeSize != 1) {
                gameover = 1;
                break;
            }

            if (newX < 0)
            {
                newX = GAME_WIDTH - 1;
            }

            if (newX >= GAME_WIDTH)
            {
                newX = 0;
            }

            if (newY < 0)
            {
                newY = GAME_HEIGHT - 1; 
            }

            if (newY >= GAME_HEIGHT)
            {
                newY = 0;
            }

            addToQueue(&snake, newX, newY);
            
            if (snake.size > snakeSize) {
                COORDINATE last = getItem(&snake);
            }
            frame(renderer, snake, apples);
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

void frame(SDL_Renderer* renderer, QUEUE snake, QUEUE apples)
{
    renderSnake(renderer, snake);
    renderApples(renderer, apples);
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

void renderApples(SDL_Renderer* renderer, QUEUE apples)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    for (int i = 0; i < apples.size; i++)
    {
        draw_rectangle(renderer, apples.data[i].x * PIXEL_SIZE, apples.data[i].y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
    }
}

void clear(SDL_Renderer* renderer)
{
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderClear(renderer);
}

void newApple(QUEUE snake, QUEUE* apples, COORDINATE oldApple) {
    removeItem(apples, oldApple);

    int newX;
    int newY;
    do {
        newX = random(GAME_WIDTH-1, 0);
        newY = random(GAME_HEIGHT-1, 0);
    } while (xyInQueue(snake, newX, newY));
    addToQueue(apples, newX, newY);
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

int random(int MAX, int MIN) {
    return rand() % (MAX - MIN + 1) + MIN;
}