#pragma once



#include "RE/Bethesda/BSTSmartPointer.h"  // BSIntrusiveRefCounted


namespace RE
{
	namespace BSScript
	{
		namespace Internal
		{
			class __declspec(novtable) IFuncCallQuery : 
				public BSIntrusiveRefCounted
			{
			public:
				static constexpr auto RTTI{ RTTI::BSScript__Internal__IFuncCallQuery };
				static constexpr auto VTABLE{ VTABLE::BSScript__Internal__IFuncCallQuery };
				enum class CallType
				{
					kMember,
					kStatic,
					kGetter,
					kSetter
				};
				virtual ~IFuncCallQuery();		// 00

				// add
				// TODO: Possibly GetFunctionCallInfo? look at CommonLibSSE-NG
				virtual void Unk_01(void) = 0;	// 01


				// members
				std::uint32_t pad0C;	// 0C
			};
			static_assert(sizeof(IFuncCallQuery) == 0x10);
		}
	}
}
