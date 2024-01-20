#pragma once

class VertexBuffer {
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID; //ID given from the renderer api
};