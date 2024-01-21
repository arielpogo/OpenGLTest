#pragma once

#include "Scene.h"
#include "stdafx.h"
#include "imgui/imgui.h"
#include "Core.h"
#include "Camera.h"

namespace Scenes {
	class Test_3D : public Scene {
    private:
        VertexBuffer* vertexBuffer; //automatically binds
        //IndexBuffer* indexBuffer;
        unsigned int cubeVertexArray; //regular cubes

        unsigned int lightVertexArray;

        float vertices[8*6*6] = {
            // positions // normals // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };

        std::vector<glm::vec3> cubePositions{
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
        };

        //const unsigned int indices[6*2] = { //the indexes of the vertices to draw the triangles
        //    0, 1, 2,
        //    2, 3, 0,

        //    1, 5, 4,
        //    4, 2, 1
        //};
   
        Texture texture = Texture("res/textures/texture.png");
        Shader texturedShader = Shader("res/shaders/textured_vertex.shader", "res/shaders/textured_fragment.shader");
        Shader litShader = Shader("res/shaders/lit_vertex.shader", "res/shaders/lit_fragment.shader");
        Shader emissiveSurfaceShader = Shader("res/shaders/emissive_surface_vertex.shader", "res/shaders/emissive_surface_fragment.shader");

        glm::vec3 lightPos = glm::vec3(3.2f, 1.0f, 2.0f);

    public:
	    Test_3D() {
            glGenVertexArrays(1, &cubeVertexArray); //create 1 VAO
            glBindVertexArray(cubeVertexArray);

            vertexBuffer = new VertexBuffer(vertices, sizeof(vertices));
            //indexBuffer = new IndexBuffer(indices, 6*2);

            vertexBuffer->Bind();
            //indexBuffer->Bind();

            //glEnableVertexAttribArray(0); //default VAO on compat profile. Core requires specifically creating it

            //specify to opengl how the data in the bound buffer is arranged
            //Specifically, the memory can be arranged into struct-like sections
            // (or also literally into structs), so we need to explain where everything goes
            //index: first attribute (0)
            //size: components per vertex, we're doing 2D pos so 2
            //type: self-explanitory
            //normalized: self-explanitory
            //stride: bytes between each index
            //pointer: offset of the first instance of this attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);

            texture.Bind();
            vertexBuffer->Unbind();
            //indexBuffer->Unbind();
            texturedShader.Unbind();

            //LIGHT CUBES
            glGenVertexArrays(1, &lightVertexArray);
            glBindVertexArray(lightVertexArray);
            vertexBuffer->Bind();

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);

            litShader.Bind();
            litShader.SetUniformVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.5f));
            litShader.SetUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            litShader.SetUniformVec3("lightPos", lightPos);

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
	    }

	    ~Test_3D() {
            delete vertexBuffer;
            //delete indexBuffer;
        }

	    void OnUpdate(float deltaTime) override {};

	    void OnRender() override {
            glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            texture.Bind();
            //camera.Update();
            litShader.Bind();

            //draw the various cubes
            for (int i = 0; i < cubePositions.size(); i++) {
                camera.modelMatrix = glm::mat4(1.0f);
                camera.modelMatrix = glm::translate(camera.modelMatrix, cubePositions[i]);
                camera.Update();
                litShader.SetUniformMatrix4f("u_MVP", camera.MVP);
                glm::mat3 normalMatrix = glm::mat3(camera.modelMatrix);
                normalMatrix = glm::transpose(glm::inverse(normalMatrix));
                litShader.SetUniformMatrix3f("normalMatrix", normalMatrix);
                litShader.SetUniformMatrix4f("modelMatrix", camera.modelMatrix);
                litShader.SetUniformVec3("viewPos", camera.cameraPos);

                glBindVertexArray(cubeVertexArray);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
            texture.Unbind(); //not strictly necesarry, but why not

            //draw the light source
            emissiveSurfaceShader.Bind();
            camera.modelMatrix = glm::mat4(1.0f);
            camera.modelMatrix = glm::translate(camera.modelMatrix, lightPos);
            camera.modelMatrix = glm::scale(camera.modelMatrix, glm::vec3(0.2f));
            camera.Update();
            emissiveSurfaceShader.SetUniformMatrix4f("u_MVP", camera.MVP);

            glBindVertexArray(lightVertexArray);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(0);
        }

	    void OnImGuiRender()  override {}
	};
}