#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <set>
#include <utility>
#include "GameEngine.h"
#include <termios.h>
#include <unistd.h>
#include<player.h>

char getch() {//為了直接在地圖移動
    char buf = 0;
    struct termios old = {};
    tcgetattr(STDIN_FILENO, &old);            
    old.c_lflag &= ~(ICANON | ECHO);          
    tcsetattr(STDIN_FILENO, TCSANOW, &old);   
    read(STDIN_FILENO, &buf, 1);              
    old.c_lflag |= (ICANON | ECHO);           
    tcsetattr(STDIN_FILENO, TCSADRAIN, &old); 
    return buf;
}
GameEngine::GameEngine()
    : player(1,1), seconds(0), running(true)
{
std::srand(std::time(nullptr)); // 初始化隨機種子

    // 隨機產生靜態陷阱
    for (int i = 0; i < 5; ++i) {
        int x = rand() % W;
        int y = rand() % H;
        objects.emplace_back(std::make_unique<Trap>(x, y));
    }

    // 隨機產生動態陷阱
    for (int i = 0; i < 3; ++i) {
        int x = rand() % W;
        int y = rand() % H;
        int dx = (rand() % 2 == 0) ? 1 : -1;
        objects.emplace_back(std::make_unique<MovingTrap>(x, y, dx));
    }

    // 隨機產生寶石
    for (int i = 0; i < TARGET_GEMS; ++i) {
        int x = rand() % W;
        int y = rand() % H;
        objects.emplace_back(std::make_unique<Gem>(x, y));
    }}
void GameEngine::reset() {
    player = Explorer(1, 1);
    seconds = 0;
    running = true;
    objects.clear();
    std::srand(std::time(nullptr)); // 初始化隨機種子
	std::set<std::pair<int, int>> used;
    used.insert({1, 1});
    // 隨機產生靜態陷阱
	int numStatic = 9 + rand() % 7;
	for (int i = 0; i < numStatic; ++i) {
        int x, y;
        do {
            x = rand() % W;
            y = rand() % H;
        } while (used.count({x, y}));
        used.insert({x, y});
        objects.emplace_back(std::make_unique<Trap>(x, y));
    }
	// 隨機數量：3～5個動態陷阱
    int numMoving = 3 + rand() % 3;  
    for (int i = 0; i < numMoving; ++i) {
        int x, y;
        do {
            x = rand() % W;
            y = rand() % H;
        } while (used.count({x, y}));
        used.insert({x, y});
        int dx = (rand() % 2 == 0) ? 1 : -1;
        objects.emplace_back(std::make_unique<MovingTrap>(x, y, dx));
    }
    // 固定數量寶石
	for (int i = 0; i < TARGET_GEMS; ++i) {
        int x, y;
        do {
            x = rand() % W;
            y = rand() % H;
        } while (used.count({x, y}));
        used.insert({x, y});
        objects.emplace_back(std::make_unique<Gem>(x, y));
    }
}
void GameEngine::run() {
	while(true){
		reset();
	std::cout << "\033[1;33m=== Explorer: Gems and Traps ===\033[0m\n";
    std::cout << "Use WASD to move your character (@).\n";
    std::cout << "Collect 5 purple gems (*) and avoid traps (^ or O).\n";
    std::cout << "Press Q anytime to quit.\n";
    std::cout << "Press Enter to start...\n";
    std::string dummy;
    std::getline(std::cin, dummy);
	render();
		while (running) {
        char ch = getch();          // ✅ 非同步鍵盤輸入，不用 Enter
        ch = std::tolower(ch);

        if (ch == 'q') {
            running = false;
            std::cout << "You quit the game.\n";
            return;
        }
		else if (ch == 'r') {
                std::cout << "Restarting...\n";
				
                break; // 跳出內層 while 重新 run()
            }


        player.move(ch, W, H);     // W/H 為地圖範圍，用來做邊界檢查
        updateObjects();
        checkCollision();
        render();

        std::this_thread::sleep_for(std::chrono::milliseconds(80));  // 控制刷新速度
        if (++seconds >= TIME_LIMIT) running = false;
    }
      render(); // 再刷一次畫面（玩家會消失）
    if (player.gems() >= TARGET_GEMS) {
        std::cout << "\033[32mYOU WIN! You collected all the gems!\033[0m\n";
    } else {
        std::cout << "\033[31mGAME OVER: You died or time ran out.\033[0m\n";       								}
		std::cout << "\033[36mPress R to restart or Q to quit...\033[0m\n";

        // 等待玩家輸入 R 或 Q
        while (true) {
            char next = getch();
            next = std::tolower(next);
            if (next == 'r') {
                break;  // 回到最外層 while，重新 reset 並開始新遊戲
            } else if (next == 'q') {
                std::cout << "Goodbye!\n";
                return;
            }
        }
    }
}
void GameEngine::updateObjects() {
    for (auto& obj : objects)
        obj->update();
}

void GameEngine::checkCollision() {
    int px = player.getX();
    int py = player.getY();

    for (auto it = objects.begin(); it != objects.end(); ) {
        GameObject* obj = it->get();
        if (obj->getX() == px && obj->getY() == py) {
            if (dynamic_cast<Trap*>(obj)) {
                std::cout << "You hit a trap!\n";
                running = false;
                return;
            } else if (dynamic_cast<Gem*>(obj)) {
                std::cout << "You got a gem!\n";
                player.addGem();
                it = objects.erase(it);
                continue;
            }
        }
        ++it;
    }
	if (player.gems() >= TARGET_GEMS) {
    std::cout << "\033[32mYou collected all the gems! YOU WIN!\033[0m\n";
    running = false;
}
}
void GameEngine::render() {
    system("clear");  

    // 建立一個地圖陣列，初始化為 '.'
    char map[H][W];
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            map[y][x] = '.';

    // 把所有 GameObject 畫到 map 上（先畫陷阱跟寶石）
    for (auto& obj : objects)
        map[obj->getY()][obj->getX()] = obj->getSprite();

    // 再畫玩家
    map[player.getY()][player.getX()] = player.getSprite();

    // 印出地圖
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
        char c = map[y][x];
        switch (c) {
            case '@': std::cout << "\033[31m@\033[0m"; break;
            case '*': std::cout << "\033[35m*\033[0m"; break;
            case '^': std::cout << "\033[96m^\033[0m"; break;
		    case 'O': std::cout << "\033[91mO\033[0m"; break;
            default:  std::cout << c;
        }
        std::cout << ' ';
    }
    std::cout << '\n';
}
    

std::cout << '\n';
    std::cout << "Gems: " << player.gems() << " / " << TARGET_GEMS
              << " | Time: " << seconds << " / " << TIME_LIMIT << "\n";
    std::cout << "[WASD] Move | [Q] Quit | [R] Restart\n";
}
