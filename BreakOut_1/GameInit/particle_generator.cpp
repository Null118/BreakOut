#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
    : shader(shader), texture(texture), amount(amount)
{
    // 初始化粒子数组和 VAO, VBO
    this->init();
}

void ParticleGenerator::Update(float dt, BallObject &ball, unsigned int newParticles, glm::vec2 offset)
{
    // 将过期的粒子换成新的
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], ball, offset);
    }
    // 更新存活粒子的状态
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt; // 更新存活时间
        if (p.Life > 0.0f)
        {	// 活着的话，更新位置和颜色
            p.Position -= p.Velocity * dt; 
            p.Color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::Draw()
{
    //使用加法混合以实现光晕的效果，即在背景的颜色上，直接加上当前的颜色
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.Use();
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            this->shader.SetVector2f("offset", particle.Position);
            this->shader.SetVector4f("color", particle.Color);
            this->texture.Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // 重新设置混合函数为默认
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // 两个三角形围成四边形，前两个是坐标，后两个是纹理坐标
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    }; 
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);


    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// 直接从记录上次返回的全局变量开始顺序遍历，找不到再从头找
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{

    for (unsigned int i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }

    for (unsigned int i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // 代表没有过期的粒子
    lastUsedParticle = 0;
    return 0;
}

// 重新设定粒子对象的状态
void ParticleGenerator::respawnParticle(Particle &particle, BallObject &ball, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;       // -5~5
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = ball.Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    /* 
    注意到粒子特效（拖尾），由于帧率非常高，所以很快就会被渲染满500个的上限
    渲染满之后仍没有粒子过期，就会有一段时间是空白的，不会产生新的拖尾
    所以想要一直都有拖尾，要么增加最大的渲染粒子数，要么减少粒子的生命周期
    */
    particle.Life = 0.5f;
    particle.Velocity = ball.Velocity * 0.1f;
}