#pragma once

#include <string>

#include <glad/glad.h>

#include "Hazel/Renderer/Texture.h"

namespace Hazel {
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const char* path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D() override;

		virtual int getWidth() const override;
		virtual int getHeight() const override;

		virtual void setData(void* data, uint32_t size) override;

		virtual void bind(int num) const override;

	private:
		std::string m_path;
		int m_width, m_height;
		uint32_t m_textureId;
		GLenum m_internalFormat, m_dataFormat;
	};
}