#ifndef BULLET_H
#define BULLET_H

#include "include/SDL_image.h"
#include <string>
#include <iostream>
#include "player.h"
class Bullet {
public:
    Bullet(SDL_Renderer* renderer, const std::string& imagePath, float startX, float startY, int w, int h)
        : x_(startX), y_(startY), width(w), height(h), velocityX(0), velocityY(0) {
        texture = IMG_LoadTexture(renderer, imagePath.c_str());
        if (!texture) {
            std::cerr << "Failed to load bullet texture: " << IMG_GetError() << std::endl;
        }
        destRect = { static_cast<int>(x_), static_cast<int>(y_), width, height };
    }

    void update(float deltaTime) {
        velocityY += deltaTime + 9.81f;
        x_ += velocityX * deltaTime;
        y_ += velocityY * deltaTime;
        destRect.x = static_cast<int>(x_);
        destRect.y = static_cast<int>(y_);
    }
    void Render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    }

    void checkY(float x) {
        if (y_ > 600) { // Assuming the bottom of the screen is at y = 600
            x_ = x + (rand() % 100 - 50);  // Reset bullet position to a random location
            y_ = 0;
            velocityY = 3;
            timesdodged += 1;
        }
    }

    int timesdodged = 0;
    int getX() const { return static_cast<int>(x_); }
    int getY() const { return static_cast<int>(y_); }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getVX() const { return static_cast<int>(velocityX); }
    int getVY() const { return static_cast<int>(velocityY); }
    SDL_Rect getRect() const { return destRect; }
    float x_, y_;
    float velocityX, velocityY;
private:
    int width, height;
    SDL_Texture* texture;
    SDL_Rect destRect;
};

#endif // BULLET_H
