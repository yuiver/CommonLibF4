#pragma once

#include "RE/Bethesda/BSFixedString.h"
#include "RE/NetImmerse/NiSmartPointer.h"
#include "RE/NetImmerse/NiTexture.h"

namespace RE
{
	namespace BSGraphics
	{
		class Texture;
	}

	struct ImageSpaceBaseData
	{
	public:
		// members
		float hdrData[9];        // 00
		float cinematicData[3];  // 24
		float tintData[4];       // 30
		float dofData[6];        // 40
	};
	static_assert(sizeof(ImageSpaceBaseData) == 0x58);

	struct ImageSpaceLUTData
	{
	public:
		// members
		BSFixedString filename[4];          // 00
		float weight[4];                    // 20
		NiPointer<NiTexture> niTexture[4];  // 30
		BSGraphics::Texture* texture[4];    // 50
	};
	static_assert(sizeof(ImageSpaceLUTData) == 0x70);

	struct ImageSpaceModData
	{
	public:
		// members
		float data[20];  // 00
	};
	static_assert(sizeof(ImageSpaceModData) == 0x50);

	struct ImageSpaceData
	{
	public:
		// members
		ImageSpaceBaseData baseData;  //00
		ImageSpaceModData modData;    //58
		float highestTintAmount;      //A8
		float highestFadeAmount;      //AC
	};
	static_assert(sizeof(ImageSpaceData) == 0xB0);
}
