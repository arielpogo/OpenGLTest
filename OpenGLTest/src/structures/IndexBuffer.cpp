#include "stdafx.h"
#include "IndexBuffer.h"

///<summary>
/// Creates + binds an index buffer.
/// </summary>
/// <param name="data">Internal data array.</param>
/// <param name="size">Number of indices.</param>
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) {
    m_Count = count;

    glGenBuffers(1, &m_RendererID); //create 1 buffer

    Bind();
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unselect the buffer
}