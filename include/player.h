#ifndef PLAYER_H
#define PLAYER_H
#include "gameobject.h"

class Explorer : public GameObject {
    int gemCount;
public:
    Explorer(int px, int py);
    void move(char dir,int W,int H);
    void addGem();
    int  gems() const;
    void update() override {}     
};
#endif
