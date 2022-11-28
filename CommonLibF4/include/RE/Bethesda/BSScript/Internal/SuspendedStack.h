#pragma once

#include "RE/Bethesda/BSScript/Stack.h"  // BSScript::Stack


namespace RE
{
	namespace BSScript
	{
		class Stack;


		namespace Internal
		{
			class RawFuncCallQuery;


			struct SuspendedStack
			{
			public:
				BSTSmartPointer<Stack>				stack;	// 00
				// TODO: check if this should be IFuncCallQuery isntead of RawFuncCallQuery
				BSTSmartPointer<RawFuncCallQuery> rawFuncCallQuery;	// 08
			};
			static_assert(sizeof(SuspendedStack) == 0x10);
		}
	}
}
