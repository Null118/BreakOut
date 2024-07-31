#include "2.game.h"
#include "ResourceManager/resource_manager.h"
#include "OpenGL_Init/sprite_renderer.h"
#include "game_object.h"
#include "particle_generator.h"
// �����ã���Ϸ����Ҫ����һ����Ⱦ������һ����Ҷ���
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

// �������л�����Ҫ����Դ�ļ���
void Game::Init()
{
    // ��������Ϊ sprite ����ɫ��
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    // ������Ⱦ���ӵ���ɫ��
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
    // ������Ⱦ��Ч����ɫ��
    ResourceManager::LoadShader("shaders/post_processing.vs", "shaders/post_processing.fs", nullptr, "postprocessing");
    // �趨ͶӰ������Ϊ�Ƕ�ά��Ϸ������Ҫ�۲����
    // ��ɫ���е����� uniform ����
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    // ��ʵ����Ⱦ�Ĺ������� ��Ⱦ�࣬���� �任�����������ʾ
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // ʹ����Դ��������������
    ResourceManager::LoadTexture("../textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("../textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../textures/block.png", false, "block");
    ResourceManager::LoadTexture("../textures/block_solid.png", false, "block_solid");
    ResourceManager::LoadTexture("../textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("../textures/particle.png", GL_TRUE, "particle");
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
    // ����������Ч��
    Particles = new ParticleGenerator(
        ResourceManager::GetShader("particle"),
        ResourceManager::GetTexture("particle"),
        2000
    );
    // ������Ч������
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
    // Effects->Chaos = true;
}

// ���������Ⱦѭ��ʱÿ�ζ������ update����Ⱦÿһ֡����Ҫ������Ϸ״̬
void Game::Update(float dt)
{
    // ����ÿ֡С����ƶ�
    Ball->Move(dt, this->Width);
    // ����Ƿ���ײ
    this->DoCollisions();
    // ����Ƿ������Ϸ
    if (Ball->Position.y >= this->Height) // ���Ƿ�Ӵ��ײ��߽磿
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
    // ����������Ч
    Particles->Update(dt, *Ball, 2, glm::vec2(BALL_RADIUS / 2));
    // �жϻζ���ʱ���Ƿ����
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = false;
    }
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

// �� update ���룬ǰ���Ǹ���״̬�ĺ������ڸ�����֮���ٵ��ô˺�����Ⱦ
void Game::Render()
{
    if (this->State == GAME_ACTIVE)
    {
        // �� begin �� end Ȧ�����Ĳ��ִ�����Ⱦ�� MSFBO �Ķ��󣬼�����Ҫ�������ڴ���Ķ���
        Effects->BeginRender();

        // �Ȼ�һ������
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // ���ؿ����ש��
        this->Levels[this->Level].Draw(*Renderer);
        // ����ң����壩
        Pannel->Draw(*Renderer);
        // ��������Ч��Ҫע����Ⱦ��˳����Ч��������������Ⱦ��˵�����Ḳ�����������棬�������Ⱦ��ȷ��������������ʾ��
        Particles->Draw();
        // �� ��
        Ball->Draw(*Renderer);

        Effects->EndRender();
        Effects->Render(glfwGetTime());     // ��������ٻ���
    }
}

// �ж��Ƿ���ײ
Collision Game::CheckCollision(BallObject& one, GameObject& two) // ������ - Բ�� ��ײ���
{
    // �����ϽǼ��ϰ뾶���õ����ĵ�
    glm::vec2 center(one.Position + one.Radius);
    // ͬ�ϣ�����ש������ĵ�
    glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // �Ȼ�ô�Բ��ָ��������ĵ�����
    // Ȼ���ð�߳����������ҳ���Բ����ͬһˮƽ/��ֱ���ϵľ����ϵĵ�
    // ��ʱ closest ��Բ ���볤������Զ��һ�㣨��������ƽ�У��Ǿ���Բ�����ұߵĵ��������Զ����Ӧ�����ε��м䣩�ӳ������������εĵ㣬���������ĵľ���
    // ������� difference ���� ������Բ�����һ�� �� Բ���� ������
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;

    if (glm::length(difference) <= one.Radius)
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

// ִ�з�����ײ����߼�
// ��ײ�Ļ����� Destroyed Ϊ true��ֹͣ��Ⱦ
// ͬʱ������ײ����ĳ̶ȣ����һ�����ײ����ʹ���λ��ʸ��������ӻ����
void Game::DoCollisions()
{
    // �������ש�����ײ�������ݽ���ı�����ٶȡ�ש�����ʾ״̬
    for (BrickObject& box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // ���collision �� true
            {
                // ���ש�鲻��ʵ�ľ�����ש��
                if (!box.IsSolid)
                    box.Destroyed = GL_TRUE;
                else
                {   // �����ʵ�ĵ�ש���򼤻�shake��Ч
                    ShakeTime = 0.05f;
                    Effects->Shake = true;
                }
                // ��ײ����
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) // ˮƽ������ײ
                {
                    Ball->Velocity.x = -Ball->Velocity.x; // ��תˮƽ�ٶ�
                    // �ض�λ
                    GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        Ball->Position.x += penetration; // ��������
                    else
                        Ball->Position.x -= penetration; // ��������
                }
                else // ��ֱ������ײ
                {
                    Ball->Velocity.y = -Ball->Velocity.y; // ��ת��ֱ�ٶ�
                    // �ض�λ
                    GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        Ball->Position.y -= penetration; // ��������
                    else
                        Ball->Position.y += penetration; // ��������
                }
            }
        }
    }
    // ����������ӵ���ײ�������ݽ���ı�����ٶ�
    Collision result = CheckCollision(*Ball, *Pannel);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // ��������˵�����ĸ�λ�ã������������ĸ�λ�����ı��ٶ�
        GLfloat centerBoard = Pannel->Position.x + Pannel->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Pannel->Size.x / 2);
        // ���ݽ���ƶ�
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        // ��������ְ���̫�࣬��������һ֡�ڵ������ӣ���ô��һֱ�ڰ����ڲ���ת�ٶ�
        // Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    }
}



Direction Game::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // ��
        glm::vec2(1.0f, 0.0f),  // ��
        glm::vec2(0.0f, -1.0f), // ��
        glm::vec2(-1.0f, 0.0f)  // ��
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
