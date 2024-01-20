#pragma once

#include "stdafx.h"

static long GetShaderLength(FILE*& shaderFile);

static bool ReadShader(char*& shaderCode, long& shaderLength, FILE*& shaderFile);

static unsigned int CompileShader(unsigned int type, char*& src);

static unsigned int CreateShader(std::string vertexPath, std::string fragmentPath);

class Shader {
public:
    Shader(std::string vertexPath, std::string fragmentPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //Set Uniforms
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v4);
    void SetUniform1i(const std::string& name, int value);
    void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);

private:
    unsigned int m_RendererID;
    int GetUniformLocation(const std::string& name);
};