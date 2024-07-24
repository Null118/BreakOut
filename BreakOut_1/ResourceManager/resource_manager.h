#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "OpenGL_Init/texture.h"
#include "OpenGL_Init/shader.h"



// ��Դ�������࣬���õ������ģʽʵ�֣������� ȫ����̬�ĳ�Ա���� �� ˽�л��Ĺ��캯��
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    // �� .vs .fs .gs ���ļ��ж�ȡ��ɫ�����봴��һ����ɫ������������ɫ������
    // ע�⵽���Ķ�����һ�� map������ζ�����ǿ���ͨ������������ɫ������������ ResourceManager::GetShader("yourShader").Use();
    // ��ȻҲ����ֱ��ʹ�÷��صĶ���������ķ������� shader.Use();
    static Shader     LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    // ͨ�������ҵ���ʼ��������ɫ������
    static Shader&    GetShader(std::string name);
    // ͬ�����غͻ�ȡ����
    static Texture2D  LoadTexture(const char *file, bool alpha, std::string name);
    static Texture2D& GetTexture(std::string name);
    // �ͷ����м��ص���Դ��Ϊʲô��д���������������Ϊ����ģʽ������ô����
    static void      Clear();
private:
    // ���ǲ���Ҫ��������Դ������ʵ����ֻ��Ҫ���Ǿ�̬�ҹ����ĳ�Ա�����ͱ���
    ResourceManager() { }
    // ������ɫ����ʵ��ʵ��
    static Shader    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    // ���������ʵ��ʵ��
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif