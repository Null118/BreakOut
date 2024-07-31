#include "2.game.h"
#include "ResourceManager/resource_manager.h"
#include "OpenGL_Init/sprite_renderer.h"
#include "game_object.h"
#include "particle_generator.h"
// 测试用，游戏类需要持有一个渲染器对象，一个玩家对象
//SpriteRenderer* Renderer;
//GameObject* Player;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Pannel;
    delete Ball;
    delete Particles;
    delete Effects;
}

// 用于所有绘制需要的资源的加载
void Game::Init()
{
    // 创建别名为 sprite 的着色器
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    // 创建渲染粒子的着色器
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
    // 创建渲染特效的着色器
    ResourceManager::LoadShader("shaders/post_processing.vs", "shaders/post_processing.fs", nullptr, "postprocessing");
    // 设定投影矩阵，因为是二维游戏，不需要观察矩阵
    // 着色器中的其他 uniform 矩阵
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    // 将实际渲染的工作交给 渲染类，包括 变换，绑定纹理和显示
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // 使用资源管理器加载纹理
    ResourceManager::LoadTexture("../textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("../textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../textures/block.png", false, "block");
    ResourceManager::LoadTexture("../textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("../textures/particle.png", GL_TRUE, "particle");
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
    // 创建粒子特效器
    Particles = new ParticleGenerator(
        ResourceManager::GetShader("particle"),
        ResourceManager::GetTexture("particle"),
        2000
    );
    // 创建特效控制器
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
    // Effects->Chaos = true;
}

// 主程序的渲染循环时每次都会调用 update，渲染每一帧都需要更新游戏状态
void Game::Update(float dt)
{
    // 计算每帧小球的移动
    Ball->Move(dt, this->Width);
    // 检测是否碰撞
    this->DoCollisions();
    // 检测是否结束游戏
    if (Ball->Position.y >= this->Height) // 球是否接触底部边界？
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
    // 更新粒子特效
    Particles->Update(dt, *Ball, 2, glm::vec2(BALL_RADIUS / 2));
    // 判断晃动的时间是否结束
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }
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
        // 被 begin 和 end 圈起来的部分代表渲染在 MSFBO 的对象，即是需要经过后期处理的对象
        Effects->BeginRender();

        // 先画一个背景
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // 画关卡里的砖块
        this->Levels[this->Level].Draw(*Renderer);
        // 画玩家（挡板）
        Pannel->Draw(*Renderer);
        // 画粒子特效，要注意渲染的顺序，特效在其他东西后渲染，说明它会覆盖在他们上面，而最后渲染球，确保球是最优先显示的
        Particles->Draw();
        // 画 球
        Ball->Draw(*Renderer);

        Effects->EndRender();
        Effects->Render(glfwGetTime());     // 处理完后再绘制
    }
}

// 判断是否碰撞
Collision Game::CheckCollision(BallObject& one, GameObject& two) // 长方形 - 圆形 碰撞检测
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

    if (glm::length(difference) <= one.Radius)
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

// 执行发生碰撞后的逻辑
// 碰撞的话，置 Destroyed 为 true，停止渲染
// 同时计算碰撞侵入的程度，并且基于碰撞方向使球的位置矢量与其相加或相减
void Game::DoCollisions()
{
    // 计算球和砖块的碰撞，并根据结果改变球的速度、砖块的显示状态
    for (BrickObject& box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // 如果collision 是 true
            {
                // 如果砖块不是实心就销毁砖块
                if (!box.IsSolid)
                    box.Destroyed = GL_TRUE;
                else
                {   // 如果是实心的砖块则激活shake特效
                    ShakeTime = 0.05f;
                    Effects->Shake = true;
                }
                // 碰撞处理
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) // 水平方向碰撞
                {
                    Ball->Velocity.x = -Ball->Velocity.x; // 反转水平速度
                    // 重定位
                    GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        Ball->Position.x += penetration; // 将球右移
                    else
                        Ball->Position.x -= penetration; // 将球左移
                }
                else // 垂直方向碰撞
                {
                    Ball->Velocity.y = -Ball->Velocity.y; // 反转垂直速度
                    // 重定位
                    GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        Ball->Position.y -= penetration; // 将球上移
                    else
                        Ball->Position.y += penetration; // 将球下移
                }
            }
        }
    }
    // 计算球与板子的碰撞，并根据结果改变球的速度
    Collision result = CheckCollision(*Ball, *Pannel);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
        GLfloat centerBoard = Pannel->Position.x + Pannel->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Pannel->Size.x / 2);
        // 依据结果移动
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        // 如果球入侵板子太多，不足以在一帧内弹出板子，那么会一直在板子内部反转速度
        // Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    }
}



Direction Game::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // 上
        glm::vec2(1.0f, 0.0f),  // 右
        glm::vec2(0.0f, -1.0f), // 下
        glm::vec2(-1.0f, 0.0f)  // 左
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}



void Game::ResetLevel()
{
    if (this->Level == 0)this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 1)
        this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 2)
        this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 3)
        this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    Pannel->Size = PLAYER_SIZE;
    Pannel->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Pannel->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}
