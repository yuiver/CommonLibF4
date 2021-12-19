#include "RE/Bethesda/TESObjectREFRs.h"

#include "RE/Bethesda/TESBoundObjects.h"
#include "RE/NetImmerse/NiAVObject.h"

namespace RE
{
	BIPOBJECT::~BIPOBJECT()
	{
		Dtor();
		stl::memzero(this);
	}

	bool TESObjectREFR::IsFurniture()
	{
		return (data.objectReference && data.objectReference->formType.any(RE::ENUM_FORM_ID::kFURN, RE::ENUM_FORM_ID::kTERM));
	}
}
