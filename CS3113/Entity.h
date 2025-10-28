/**
* Author: Zirui Xu
* Assignment: Lunar Lander
* Date due: 2025-10-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include <string>


class Entity
{
public:
    Entity();
    Entity(Vector2 pos, const std::string& texturePath);
    ~Entity();

    void update(float deltaTime);
    void render();

    Vector2 getPosition() const;
    Vector2 getVelocity() const;
    Vector2 getSize() const;
    void setPosition(Vector2 pos);
    void setVelocity(Vector2 vel);
    void setAcceleration(Vector2 acc);
    void setDrawScale(float s);

private:
    Texture2D texture;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float drawScale;
};

#endif




