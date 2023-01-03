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
#include "RE/Bethesda/BSScript/Variable.h"
namespace RE
{
	namespace BSScript
	{
		class TypeInfo;
		class Variable;

		class Array :
			public BSIntrusiveRefCounted  // 00
		{
		public:
			// members
			TypeInfo elementType;         // 08
			BSSpinLock elementsLock;      // 10
			BSTArray<Variable> elements;  // 18
		};
		static_assert(sizeof(Array) == 0x30);
	}
}
