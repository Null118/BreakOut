#include "2.game.h"
#include "ResourceManager/resource_manager.h"
#include "OpenGL_Init/sprite_renderer.h"
#include "game_object.h"
// �����ã���Ϸ����Ҫ����һ����Ⱦ������һ����Ҷ���
//SpriteRenderer* Renderer;
//GameObject* Player;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    
}

// �������л�����Ҫ����Դ�ļ���
void Game::Init()
{
    // ��������Ϊ sprite ����ɫ��
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    // �趨ͶӰ������Ϊ�Ƕ�ά��Ϸ������Ҫ�۲����
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // ��ʵ����Ⱦ�Ĺ������� ��Ⱦ�࣬���� �任�����������ʾ
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // ʹ����Դ��������������
    ResourceManager::LoadTexture("../textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("../textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../textures/block.png", false, "block");
    ResourceManager::LoadTexture("../textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../textures/paddle.png", true, "paddle");
    // �����ؿ���ÿ���ؿ������˲�ͬ��ש����������
    GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // ������ң����壩
    glm::vec2 pannelPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Pannel = new PannelObject(pannelPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"), glm::vec3(1.0f));
    // ������
    glm::vec2 ballPos = pannelPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, ResourceManager::GetTexture("face"), INITIAL_BALL_VELOCITY);
}

void Game::Update(float dt)
{
    Ball->Move(dt, this->Width);
}

// �����������A, D �����������ƶ������Ҳ��ܳ����߽�
void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // ����Ҫ��С��һ��
        if (this->Keys[GLFW_KEY_A])
        {
            if (Pannel->Position.x >= 0.0f) {
                Pannel->Position.x -= velocity;
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Pannel->Position.x <= this->Width - Pannel->Size.x) {
                Pannel->Position.x += velocity;
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE])
            Ball->Stuck = false;
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // �Ȼ�һ������
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // ���ؿ����ש��
        this->Levels[this->Level].Draw(*Renderer);
        // ����ң����壩
        Pannel->Draw(*Renderer);
        // �� ��
        Ball->Draw(*Renderer);
    }
}