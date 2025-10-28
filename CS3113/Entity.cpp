/**
* Author: Zirui Xu
* Assignment: Lunar Lander
* Date due: 2025-10-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#include "Entity.h"
#include <string>

Entity::Entity() : position({0,0}), velocity({0,0}), acceleration({0,0}), drawScale(1.0f) {}

Entity::Entity(Vector2 pos, const std::string& texturePath) : position(pos), velocity({0,0}), acceleration({0,0}), drawScale(1.0f)
{
    texture = LoadTexture(texturePath.c_str());
}


Entity::~Entity()
{
    UnloadTexture(texture);
}


void Entity::update(float deltaTime)
{
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}


void Entity::render()
{
    Rectangle src = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle dst = { position.x, position.y, texture.width * drawScale, texture.height * drawScale };
    Vector2 origin = { (texture.width * drawScale) / 2.0f, (texture.height * drawScale) / 2.0f };
    DrawTexturePro(texture, src, dst, origin, 0.0f, WHITE);
}


Vector2 Entity::getPosition() const { return position; }
Vector2 Entity::getVelocity() const { return velocity; }
Vector2 Entity::getSize() const { return { texture.width * drawScale, texture.height * drawScale }; }
void Entity::setPosition(Vector2 pos) { position = pos; }
void Entity::setVelocity(Vector2 vel) { velocity = vel; }
void Entity::setAcceleration(Vector2 acc) { acceleration = acc; }
void Entity::setDrawScale(float s) { drawScale = s; }


