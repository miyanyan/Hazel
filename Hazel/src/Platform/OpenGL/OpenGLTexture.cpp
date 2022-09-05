#include "OpenGLTexture.h"

#include "glad/glad.h"

#include "Hazel/Utils/stb_image_impl.h"
#include "Hazel/Core/Log.h"

namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(const char* path)
		: m_path(path)
		, m_width(0)
		, m_height(0)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_textureId);
		stbi_set_flip_vertically_on_load(1);

		int channels;
		auto data = stbi_load(path, &m_width, &m_height, &channels, 0);

		if (!data) {
			HZ_CORE_ERROR("failed to load image({})!", path);
		}
		else {
			// format
			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4) {
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3) {
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			if (internalFormat != 0 && dataFormat != 0) {
				glTextureStorage2D(m_textureId, 1, internalFormat, m_width, m_height);
				glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureSubImage2D(m_textureId, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);
			}
			else {
				HZ_CORE_ERROR("not supported format, image({})!", path);
			}
		}

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_textureId);
	}

	int OpenGLTexture2D::getWidth() const
	{
		return m_width;
	}

	int OpenGLTexture2D::getHeight() const
	{
		return m_height;
	}

	void OpenGLTexture2D::bind(int num) const
	{
		glBindTextureUnit(num, m_textureId);
	}
}