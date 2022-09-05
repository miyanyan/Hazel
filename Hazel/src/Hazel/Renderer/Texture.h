#pragma once

namespace Hazel {
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;

		virtual void bind(int num = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Texture2D* create(const char* path);
	};
}