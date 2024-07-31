#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"
#include "particle_generator.h"
#include "Game_Process/post_processor.h"

// ��Ϸ״̬
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// ö�����ͣ������ĸ��������ڷ�ת��Ӧ������ٶ�
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// ��ײ��⺯����Ҫ���� �Ƿ���ײ����ײ����͵�ǰ����ķ���������Ԫ�飬���ڴ�����ײ
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;


// ��ң����壩�ĳ�ʼ������
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

// �� �ĳ�ʼ������
const glm::vec2 INITIAL_BALL_VELOCITY(150.0f, -500.0f);
const GLfloat BALL_RADIUS = 12.5f;


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
    BallObject* Ball;
    // ������
    ParticleGenerator* Particles;
    //��Ч��
    PostProcessor* Effects;
    GLfloat            ShakeTime = 0.0f;

    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    Collision CheckCollision(BallObject& one, GameObject& two);
    void DoCollisions();
    Direction VectorDirection(glm::vec2 target);
    void ResetPlayer();
    void ResetLevel();
};

#endif