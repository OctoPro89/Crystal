#pragma once

#include <xhash>

namespace Crystal
{
	class UUID /* uuid class */
	{
	public:
		// UUID id
		UUID();											/* uuid normal constructor */
		UUID(uint64_t uuid);							/* uuid constructor that takes in an already existing uuid */
		UUID(const UUID&) = default;					/* uuid copy constructor */

		operator uint64_t() const { return m_UUID; }	/* override the uint64_t operations to give the operation the uint64_t id instead for easyness */
	private:
		uint64_t m_UUID;								/* uuid */
	};
}

namespace std {
	template<>
	struct hash<Crystal::UUID> /* templated hash function for filling in the std::hash for UUIds */
	{
		std::size_t operator()(const Crystal::UUID& uuid) const  /* override the size_t operator and return a hashed uuid */
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
