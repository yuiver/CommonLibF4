#pragma once

#include "RE/Bethesda/ImageSpaceData.h"
#include "RE/NetImmerse/NiColor.h"
#include "RE/NetImmerse/NiRect.h"
#include "RE/NetImmerse/NiSmartPointer.h"

namespace RE
{
	class BSTriShape;
	class ImageSpaceEffect;

	class ImageSpaceManager
	{
	public:
		[[nodiscard]] static ImageSpaceManager* GetSingleton()
		{
			REL::Relocation<ImageSpaceManager**> singleton{ REL::ID(161743) };
			return *singleton;
		}

		// members
		NiRect<std::int32_t> scissorRect;                 // 000
		NiTPrimitiveArray<ImageSpaceEffect*> effectList;  // 010
		NiPointer<BSTriShape> screenTriShape;             // 028
		NiPointer<BSTriShape> screenTriShapeColors;       // 030
		NiPointer<BSTriShape> dynamicScreenTriShape;      // 038
		NiPointer<BSTriShape> partialScreenTriShape;      // 040
		bool enablePartialRender;                         // 048
		std::int32_t mainTarget;                          // 04C
		NiColorA refractionTint;                          // 050
		const ImageSpaceBaseData* paseData;               // 060
		const ImageSpaceBaseData* overRideBaseData;       // 068
		const ImageSpaceBaseData* underwaterBaseData;     // 070
		ImageSpaceBaseData* consoleBaseData;              // 078
		ImageSpaceData currentEOFData;                    // 080
		ImageSpaceLUTData lutData;                        // 130
		ImageSpaceLUTData* overrideLUTData;               // 1A0
		float motionBlurIntensity;                        // 1A8
		float motionBlurMaxBlur;                          // 1AC
		std::uint32_t forceNoBokehDepthOfField;           // 1B0
		bool isReady;                                     // 1B4
		bool saveTarget;                                  // 1B5
		bool useBokehDepthOfField;                        // 1B6
	};
	static_assert(sizeof(ImageSpaceManager) == 0x1B8);
}
