#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "OpenGl_Init/texture.h"
#include "OpenGl_Init/sprite_renderer.h"


class GameObject
{
public:
	// ��Ϊ���࣬���������˻���������ı���
    // λ�úʹ�С���������ɫ
    glm::vec2   Position, Size;
    glm::vec3   Color;
    Texture2D   Sprite;	
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color);

    
    virtual void Draw(SpriteRenderer &renderer);
};



class PannelObject :public GameObject
{
public:
    PannelObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color);
};

class BrickObject :public GameObject
{
public:
    // ש����е����ԣ��Ƿ���Ա��ݻ٣��Ƿ��Ѿ����ݻ�
    bool        IsSolid;
    bool        Destroyed;

    BrickObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color);
};

class BallObject : public GameObject
{
public:
    // ����е����ԣ��뾶���Ƿ񱻷��䣬
    float       Radius;
    bool        Stuck;
    glm::vec2   Velocity;
    bool        Sticky, PassThrough;

    BallObject(glm::vec2 pos, float radius, Texture2D sprite, glm::vec2 velocity);
    // moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
    glm::vec2 Move(float dt, unsigned int window_width);
    // resets the ball to original state with given position and velocity
    void      Reset(glm::vec2 position, glm::vec2 velocity);
};


#endif