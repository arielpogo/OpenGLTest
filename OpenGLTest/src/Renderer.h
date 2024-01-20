#pragma once

#include "structures/VertexArray.h"
#include "structures/IndexBuffer.h"
#include "structures/Shader.h"

class Renderer {
public:
	void Draw(const IndexBuffer& ib, const unsigned int vao, const Shader& shader) const;
};

