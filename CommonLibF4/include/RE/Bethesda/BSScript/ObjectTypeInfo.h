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
#include "RE/Bethesda/BSScript/IComplexType.h"
#include "RE/Bethesda/BSScript/PropertyGroupInfo.h"
namespace RE
{
	namespace BSScript
	{
		class IComplexType;
		class PropertyGroupInfo;

		class __declspec(novtable) ObjectTypeInfo :
			public IComplexType  // 00
		{
		public:
			static constexpr auto RTTI{ RTTI::BSScript__ObjectTypeInfo };
			static constexpr auto VTABLE{ VTABLE::BSScript__ObjectTypeInfo };

			enum class LinkValidState : std::uint32_t
			{
				kNotLinked,
				kCurrentlyLinking,
				kLinkedInvalid,
				kLinkedValid
			};

			[[nodiscard]] std::uint32_t GetVariableCount() const noexcept
			{
				std::uint32_t count = 0;
				for (BSTSmartPointer iter{ this }; iter && iter->Valid(); iter = iter->parentTypeInfo) {
					count += iter->variableCount;
				}
				return count;
			}

			[[nodiscard]] bool Valid() const noexcept { return linkedValid == LinkValidState::kLinkedValid; }

			// members
			BSFixedString name;                                           // 10
			BSTSmartPointer<ObjectTypeInfo> parentTypeInfo;               // 18
			BSFixedString docString;                                      // 20
			BSTArray<BSTSmartPointer<PropertyGroupInfo>> propertyGroups;  // 28
			LinkValidState linkedValid: 2;                                // 40:00
			std::uint32_t isConst: 1;                                     // 40:02
			std::uint32_t userFlagCount: 5;                               // 40:03
			std::uint32_t variableCount: 10;                              // 40:08
			std::uint32_t variableUserFlagCount: 6;                       // 40:18
			std::uint32_t initialValueCount: 10;                          // 44:00
			std::uint32_t propertyCount: 10;                              // 44:10
			std::uint32_t staticFunctionCount: 9;                         // 44:20
			std::uint32_t emptyStateMemberFunctionCount: 11;              // 48:00
			std::uint32_t namedStateCount: 7;                             // 48:11
			std::uint32_t initialState: 7;                                // 48:18
			void* data;                                                   // 50
		};
		static_assert(sizeof(ObjectTypeInfo) == 0x58);
	}
}
