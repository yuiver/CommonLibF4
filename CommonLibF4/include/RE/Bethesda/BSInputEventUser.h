#pragma once

#include "RE/Bethesda/InputEvent.h"
#include "RE/Bethesda/MemoryManager.h"

namespace RE
{
	class alignas(0x08) BSInputEventUser
	{
	public:
		static constexpr auto RTTI{ RTTI::BSInputEventUser };
		static constexpr auto VTABLE{ VTABLE::BSInputEventUser };

		virtual ~BSInputEventUser() = default;  // 00

		// add
		virtual bool ShouldHandleEvent(const InputEvent*) { return false; }  // 01
		virtual void HandleEvent(const KinectEvent*) { return; }
		virtual void HandleEvent(const DeviceConnectEvent*) { return; }
		virtual void HandleEvent(const ThumbstickEvent*) { return; }
		virtual void HandleEvent(const CursorMoveEvent*) { return; }
		virtual void HandleEvent(const MouseMoveEvent*) { return; }
		virtual void HandleEvent(const CharacterEvent*) { return; }
		virtual void HandleEvent(const ButtonEvent*) { return; }

		F4_HEAP_REDEFINE_NEW(BSInputEventUser);

		// members
		bool inputEventHandlingEnabled{ true };  // 08
	};
	static_assert(sizeof(BSInputEventUser) == 0x10);

	struct __declspec(novtable) DisconnectHandler :
		public BSInputEventUser
	{
	public:
		static constexpr auto RTTI{ RTTI::DisconnectHandler };
		static constexpr auto VTABLE{ VTABLE::DisconnectHandler };
	};
	static_assert(sizeof(DisconnectHandler) == 0x10);
}
