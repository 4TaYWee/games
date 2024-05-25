#include <SFML/Graphics.hpp>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Player {
  float x, y;
  float xVel, yVel;
  bool isJumping;
  sf::RectangleShape shape; // Added shape member for player sprite
};

struct Platform {
  sf::RectangleShape shape;
  float x, y;
};

// Function prototypes for collision and animation
bool checkCollision(const Player& player, const Platform& platform);
void animatePlayer(Player& player);

int main() {
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Platformer");
  sf::Clock deltaClock;

  Player player;
  player.shape.setSize(sf::Vector2f(30, 30)); // Player size
  player.x = 100;
  player.y = SCREEN_HEIGHT - player.shape.getSize().y;
  player.xVel = 0;
  player.yVel = 0;
  player.isJumping = false;

  std::vector<Platform> platforms;
  platforms.push_back({ sf::Vector2f(200, 20), 300, 400 }); // Add more platforms here
  // ...

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left) {
          player.xVel = -5;
        } else if (event.key.code == sf::Keyboard::Right) {
          player.xVel = 5;
        } else if (event.key.code == sf::Keyboard::Space && !player.isJumping) {
          player.isJumping = true;
          player.yVel = -10;
        }
      }
      if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
          player.xVel = 0;
        }
      }
    }

    // Update game logic
    float dt = deltaClock.restart().asSeconds();
    updateGame(dt, player, platforms); // Pass player and platforms to updateGame

    // Clear window and draw game objects
    window.clear();
    drawGame(player, platforms); // Pass player and platforms to drawGame

    window.display();
  }

  return 0;
}

void updateGame(float dt, Player& player, const std::vector<Platform>& platforms) {
  // Update player position based on velocities
  player.x += player.xVel * dt;
  player.y += player.yVel * dt;

  // Gravity simulation
  player.yVel += 1.5f * dt; // Adjust gravity strength

  // Jumping logic
  if (player.isJumping) {
    player.isJumping = player.yVel < 0; // Stop jumping when reaching peak
  }

  // Collision detection with platforms
  for (const Platform& platform : platforms) {
    if (checkCollision(player, platform)) {
      // Handle collision (e.g., stop falling, adjust y position)
      player.yVel = 0;
      player.y = platform.y - player.shape.getSize().y;
      player.isJumping = false;
    }
  }

  // Check for out-of-bounds and adjust accordingly
  if (player.y >= SCREEN_HEIGHT - player.shape.getSize().y) {
    player.y = SCREEN_HEIGHT - player.shape.getSize().y;
    player.yVel = 0;
  } else if (player.x < 0) {
    player.x = 0;
  } else if (player.x >= SCREEN_WIDTH - player.shape.getSize().x) {
    player.x = SCREEN_WIDTH - player.shape.getSize().x;
  }
}

bool checkCollision(const Player& player, const Platform& platform) {
  return (player.x + player.shape.getSize().x > platform.x &&
          player.x < platform.x + platform.shape.getSize().x &&
          player.y + player.shape.getSize().y > platform.y &&
          player.y < platform.y + platform.shape.getSize().y);
}

void drawGame(const Player& player, const std::vector<Platform>& platforms) {
  animatePlayer(player); // Update animation before drawing

  // Draw player
  window.draw(player.shape);

  // Draw platforms
  for (const Platform& platform : platforms) {
    window.draw(platform.shape);
  }
}
