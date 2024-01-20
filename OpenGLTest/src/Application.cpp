#include "stdafx.h"
#include "Core.h"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Scene.h"
#include "DefaultScene.h"
#include "Test_ClearColor.h"
#include "Test_3D.h"

#include "Camera.h"

Scenes::Scene* scene = new Scenes::Test_3D();

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //whenever the window is resized, resize the viewport
    globalWidth = width;
    globalHeight = height;
}

//called on debug message
void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    if (id == 131185) return;

    std::cerr << "[OpenGL] ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               std::cerr << "ERROR"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "DEPRECATED_BEHAVIOR";break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cerr << "UNDEFINED_BEHAVIOR";break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cerr << "PORTABILITY"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cerr << "PERFORMANCE"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cerr << "OTHER MESSAGE"; break;
    }

    std::cerr << " id: " << id;
    std::cerr << " severity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_LOW:          std::cerr << "LOW"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "MEDIUM"; break;
        case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "HIGH"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "NOTIFICATION"; break;
    }
    std::cerr << std::endl;
    std::cerr << message << "\n" << std::endl;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 0.20f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cameraSpeed += 0.30f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraSpeed /= 10.0f;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        scene->camera.cameraPos += cameraSpeed * scene->camera.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        scene->camera.cameraPos -= cameraSpeed * scene->camera.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        scene->camera.cameraPos -= glm::normalize(glm::cross(scene->camera.cameraFront, scene->camera.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        scene->camera.cameraPos += glm::normalize(glm::cross(scene->camera.cameraFront, scene->camera.cameraUp)) * cameraSpeed;
}

const float sensitivity = 0.05f;
bool firstMouseInput = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouseInput) {
        lastx = xpos;
        lasty = ypos;
        firstMouseInput = false;
    }

    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos; //reversed since y ranges from bottom to top
    lastx = xpos;
    lasty = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    scene->camera.yaw += xoffset;
    scene->camera.pitch += yoffset;

    if (scene->camera.pitch > 89.0f) scene->camera.pitch = 89.0f;
    else if (scene->camera.pitch < -89.0f) scene->camera.pitch = -89.0f;

    scene->camera.cameraDirection.x = cos(glm::radians(scene->camera.yaw)) * cos(glm::radians(scene->camera.pitch));
    scene->camera.cameraDirection.y = sin(glm::radians(scene->camera.pitch));
    scene->camera.cameraDirection.z = sin(glm::radians(scene->camera.yaw)) * cos(glm::radians(scene->camera.pitch));
    scene->camera.cameraFront = glm::normalize(scene->camera.cameraDirection);
}

int main(void) {
    //****************************//
    //                            //
    //         GLFW Setup         //
    //                            //
    //****************************//

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cerr << "[ERROR] glfw could not initialize." << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1); //tell glfw to tell opengl to create a debug context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(globalWidth, globalHeight, "OpenGLTest", NULL, NULL);
    if (!window) {
        std::cerr << "[ERROR] glfw could not create a window." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    //glew setup
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error! GLEW could not initialize." << std::endl;
        glfwTerminate();
        return -1;
    }

    //****************************//
    //                            //
    //        OpenGL Setup        //
    //                            //
    //****************************//

    glfwSwapInterval(1); //enable vsync

    glEnable(GL_DEBUG_OUTPUT); //enable debug output
    glDebugMessageCallback(DebugMessageCallback, 0);

    glViewport(0, 0, 1280, 720);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback); //whenever the screen is resized, call this function
    
    glEnable(GL_DEPTH_TEST); //enable the z buffer
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //source factor is what we do with the source (new data from fragment shader) and the destination (existing data)

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //FPS camera input
    glfwSetCursorPosCallback(window, mouse_callback);

    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Reminder: Debug message 131185 is being ignored." << std::endl;
    
    //****************************//
    //                            //
    //         Dear ImGUI         //
    //                            //
    //****************************//

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsLight();

    //****************************//
    //                            //
    //        Render Scope        //
    //                            //
    //****************************//

    {
       int selection = 0;
       const char* tests[] = { "Default", "Background Picker", "3D Test" };
       
        // Loop until the user closes the window 
        while (!glfwWindowShouldClose(window)) {
            processInput(window);


            scene->OnUpdate(0.0f);
            scene->OnRender();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Scene Selection");
            ImGui::ListBox("Scene", &selection, tests, IM_ARRAYSIZE(tests), 10);

            if (ImGui::Button("Load")) {
                switch (selection) {
                    delete scene;
                case 0:
                    scene = new Scenes::Test_Default();
                    break;
                case 1:
                    scene = new Scenes::Test_ClearColor();
                    break;
                case 2:
                    scene = new Scenes::Test_3D();
                    break;
                default:
                    scene = new Scenes::Test_Default();
                    break;
                }
            }

            ImGui::End();

            scene->OnImGuiRender();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete scene;
    } //end renderer scope, call destructors

    //****************************//
    //                            //
    //          Cleanup           //
    //                            //
    //****************************//

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}