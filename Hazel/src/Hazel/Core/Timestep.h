#pragma once

namespace Hazel {

	class TimeStep
	{
	public:
		TimeStep(float time = 0.0f) : m_time(time) {}

		operator float() const { return m_time; };

		float getSeconds() const { return m_time; }
		float getMilliseconds() const{ return m_time / 1000.0f; }

	private:
		float m_time;
	};

}