#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "OpenGL_Init/sprite_renderer.h"
#include "ResourceManager/resource_manager.h"



class GameLevel
{
public:
    // ÿһ��ש�鶼��һ�� GameObject ������һ�����鴢��
    std::vector<GameObject> Bricks;

    GameLevel() { }
    // ���ļ���Ķ�ά�ؿ���ƶ�����Ա������
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);

    void Draw(SpriteRenderer &renderer);

    bool IsCompleted();
private:
    // ���� tiledata ��ʼ�� GameObject
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif