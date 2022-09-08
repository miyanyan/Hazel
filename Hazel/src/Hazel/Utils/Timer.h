#pragma once

#include <chrono>

namespace Hazel {
	// measure execution time of a function
	class Timer
	{
	public:
		Timer() : m_begin(std::chrono::steady_clock::now()) {}

		inline void reset() {
			m_begin = std::chrono::steady_clock::now();
		}

		template<typename Duration = std::chrono::milliseconds>
		inline double elapsed() {
			return std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - m_begin).count();
		}
	private:
		std::chrono::steady_clock::time_point m_begin;
	};

}