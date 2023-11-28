#include "crystalpch.h"
#include <random>
#include <unordered_map>
#include "UUID.h"

namespace Crystal
{
	static std::random_device s_RandomDevice;								 /* a static random device */
	static std::mt19937_64  s_Engine(s_RandomDevice());						 /* a static mt19937_64 random generator */
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;	 /* a static uniform_int_distrubution for the final random stage*/

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine)) /* a constructor that sets m_UUID to the randomly generated number */
	{

	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid) /* a different constructor that sets m_UUID to the given uuid */
	{
	}
}
