#include "player.h"
#include "world.h"
#include "include/SDL_image.h"
#include <iostream>
#include <Windows.h>
#include "bullet.h"
Player::Player(SDL_Renderer* renderer, const std::string& imagePath, float startX, float startY, int w, int h)
    : x(startX), y(startY), width(w), height(h), vx(0.0f), vy(10.0f), ax(0.0f), ay(300.0f), speed(200.0f), isJumping(false), isOnGround(false), health(100)
{
    texture = IMG_LoadTexture(renderer, imagePath.c_str());
    if (!texture) {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
    }
 
    destRect = { static_cast<int>(x), static_cast<int>(y), width, height };
}
void Player::HandleCollision(Bullet& bullet)
{
	SDL_Rect bulletRect = bullet.getRect();
	SDL_Rect playerRect = GetRect();
	if (SDL_HasIntersection(&bulletRect, &playerRect)) {
		int damage = 10; // Example damage value
		ApplyDamage(damage);
		std::cout << "Player hit by bullet! Health: " << health << std::endl;
        bullet.checkY(Player::x);
	}
}
void Player::SetPosition(float newX, float newY) {
    x = newX;
    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);
}

void Player::SetVelocityY(float newVy) {
    vy = newVy;
}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

void Player::HandleInput(const Uint8* state) {
    vx = 0;
    if (isJumping = true) {
        isOnGround = false;
    }
    if (state[SDL_SCANCODE_A]) {
        vx = -speed;
    }

    if (state[SDL_SCANCODE_D]) {
        vx = speed;
    }

    if (state[SDL_SCANCODE_F1]) {
		std::cout << "Player position: (" << x << ", " << y << ")" << std::endl;
		std::cout << "Player velocity: (" << vx << ", " << vy << ")" << std::endl;
		std::cout << "Player health: " << health << std::endl;
    }

    if (!isJumping) {
        if (state[SDL_SCANCODE_SPACE]) {
            vy -= 400;
			isJumping = true; 
        }
        else {
            vy = 0;
            isJumping = false;
        }
    }
}

void Player::Update(float deltaTime) {
      vy += deltaTime + 9.81f; //why is this going so slow??
      x += vx * deltaTime;
      y += vy * deltaTime;


    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);

    if (y + height > 600) {
        y = 600 - height;
        vy = 0;
        isOnGround = false;
    }
    if (x + width > 800) {
        x = 800 - width;
        vx = 0;
    }
    else if (x < 0) {
        x = 0;
        vx = 0;
    }
}

SDL_Rect Player::GetRect() const {
    return destRect;
}

void Player::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}

void Player::ApplyDamage(int damage) {
    health -= damage;
    std::cout << health << std::endl;
    if (health <= 0) {

        std::cout << "Player has died!" << std::endl;
        health = 0;
        Sleep(500);
        exit(0);
        
    }
}

float Player::GetX() const {
    return x;
}

float Player::GetY() const {
    return y;
}

bool Player::IsDead() const {
    return health <= 0;
    exit(0);
}

