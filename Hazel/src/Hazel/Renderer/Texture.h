#pragma once

#include <memory>

namespace Hazel {
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;

		virtual void setData(void* data, uint32_t size) = 0;

		virtual void bind(int num = 0) const = 0;

		virtual uint32_t getRendererId() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static std::shared_ptr<Texture2D> create(const char* path);
		static std::shared_ptr<Texture2D> create(uint32_t width, uint32_t height);
	};
}