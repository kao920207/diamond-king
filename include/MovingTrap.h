#pragma once
#include "Trap.h"
class MovingTrap : public Trap {
    int dx;
public:
    MovingTrap(int px, int py, int step = 1);
    void update() override;       // 來回移動
};
