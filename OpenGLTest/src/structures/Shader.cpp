#include "stdafx.h"
#include "Core.h"
#include "Shader.h"

//types of shaders (programs on the GPU):
//vertex, fragment, <-- (two most common) tesselation, geometry, compute

//Draw call -> vertex shader called, then fragment shader, then image (many steps between, before, after)
//in our case,we have 3 vertexes, so the vertex shader will be called 3 times
//the primary point of a vertex shader is to determine where a vertex ought to
// be in your screenspace

//you can pass data from the vertex shader to the fragment shader

//fragment shader runs once for *every* pixel rasterized, so consider optimizations
//where repeat calculations occur in the the vertex shader (way fewer) than the fragment shader

/// <summary>
/// Helper for CreateShader(), gets the length (in bytes) of a shader file.
/// </summary>
/// <returns>The length, or -1 on error.</returns>
static long GetShaderLength(FILE*& shaderFile) {
    errno = 0;

    fseek(shaderFile, 0L, SEEK_END);
    if (ferror(shaderFile)) {
        std::cerr << "[ERROR] ferror while seeking to the end of a shader file." << std::endl;
        return -1;
    }

    long len = ftell(shaderFile);
    if (errno) {
        char* error_string = (char*)_malloca((256 + 1) * sizeof(char));

        if (error_string == nullptr) std::cerr << "[ERROR] ftell failed on shader file AND error string failed to _malloca." << std::endl;
        else std::cerr << strerror_s(error_string, 256 + 1, errno) << std::endl;

        errno = 0;
        return -1;
    }

    fseek(shaderFile, 0L, SEEK_SET);
    if (ferror(shaderFile)) {
        std::cerr << "[ERROR] ferror while seeking to the start of a shader file." << std::endl;
        return -1;
    }

    return len;
}

/// <summary>
/// Helper for CreateShader(), reads the file into the code buffer.
/// </summary>
/// <returns>If succeeded.</returns>
static bool ReadShader(char*& shaderCode, long& shaderLength, FILE*& shaderFile) {
    fread(shaderCode, sizeof(char), shaderLength-1, shaderFile);

    bool success = true;

    if (feof(shaderFile)) {
        std::cerr << "[ERROR] feof set while using fread on shader file." << std::endl;
        success = false;
    }
    else if (ferror(shaderFile)) {
        std::cerr << "[ERROR] Error occured while reading shader file (ferror)." << std::endl;
        success = false;
    }

    return success;
}

/// <summary>
/// Helper for CreateShader(), compiles a single shader.
/// </summary>
/// <param name="type">The shader type (GL_..._SHADER)</param>
/// <param name="src">The code buffer.</param>
/// <returns>The shader id, or 0 on error.</returns>
static unsigned int CompileShader(unsigned int type, char*& src) {
    unsigned int id = glCreateShader(type); //creates an empty shader obj of specified type, returns its id

    //Replaces the source code in a shader object
    //shader: the shader (its id)
    //count: count in the string, length arrays
    //string: array of strings. since we have 1, we have a pointer to the string's pointer
    //length: array of lengths of the strings. nullptr -> null-terminated (per the docs)
    glShaderSource(id, 1, &src, nullptr);

    //Compiles the given code in a shader object
    glCompileShader(id);

    int result;

    //gets parameter from shader object
    //i -> int
    //v -> vector/array. This is one int so pointer to the int
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (!result) {
        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = new char[len];

        //gets the shader log
        //shader: self-explanitory
        //maxLength: length of the buffer, we made it len
        //length: length of the message, we stored that in len
        //infoLog: the array
        glGetShaderInfoLog(id, len, &len, message);
        std::cerr << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader") << std::endl;
        std::cerr << message << std::endl;

        glDeleteShader(id);
        delete[] message;

        return 0;
    }

    return id;
}

#define CLOSE_FILES_RETURN_ZERO fclose(vertexFile);\
                                fclose(fragmentFile);\
                                return 0;

#define CLOSE_FILES_FREE_RETURN_ZERO fclose(vertexFile);\
                                     fclose(fragmentFile);\
                                     free(vertexShaderCode);\
                                     free(fragmentShaderCode);\
                                     return 0;
/// <summary>
/// Convenient function for opening shader file, reading it, compiling, linking and creating/returning a shader/program, complete with error handling.
/// </summary>
/// <returns>Shader/program id, otherwise 0 on error.</returns>
static unsigned int CreateShader(std::string vertexPath, std::string fragmentPath) {
    FILE* vertexFile;
    FILE* fragmentFile;
    fopen_s(&vertexFile, vertexPath.c_str(), "rb");
    fopen_s(&fragmentFile, fragmentPath.c_str(), "rb");

    if (vertexFile == nullptr || fragmentFile == nullptr) {
        std::stringstream ss;

        ss << "[ERROR] Could not open the following shader files: ";

        if (vertexFile == nullptr) ss << "\n" << vertexPath;
        else fclose(vertexFile);

        if (fragmentFile == nullptr) ss << "\n" << fragmentPath;
        else fclose(fragmentFile);

        std::cerr << ss.str() << std::endl;

        return 0;
    }

    long vertexLen = GetShaderLength(vertexFile);
    long fragmentLen = GetShaderLength(fragmentFile);

    if (vertexLen == -1 || fragmentLen == -1) { CLOSE_FILES_RETURN_ZERO }

    char* vertexShaderCode = (char*)malloc(vertexLen); //for some reason, it allocates 4 more bytes than text in the file, we replace it with \0
    if (vertexShaderCode == nullptr) { CLOSE_FILES_RETURN_ZERO }

    char* fragmentShaderCode = (char*)malloc(fragmentLen);

    if (fragmentShaderCode == nullptr) {
        fclose(vertexFile);
        fclose(fragmentFile);
        free(vertexShaderCode);
        return 0;
    }

    memset(vertexShaderCode, '\0', vertexLen);
    memset(fragmentShaderCode, '\0', fragmentLen);

    if (!ReadShader(vertexShaderCode, vertexLen, vertexFile)) { CLOSE_FILES_FREE_RETURN_ZERO }
    if (!ReadShader(fragmentShaderCode, fragmentLen, fragmentFile)) { CLOSE_FILES_FREE_RETURN_ZERO }

    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderCode);
    if (vs == 0) { CLOSE_FILES_FREE_RETURN_ZERO }

    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    if (fs == 0) { CLOSE_FILES_FREE_RETURN_ZERO }

    //specify what is to be linked
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    //link, create executables
    glLinkProgram(program);
    glValidateProgram(program);

    //delete the intermediates
    glDeleteShader(vs);
    glDeleteShader(fs);

    fclose(vertexFile);
    fclose(fragmentFile);
    free(vertexShaderCode);
    free(fragmentShaderCode);

    return program;
}

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
    m_RendererID = CreateShader(vertexPath, fragmentPath);
    ASSERT(m_RendererID != 0);

    Bind();
    //TODO: handle exception handling better
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1i(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]); //no need to transpose, glm does column major
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& v){
    glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
}

int Shader::GetUniformLocation(const std::string& name)
{
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) std::cerr << "[Warning] Uniform " << name << " doesn't exist " << std::endl;
    return location;
}
