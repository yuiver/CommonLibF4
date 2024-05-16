#pragma once

#include "RE/Bethesda/BSFixedString.h"
#include "RE/Bethesda/BSTArray.h"
#include "RE/Bethesda/BSTHashMap.h"

namespace RE
{
	class PipboyObject;

	class __declspec(novtable) PipboyValue
	{
	public:
		static constexpr auto RTTI{ RTTI::PipboyValue };
		static constexpr auto VTABLE{ VTABLE::PipboyValue };

		enum SERIALIZATION_DATA_TYPE : std::uint8_t
		{
			SDT_BOOL = 0x0,
			SDT_INT_8 = 0x1,
			SDT_UINT_8 = 0x2,
			SDT_INT_32 = 0x3,
			SDT_UINT_32 = 0x4,
			SDT_FLOAT = 0x5,
			SDT_STRING = 0x6,
			SDT_ARRAY = 0x7,
			SDT_OBJECT = 0x8,
		};
	
		virtual ~PipboyValue();

		virtual void CleanDirtyToGame();
		virtual void Serialize(void* a_args1);
		virtual void SerializeChanged(void* a_args1);
		virtual SERIALIZATION_DATA_TYPE GetType();

		// members
		std::uint32_t id;			// 08
		bool isDirtyGame;			// 0C
		bool isDirtyToCompanion;	// 0D
		PipboyValue* parentValue;	// 10
	};
	static_assert(sizeof(PipboyValue) == 0x18);

	class __declspec(novtable) PipboyObject :
		PipboyValue
	{
	public:
		static constexpr auto RTTI{ RTTI::PipboyObject };
		static constexpr auto VTABLE{ VTABLE::PipboyObject };

		virtual ~PipboyObject();

		//members
		BSTHashMap<BSFixedString, PipboyValue*> memberMap;
		BSTSet<std::uint32_t> addedMemberIDs;
		BSTArray<std::uint32_t> removedMemberIDs;
		bool newlyCreated;
	};
	static_assert(sizeof(PipboyObject) == 0x98);
}
