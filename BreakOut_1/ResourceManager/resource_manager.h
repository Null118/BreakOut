#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "OpenGL_Init/texture.h"
#include "OpenGL_Init/shader.h"



// 资源管理器类，采用单例设计模式实现，包含了 全部静态的成员函数 和 私有化的构造函数
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    // 从 .vs .fs .gs 的文件中读取着色器代码创建一个着色器，并返回着色器对象
    // 注意到上文定义了一个 map，这意味着我们可以通过别名调用着色器方法，例如 ResourceManager::GetShader("yourShader").Use();
    // 当然也可以直接使用返回的对象调用它的方法，如 shader.Use();
    static Shader     LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    // 通过别名找到初始化过的着色器对象
    static Shader&    GetShader(std::string name);
    // 同理，加载和获取纹理
    static Texture2D  LoadTexture(const char *file, bool alpha, std::string name);
    static Texture2D& GetTexture(std::string name);
    // 释放所有加载的资源（为什么不写在析构函数里？是因为单例模式就这样么？）
    static void      Clear();
private:
    // 我们不需要真正的资源管理器实例，只需要他们静态且公开的成员函数和变量
    ResourceManager() { }
    // 加载着色器的实际实现
    static Shader    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    // 加载纹理的实际实现
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif