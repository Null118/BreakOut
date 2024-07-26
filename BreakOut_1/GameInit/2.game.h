#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"

// 游戏状态
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};


// 玩家（挡板）的初始化数据
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

// 球 的初始化数据
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;


class Game
{
public:
    // 游戏拥有的状态，包括游戏状态（通关与否）、按键、长宽、所有关卡、当前关卡
    GameState               State;	
    bool                    Keys[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel>  Levels;
    unsigned int            Level;
    SpriteRenderer* Renderer;
    PannelObject* Pannel;
    BallObject* Ball;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
};

#endif