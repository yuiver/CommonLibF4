#pragma once

namespace RE
{
	class NiFrustum
	{
	public:
		float left;                // 00
		float right;               // 04
		float top;                 // 08
		float bottom;              // 0C
		float near;                // 10
		float far;                 // 14
		bool ortho;                // 18
		std::uint8_t pad19{ 0 };   // 19
		std::uint16_t pad1A{ 0 };  // 1A
	};
	static_assert(sizeof(NiFrustum) == 0x1C);
}