#pragma once

#include "RE/Bethesda/BSTEvent.h"
#include "RE/Bethesda/IMenu.h"
#include "RE/Bethesda/PipboyValue.h"

namespace RE
{
	class PipboyDataGroup : BSTEventSource<PipboyValueChangedEvent>
	{
	public:
		static constexpr auto RTTI{ RTTI::PipboyDataGroup };
		static constexpr auto VTABLE{ VTABLE::PipboyDataGroup };

		virtual ~PipboyDataGroup();

		virtual void Populate(bool);
		virtual void DoClearData();
		virtual void DoClearSink();

		REX::W32::CRITICAL_SECTION PipboyDataMutex;
		void* rThrottleManager;  // PipboyThrottleManager
		PipboyValue* rootValue;
	};
	static_assert(sizeof(PipboyDataGroup) == 0x98);
}
