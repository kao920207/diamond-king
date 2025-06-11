#include <vector>
#include <memory>
#include "player.h"
#include "Trap.h"
#include "MovingTrap.h"
#include "Gem.h"

class GameEngine {
    const int W = 20, H = 10, TARGET_GEMS = 5, TIME_LIMIT = 50;
    Explorer player;
    std::vector<std::unique_ptr<GameObject>> objects;
    int seconds;
    bool running;

    void reset();
    void updateObjects();
    void checkCollision();
    void render();
public:
    GameEngine();
    void run();
};
