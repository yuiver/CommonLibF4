#pragma once

namespace RE
{
	enum class POWER_ARMOR_HUD_VISIBILITY_RULE;

	class HUDModeType;

	class HUDModes
	{
	public:
		// members
		BSTArray<HUDModeType> validHUDModes;                                                           // 00
		stl::enumeration<POWER_ARMOR_HUD_VISIBILITY_RULE, std::uint32_t> powerArmorHUDVisibilityRule;  // 18
		bool canBeVisible;                                                                             // 1C
		std::uint8_t pad1D{ 0 };                                                                       // 1D
		std::uint16_t pad1E{ 0 };                                                                      // 1E
	};
	static_assert(sizeof(HUDModes) == 0x20);
}
