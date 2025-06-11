#include "gameobject.h"

GameObject::GameObject(int px, int py, char s)
    : x(px), y(py), sprite(s) {}

int  GameObject::getX() const { return x; }
int  GameObject::getY() const { return y; }
char GameObject::getSprite() const { return sprite; }

