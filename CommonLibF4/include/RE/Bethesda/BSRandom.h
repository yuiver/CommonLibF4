#pragma once

namespace RE
{
	namespace BSRandom
	{
		inline std::uint32_t UnsignedInt(unsigned int aMin, unsigned int aMax)
		{
			using func_t = decltype(&BSRandom::UnsignedInt);
			static REL::Relocation<func_t> func{ REL::ID(2267950) };
			return (aMin, aMax);
		}
	}
}
