#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "OpenGl_Init/texture.h"
#include "OpenGl_Init/sprite_renderer.h"


class GameObject
{
public:
	    // 全部的状态，不同的道具可能持有不同的状态
    glm::vec2   Position, Size;
    glm::vec3   Color;
    // float       Rotation;
    //bool        issolid;
    //bool        destroyed;
    // 应该持有纹理和渲染器对象，纹理用于初始化，渲染器用于绘制函数
    Texture2D   Sprite;	
    SpriteRenderer* Renderer;
    // GameObject* Player;
    //GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f));

    
    virtual void Draw(SpriteRenderer &renderer);
};



class PannelObject :public GameObject
{
public:
    PannelObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color);
	// Pannel createObject();
};

class BrickObject :public GameObject
{
public:
    bool        IsSolid;
    bool        Destroyed;

    BrickObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color);
    // Bricks createObject();
};

class Ball :public GameObject
{
public:
    // Ball createObject();
};


//class GameObject
//{
//public:
//    // 全部的状态，不同的道具可能持有不同的状态
//    glm::vec2   Position, Size, Velocity;
//    glm::vec3   Color;
//    float       Rotation;
//    bool        IsSolid;
//    bool        Destroyed;
//    // 持有的纹理
//    Texture2D   Sprite;	
//    GameObject();
//    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f),
//        glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
//    // 绘制
//    virtual void Draw(SpriteRenderer &renderer);
//};

#endif