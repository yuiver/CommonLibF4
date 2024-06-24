#pragma once

#include "RE/Bethesda/BSTEvent.h"
#include "RE/Bethesda/TESObjectREFRs.h"
#include "RE/NetImmerse/NiSmartPointer.h"

namespace RE
{
	class TESObjectCELL;

	struct TESCellAttachDetachEvent
	{
		NiPointer<TESObjectREFR> reference;  // 00
		bool attached;                       // 08	- 01 - attached, 00 - detached
	};

	inline void RegisterForCellAttachDetach(BSTEventSink<TESCellAttachDetachEvent>* a_sink)
		{
			using func_t = decltype(&RegisterForCellAttachDetach);
			REL::Relocation<func_t> func{ REL::ID(409767) };
			return func(a_sink);
		}
	static_assert(sizeof(TESCellAttachDetachEvent) == 0x10);
}
