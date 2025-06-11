#include "player.h"

Explorer::Explorer(int px, int py)
    : GameObject(px, py, '@'), gemCount(0) {}

void Explorer::move(char dir,int W,int H) {
    switch (dir) {
        case 'w': if (y > 0)     --y; break;
        case 's': if (y < H - 1) ++y; break;
        case 'a': if (x > 0)     --x; break;
        case 'd': if (x < W - 1) ++x; break;

	}
}
void Explorer::addGem() { ++gemCount; }
int  Explorer::gems() const { return gemCount; }
