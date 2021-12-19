#pragma once

#include "RE/Bethesda/BSLock.h"
#include "RE/Bethesda/BSPointerHandle.h"
#include "RE/Bethesda/BSTArray.h"
#include "RE/Bethesda/BSTHashMap.h"
#include "RE/Bethesda/BSTSingleton.h"

namespace RE
{
	class BGSRegisteredStoryEvent;
	class BGSStoryEvent;

	class BGSStoryEventManager :
		public BSTSingletonImplicit<BGSStoryEventManager>
	{
	public:
		[[nodiscard]] static BGSStoryEventManager* GetSingleton()
		{
			REL::Relocation<BGSStoryEventManager**> singleton{ REL::ID(1160067) };
			return *singleton;
		}

		std::uint32_t AddEvent(std::uint32_t a_index, const void* a_event)
		{
			using func_t = std::uint32_t(BGSStoryEventManager::*)(std::uint32_t, const void*);
			REL::Relocation<func_t> func{ REL::ID(312533) };
			return func(this, a_index, a_event);
		}

		template <class T>
		std::uint32_t AddEvent(const T& a_event)
		{
			return AddEvent(T::EVENT_INDEX(), &a_event);
		}

		// members
		BSTArray<BGSRegisteredStoryEvent> registeredEventArray;         // 00
		BSTHashMap<std::uint32_t, std::uint32_t> registeredEventIDMap;  // 18
		BSTArray<BGSStoryEvent> eventArray;                             // 48
		BSSpinLock eventArrayLock;                                      // 60
	};
	static_assert(sizeof(BGSStoryEventManager) == 0x68);

	struct BGSCraftItemEvent
	{
	public:
		BGSCraftItemEvent(TESObjectREFR* a_workbench, BGSLocation* a_location, TESForm* a_baseObject) :
			location(a_location), createdItemBase(a_baseObject)
		{
			if (a_workbench) {
				workbench = a_workbench->GetHandle();
			}
		}

		[[nodiscard]] static std::uint32_t EVENT_INDEX()
		{
			REL::Relocation<std::uint32_t*> eventIdx{ REL::ID(1382765) };
			return *eventIdx;
		}

		// members
		ObjectRefHandle workbench;  // 00
		BGSLocation* location;      // 08
		TESForm* createdItemBase;   // 10
	};
	static_assert(sizeof(BGSCraftItemEvent) == 0x18);
}
