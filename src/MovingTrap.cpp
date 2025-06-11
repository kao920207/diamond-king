#include "MovingTrap.h"
#include<cstdlib>
MovingTrap::MovingTrap(int px, int py, int step)
    : Trap(px, py), dx(step) { sprite = 'O'; }

void MovingTrap::update() {
 int dir = rand() % 4;
    switch (dir) {
        case 0: if (x > 0)     --x; break;
        case 1: if (x < 19)    ++x; break;
        case 2: if (y > 0)     --y; break;
        case 3: if (y < 9)     ++y; break;
    
}
}
