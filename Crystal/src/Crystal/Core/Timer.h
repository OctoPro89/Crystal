#pragma once

#include <chrono>

namespace Crystal {

	class Timer /* timer class */
	{
	public:
		Timer() /* timer constructor that resets the newly made timer */
		{
			Reset(); /* reset it */
		}

		void Timer::Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now(); /* create the start clock */
		}

		float Timer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f; /* get the elapsed time */
		}

		float Timer::ElapsedMillis() 
		{
			return Elapsed() * 1000.0f; /* gets the elapsed time in milliseconds */
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start; /* the start clock */
	};

}
