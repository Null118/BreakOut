#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "OpenGl_Init/texture.h"
#include "OpenGl_Init/sprite_renderer.h"


class GameObject
{
public:
	    // ȫ����״̬����ͬ�ĵ��߿��ܳ��в�ͬ��״̬
    glm::vec2   Position, Size;
    glm::vec3   Color;
    // float       Rotation;
    //bool        issolid;
    //bool        destroyed;
    // Ӧ�ó����������Ⱦ�������������ڳ�ʼ������Ⱦ�����ڻ��ƺ���
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
//    // ȫ����״̬����ͬ�ĵ��߿��ܳ��в�ͬ��״̬
//    glm::vec2   Position, Size, Velocity;
//    glm::vec3   Color;
//    float       Rotation;
//    bool        IsSolid;
//    bool        Destroyed;
//    // ���е�����
//    Texture2D   Sprite;	
//    GameObject();
//    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f),
//        glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
//    // ����
//    virtual void Draw(SpriteRenderer &renderer);
//};

#endif