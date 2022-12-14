#include "OpenGLTexture.h"

#include "Hazel/Utils/stb_image_impl.h"
#include "Hazel/Utils/CheckOpenGL.h"
#include "Hazel/Core/Log.h"

namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(const char* path)
		: m_path(path)
		, m_width(0)
		, m_height(0)
		, m_internalFormat(0)
		, m_dataFormat(0)
	{
		CHECK_GL(glCreateTextures(GL_TEXTURE_2D, 1, &m_textureId));
		stbi_set_flip_vertically_on_load(1);

		int channels;
		auto data = stbi_load(path, &m_width, &m_height, &channels, 0);

		if (!data) {
			HZ_CORE_ERROR("failed to load image({})!", path);
		}
		else {
			// format
			if (channels == 4) {
				m_internalFormat = GL_RGBA8;
				m_dataFormat = GL_RGBA;
			}
			else if (channels == 3) {
				m_internalFormat = GL_RGB8;
				m_dataFormat = GL_RGB;
			}

			if (m_internalFormat != 0 && m_dataFormat != 0) {
				CHECK_GL(glTextureStorage2D(m_textureId, 1, m_internalFormat, m_width, m_height));
				CHECK_GL(glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				CHECK_GL(glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
				CHECK_GL(glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_S, GL_REPEAT));
				CHECK_GL(glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_T, GL_REPEAT));
				CHECK_GL(glTextureSubImage2D(m_textureId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data));
			}
			else {
				HZ_CORE_ERROR("not supported format, image({})!", path);
			}
		}

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_path("")
		, m_width(width)
		, m_height(height)
		, m_internalFormat(GL_RGBA8)
		, m_dataFormat(GL_RGBA)
	{
		CHECK_GL(glCreateTextures(GL_TEXTURE_2D, 1, &m_textureId));
		CHECK_GL(glTextureStorage2D(m_textureId, 1, m_internalFormat, m_width, m_height));
		CHECK_GL(glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		CHECK_GL(glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		CHECK_GL(glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_S, GL_REPEAT));
		CHECK_GL(glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		CHECK_GL(glDeleteTextures(1, &m_textureId));
	}

	int OpenGLTexture2D::getWidth() const
	{
		return m_width;
	}

	int OpenGLTexture2D::getHeight() const
	{
		return m_height;
	}

	void OpenGLTexture2D::setData(void* data, uint32_t size)
	{
		uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
		if (bpp * m_width * m_height == size) {
			CHECK_GL(glTextureSubImage2D(m_textureId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data));
		}
		else {
			HZ_CORE_ERROR("Data must be entire texture!");
		}
	}

	void OpenGLTexture2D::bind(int num) const
	{
		CHECK_GL(glBindTextureUnit(num, m_textureId));
	}

	uint32_t OpenGLTexture2D::getRendererId() const
	{
		return m_textureId;
	}

	bool OpenGLTexture2D::operator==(const Texture& other) const
	{
		return m_textureId == static_cast<const OpenGLTexture2D&>(other).m_textureId;
	}
}