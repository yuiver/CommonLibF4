#pragma once




namespace RE
{
	namespace BSScript
	{
		class __declspec(novtable) IVMDebugInterface
		{
		public:
			static constexpr auto RTTI{ RTTI::BSScript__IVMDebugInterface };
			static constexpr auto VTABLE{ VTABLE::BSScript__IVMDebugInterface };


			virtual ~IVMDebugInterface();	// 00

			// add
			// TODO: One of these is "DumpRunningStacksToLog()"
			virtual void Unk_01(void) = 0;	// 01
			virtual void Unk_02(void) = 0;	// 02
			virtual void Unk_03(void) = 0;	// 03
			virtual void Unk_04(void) = 0;	// 04
			virtual void Unk_05(void) = 0;	// 05
		};
		static_assert(sizeof(IVMDebugInterface) == 0x8);
	}
}
