#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "OpenGL_Init/texture.h"
#include "OpenGL_Init/sprite_renderer.h"
#include "OpenGL_Init/shader.h"


/*
    �������ز�����֡����
    ������һ����Ⱦ��Ϸ
    �����ز�����֡�������ݴ�����һ����ͨ��֡�����У����֡����ʹ����һ��������Ϊ����ɫ���帽����
    ����󶨣��󶨻�Ĭ�ϵ�֡���壩
    �ں��ڴ�����ɫ����ʹ��������ͨ֡�������ɫ��������
    ��Ⱦ��Ļ��С���ı�����Ϊ���ڴ�����ɫ�������
    ��������������������������������������������
    ���� vs, fs �п��Ƶ��������꣬ʵ����ȫ��������ֻ�Ǹ�����֡������������Ϸ������
*/
class PostProcessor
{
public:

    Shader PostProcessingShader;
    Texture2D Texture;
    unsigned int Width, Height;
    // ��Ч����
    bool Confuse, Chaos, Shake;

    PostProcessor(Shader shader, unsigned int width, unsigned int height);
    // ֡��Ⱦ�Ŀ��أ���������
    void BeginRender();
    void EndRender();
  
    void Render(float time);
private:
    unsigned int MSFBO, FBO; // ����֡�������MS means Multi-Sample
    unsigned int RBO; // RBO means Render Buffer Object, ������֡�����ϵĶ���
    unsigned int VAO;

    void initRenderData();
};

#endif