#include "stdafx.h"
#include "Renderer.h"

//todo: Add vertex array input?
void Renderer::Draw(const IndexBuffer& ib, const unsigned int vao, const Shader& shader) const {
    glBindVertexArray(vao);
    //ib.Bind();
    //shader.Unbind();
    shader.Bind();

    // Render here
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    ib.Unbind();
    shader.Unbind();
}
