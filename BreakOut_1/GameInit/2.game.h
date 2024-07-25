#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};


// ��ң����壩�ĳ�ʼ������
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);


class Game
{
public:
    // ��Ϸӵ�е�״̬��������Ϸ״̬��ͨ����񣩡��������������йؿ�����ǰ�ؿ�
    GameState               State;	
    bool                    Keys[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    unsigned int            Level;
    SpriteRenderer* Renderer;
    PannelObject* Pannel;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif