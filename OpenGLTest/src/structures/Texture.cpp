#include "stdafx.h"
#include "Texture.h"

#include "stb/stb_image.h"

Texture::Texture(std::string path) : m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0) {
	stbi_set_flip_vertically_on_load(1); //flips the texture vertically, opengl wants pixels to start at the bottom left
	
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); //4 channels for RGBA

	glGenTextures(1, &m_RendererID);
	Bind();

	//MUST be defined everytime, for every texture. Ofc you can change what you want but these must be set
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //filter for minituarization (when texture too big for the thing its drawn on)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //filter for magnificaion (when texture too small for the thing its drawn on)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); //dont tile in the X direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); //dont tile in the Y direction
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	
	Unbind();

	if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() {
	//if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const {
	//GL_TEXTURE0 goes up to GL_TEXTURE31 but not all platforms support all the way up to 32 slots (so query it)
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}
