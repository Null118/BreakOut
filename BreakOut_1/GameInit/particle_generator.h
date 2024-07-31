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
    // ��������ֻ��Ҫһ�����󶨺��������Ⱦ��Ҫ����ɫ���Ϳ�
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    // update all particles
    void Update(float dt, BallObject &object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    // ��Ⱦȫ��������
    void Draw();
private:
    // ά��һ���������飬����ÿ�����ӵ�״̬
    std::vector<Particle> particles;
    unsigned int amount;
    // ��Ⱦ����Ķ���
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    // ��Ⱦ��ʼ��
    void init();
    // ���ص�һ�����ڵ����ӵ��±꣬��С�ڵ��� 0 ���޹���
    unsigned int firstUnusedParticle();
    // ����ÿ�����ӵ�״̬
    void respawnParticle(Particle &particle, BallObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif