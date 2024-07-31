#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "OpenGL_Init/shader.h"
#include "OpenGL_Init/texture.h"
#include "game_object.h"



struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};



class ParticleGenerator
{
public:
    // 生成器类只需要一个，绑定好纹理和渲染需要的着色器就可
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    // update all particles
    void Update(float dt, BallObject &object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    // 渲染全部的粒子
    void Draw();
private:
    // 维护一个粒子数组，保存每个粒子的状态
    std::vector<Particle> particles;
    unsigned int amount;
    // 渲染所需的对象
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    // 渲染初始化
    void init();
    // 返回第一个过期的粒子的下标，若小于等于 0 则无过期
    unsigned int firstUnusedParticle();
    // 更新每个例子的状态
    void respawnParticle(Particle &particle, BallObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif