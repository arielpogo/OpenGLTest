#include "stdafx.h"
#include "VertexBuffer.h"

///<summary>
/// Creates + binds a vertex buffer.
/// </summary>
/// <param name="data">Internal data array.</param>
/// <param name="size">Size in bytes (use sizeof(type) * quantity).</param>
VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &m_RendererID); //create 1 buffer

    Bind();

	//tell the buffer its size and its data
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); //select the buffer
}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unselect the buffer
}