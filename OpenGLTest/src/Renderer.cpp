#include "stdafx.h"
#include "Renderer.h"

//todo: Add vertex array input?
void Renderer::Draw(const IndexBuffer& ib, const unsigned int vao, const Shader& shader) const {
    glBindVertexArray(vao);
    //ib.Bind();
    //shader.Unbind();
    shader.Bind();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}
