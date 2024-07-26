#include "2.game.h"
#include "ResourceManager/resource_manager.h"
#include "OpenGL_Init/sprite_renderer.h"
#include "game_object.h"
// 测试用，游戏类需要持有一个渲染器对象，一个玩家对象
//SpriteRenderer* Renderer;
//GameObject* Player;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    
}

// 用于所有绘制需要的资源的加载
void Game::Init()
{
    // 创建别名为 sprite 的着色器
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    // 设定投影矩阵，因为是二维游戏，不需要观察矩阵
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // 将实际渲染的工作交给 渲染类，包括 变换，绑定纹理和显示
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // 使用资源管理器加载纹理
    ResourceManager::LoadTexture("../textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("../textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../textures/block.png", false, "block");
    ResourceManager::LoadTexture("../textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../textures/paddle.png", true, "paddle");
    // 创建关卡，每个关卡保存了不同的砖块对象的数组
    GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height / 2);
    GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height / 2);
    GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height / 2);
    GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // 创建玩家（挡板）
    glm::vec2 pannelPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Pannel = new PannelObject(pannelPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"), glm::vec3(1.0f));
    // 创建球
    glm::vec2 ballPos = pannelPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, ResourceManager::GetTexture("face"), INITIAL_BALL_VELOCITY);
}

// 主程序的渲染循环时每次都会调用 update，渲染每一帧都需要更新游戏状态
void Game::Update(float dt)
{
    // 计算每帧小球的移动
    Ball->Move(dt, this->Width);
    // 检测是否碰撞
    this->DoCollisions();
}

// 处理按键情况，A, D 键控制左右移动，并且不能超过边界
void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // 板子要和小球一起动
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

// 和 update 分离，前者是更新状态的函数，在更新完之后再调用此函数渲染
void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // 先画一个背景
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // 画关卡里的砖块
        this->Levels[this->Level].Draw(*Renderer);
        // 画玩家（挡板）
        Pannel->Draw(*Renderer);
        // 画 球
        Ball->Draw(*Renderer);
    }
}

// 判断是否碰撞
GLboolean CheckCollision(BallObject& one, BrickObject& two) // 长方形 - 圆形 碰撞检测
{
    // 从左上角加上半径，得到中心点
    glm::vec2 center(one.Position + one.Radius);
    // 同上，计算砖块的中心点
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // 先获得从圆心指向矩形中心的向量
    // 然后用半边长限制它，找出和圆心在同一水平/垂直线上的矩形上的点
    // 此时 closest 是圆 上离长方形最远的一点（想象它们平行，那就是圆的最右边的点离矩形最远，对应到矩形的中间）延长出来交到矩形的点，到矩形中心的距离
    // 所以这个 difference 代表 矩形离圆最近的一点 到 圆中心 的向量
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;


    return glm::length(difference) < one.Radius;
}

// 碰撞的话，置 Destroyed 为 true，停止渲染
void Game::DoCollisions()
{
    for (BrickObject& box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            if (CheckCollision(*Ball, box))
            {
                if (!box.IsSolid)
                    box.Destroyed = GL_TRUE;
            }
        }
    }
}