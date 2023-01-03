#pragma once

#include "RE/Bethesda/BSContainer.h"
#include "RE/Bethesda/BSFixedString.h"
#include "RE/Bethesda/BSLock.h"
#include "RE/Bethesda/BSTArray.h"
#include "RE/Bethesda/BSTEvent.h"
#include "RE/Bethesda/BSTHashMap.h"
#include "RE/Bethesda/BSTObjectArena.h"
#include "RE/Bethesda/BSTSmartPointer.h"
#include "RE/Bethesda/BSTTuple.h"
#include "RE/Bethesda/MemoryManager.h"
#include "RE/Bethesda/BSScript/TypeInfo.h"
namespace RE
{
	namespace BSScript
	{
		class TypeInfo;

		class __declspec(novtable) IComplexType :
			public BSIntrusiveRefCounted  // 08
		{
		public:
			static constexpr auto RTTI{ RTTI::BSScript__IComplexType };
			static constexpr auto VTABLE{ VTABLE::BSScript__IComplexType };

			virtual ~IComplexType() = 0;  // 00

			// add
			virtual TypeInfo::RawType GetRawType() const = 0;  // 01

			[[nodiscard]] bool IsObject() const { return GetRawType() == TypeInfo::RawType::kObject; }
		};
		static_assert(sizeof(IComplexType) == 0x10);
	}
}
