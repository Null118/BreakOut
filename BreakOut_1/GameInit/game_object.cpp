#include "game_object.h"


GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color) :
    Position(pos), Size(size), Sprite(sprite), Color(color) {}

// 子类对象不能赋值父类的成员变量，必须初始化父类对象，交由父类对象赋值
BrickObject::BrickObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color)
    : GameObject(pos, size, sprite, color), IsSolid(false), Destroyed(false) { }


PannelObject::PannelObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color)
    : GameObject(pos, size, sprite, color) { }


void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, 0.0f, this->Color);
}


BallObject::BallObject(glm::vec2 pos, float radius, Texture2D sprite, glm::vec2 velocity)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f)), 
    Radius(radius), Velocity(velocity), Stuck(true), Sticky(false), PassThrough(false) { }

glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
    // 如果允许被发射
    if (!this->Stuck)
    {
        // 移动
        this->Position += this->Velocity * dt;
        // 边界探测，如果碰到，则反转速度
        if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if (this->Position.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }
    }
    return this->Position;
}

// 如果掉下去的话就重置球
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
    this->Sticky = false;
    this->PassThrough = false;

}