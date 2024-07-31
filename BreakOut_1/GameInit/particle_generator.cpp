#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
    : shader(shader), texture(texture), amount(amount)
{
    // ��ʼ����������� VAO, VBO
    this->init();
}

void ParticleGenerator::Update(float dt, BallObject &ball, unsigned int newParticles, glm::vec2 offset)
{
    // �����ڵ����ӻ����µ�
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], ball, offset);
    }
    // ���´�����ӵ�״̬
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt; // ���´��ʱ��
        if (p.Life > 0.0f)
        {	// ���ŵĻ�������λ�ú���ɫ
            p.Position -= p.Velocity * dt; 
            p.Color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::Draw()
{
    //ʹ�üӷ������ʵ�ֹ��ε�Ч�������ڱ�������ɫ�ϣ�ֱ�Ӽ��ϵ�ǰ����ɫ
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
    // �������û�Ϻ���ΪĬ��
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    // ����������Χ���ı��Σ�ǰ���������꣬����������������
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

// ֱ�ӴӼ�¼�ϴη��ص�ȫ�ֱ�����ʼ˳��������Ҳ����ٴ�ͷ��
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
    // ����û�й��ڵ�����
    lastUsedParticle = 0;
    return 0;
}

// �����趨���Ӷ����״̬
void ParticleGenerator::respawnParticle(Particle &particle, BallObject &ball, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f;       // -5~5
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = ball.Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    /* 
    ע�⵽������Ч����β��������֡�ʷǳ��ߣ����Ժܿ�ͻᱻ��Ⱦ��500��������
    ��Ⱦ��֮����û�����ӹ��ڣ��ͻ���һ��ʱ���ǿհ׵ģ���������µ���β
    ������Ҫһֱ������β��Ҫô����������Ⱦ��������Ҫô�������ӵ���������
    */
    particle.Life = 0.5f;
    particle.Velocity = ball.Velocity * 0.1f;
}