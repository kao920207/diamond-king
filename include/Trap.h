#pragma once
#include "gameobject.h"
class Trap : public GameObject {
public:
    Trap(int px, int py);
    void update() override {}     // 靜態陷阱不動
};
