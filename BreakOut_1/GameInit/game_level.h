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
    // 每一个砖块都是一个 GameObject 对象，用一个数组储存
    std::vector<GameObject> Bricks;

    GameLevel() { }
    // 把文件里的二维关卡设计读到成员变量中
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);

    void Draw(SpriteRenderer &renderer);

    bool IsCompleted();
private:
    // 依据 tiledata 初始化 GameObject
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif