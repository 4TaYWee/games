#include <SDL2/SDL.h>
#include <iostream>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_WIDTH = 50;
const int PLAYER_HEIGHT = 50;
const int GRAVITY = 1;
const int JUMP_FORCE = 15;
const int GROUND_HEIGHT = SCREEN_HEIGHT - 100;

// Player class
class Player {
public:
    int x, y;
    int velX, velY;
    bool isJumping;

    Player() {
        x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
        y = GROUND_HEIGHT - PLAYER_HEIGHT;
        velX = 0;
        velY = 0;
        isJumping = false;
    }

    void handleInput(SDL_Event& event) {
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !isJumping) {
            isJumping = true;
            velY = -JUMP_FORCE;
        }
    }

    void update() {
        // Apply gravity
        velY += GRAVITY;
        // Update position
        x += velX;
        y += velY;
        // Check if the player is on the ground
        if (y >= GROUND_HEIGHT - PLAYER_HEIGHT) {
            y = GROUND_HEIGHT - PLAYER_HEIGHT;
            velY = 0;
            isJumping = false;
        }
    }
};

// Main function
int main() {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning = true;
    SDL_Event event;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    window = SDL_CreateWindow("Platformer Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize player
    Player player;

    // Main game loop
    while (isRunning) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            player.handleInput(event);
        }

        // Update
        player.update();

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render player
        SDL_Rect playerRect = {player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        // Render present
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
