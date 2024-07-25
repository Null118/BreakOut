#include "game_object.h"

// 默认构造函数
//Bricks::Bricks(): Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f),
//    Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color) :
    Position(pos), Size(size), Sprite(sprite), Color(color) {}


BrickObject::BrickObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color)
    : GameObject(pos, size, sprite, color), IsSolid(false), Destroyed(false) { }


PannelObject::PannelObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color)
    : GameObject(pos, size, sprite, color) { }


void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, 0.0f, this->Color);
}