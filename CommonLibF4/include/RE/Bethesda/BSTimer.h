#pragma once

namespace RE
{
	class BSTimer
	{
	public:
		[[deprecated]] [[nodiscard]] static BSTimer GetSingleton()
		{
			REL::Relocation<BSTimer*> singleton{ REL::ID(1256126) };
			return *singleton;
		}

		[[deprecated]] static float QGlobalTimeMultiplier()
		{
			REL::Relocation<float*> value{ REL::ID(365546) };
			return *value;
		}

		[[deprecated]] static float QGlobalTimeMultiplierTarget()
		{
			REL::Relocation<float*> value{ REL::ID(1266509) };
			return *value;
		}

		[[deprecated]] void SetGlobalTimeMultiplier(float a_mult, bool a_now)
		{
			using func_t = decltype(&BSTimer::SetGlobalTimeMultiplier);
			REL::Relocation<func_t> func{ REL::ID(1419977) };
			return func(this, a_mult, a_now);
		}

		// members
		std::int64_t highPrecisionInitTime;  // 00
		float clamp;                         // 08
		float clampRemainder;                // 0C
		float delta;                         // 10
		float realTimeDelta;                 // 14
		std::uint64_t lastTime;              // 18
		std::uint64_t firstTime;             // 20
		std::uint64_t disabledLastTime;      // 28
		std::uint64_t disabledFirstTime;     // 30
		std::uint32_t disableCounter;        // 38
		bool useGlobalTimeMultiplierTarget;  // 3C
	};
	static_assert(sizeof(BSTimer) == 0x40);
}
