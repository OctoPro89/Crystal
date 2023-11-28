#pragma once

namespace Crystal
{
	class Timestep /* timestep class*/
	{
	public:
		Timestep(float time)
			: m_Time(time) /* contructor that sets m_Time to the given time */
		{

		}

		operator float() const { return m_Time; }					/* overrides the float operator to return a const to m_Time */

		float GetSeconds() const { return m_Time; }					/* a function to get the seconds that returns a float */
		float GetMilliseconds() const { return m_Time * 1000.0f; }	/* a function to get the milliseconds that returns a float */
	private:
		float m_Time; /* a floating point number for tracking the time */
	};
}
