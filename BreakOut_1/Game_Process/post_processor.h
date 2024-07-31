#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "OpenGL_Init/texture.h"
#include "OpenGL_Init/sprite_renderer.h"
#include "OpenGL_Init/shader.h"


/*
    绑定至多重采样的帧缓冲
    和往常一样渲染游戏
    将多重采样的帧缓冲内容传输至一个普通的帧缓冲中（这个帧缓冲使用了一个纹理作为其颜色缓冲附件）
    解除绑定（绑定回默认的帧缓冲）
    在后期处理着色器中使用来自普通帧缓冲的颜色缓冲纹理
    渲染屏幕大小的四边形作为后期处理着色器的输出
    ——————————————————————
    所以 vs, fs 中控制的纹理坐标，实际是全部场景，只是附在了帧缓冲的纹理附件上方便操作
*/
class PostProcessor
{
public:

    Shader PostProcessingShader;
    Texture2D Texture;
    unsigned int Width, Height;
    // 特效开关
    bool Confuse, Chaos, Shake;

    PostProcessor(Shader shader, unsigned int width, unsigned int height);
    // 帧渲染的开关，用于区别
    void BeginRender();
    void EndRender();
  
    void Render(float time);
private:
    unsigned int MSFBO, FBO; // 两个帧缓冲对象，MS means Multi-Sample
    unsigned int RBO; // RBO means Render Buffer Object, 即附在帧缓冲上的对象
    unsigned int VAO;

    void initRenderData();
};

#endif