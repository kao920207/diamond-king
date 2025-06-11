#pragma once
class GameObject {
protected:
    int x, y;
    char sprite;
public:
    GameObject(int px, int py, char s);
    virtual ~GameObject() = default;

    virtual void update() = 0;          
    int  getX() const;
    int  getY() const;
    char getSprite() const;
};
