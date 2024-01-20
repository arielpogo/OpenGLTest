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
        IndexBuffer* indexBuffer;
        unsigned int vertexArray;

        float vertices[180] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
        };

        //const unsigned int indices[6*2] = { //the indexes of the vertices to draw the triangles
        //    0, 1, 2,
        //    2, 3, 0,

        //    1, 5, 4,
        //    4, 2, 1
        //};

        Renderer renderer;
        
        Texture texture = Texture("res/textures/mason.png");
        Shader shader = Shader("res/shaders/basicvertex.shader", "res/shaders/basicfragment.shader");

    public:
	    Test_3D() {
            
            glGenVertexArrays(1, &vertexArray); //create 1 VAO
            glBindVertexArray(vertexArray);

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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glBindVertexArray(0);

            texture.Bind();
            vertexBuffer->Unbind();
            indexBuffer->Unbind();
            shader.Unbind();

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
	    }

	    ~Test_3D() {
            delete vertexBuffer;
            delete indexBuffer;
        }

	    void OnUpdate(float deltaTime) override {};

	    void OnRender() override {
            glClearColor(0.86f, 1.0f, 1.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            texture.Bind();
            camera.Update();
            shader.Bind();
            shader.SetUniformMatrix4f("u_MVP", camera.MVP);
            renderer.Draw(*indexBuffer, vertexArray, shader);
	    }

	    void OnImGuiRender()  override {}
	};
}