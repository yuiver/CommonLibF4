#include "F4SE/InputMap.h"

#include "F4SE/Impl/ScePadAPI.h"
#include "F4SE/Impl/XInputAPI.h"
#include "RE/Bethesda/ControlMap.h"

namespace F4SE
{
	using XInputButton = RE::XInput::XInputButton;
	using ScePadButton = RE::ScePad::ScePadButton;

	std::uint32_t InputMap::XInputToScePadOffset(std::uint32_t keyMask)
	{
		switch (keyMask) {
		case XInputButton::XINPUT_GAMEPAD_DPAD_UP:
			return ScePadButton::SCE_PAD_BUTTON_UP;
		case XInputButton::XINPUT_GAMEPAD_DPAD_DOWN:
			return ScePadButton::SCE_PAD_BUTTON_DOWN;
		case XInputButton::XINPUT_GAMEPAD_DPAD_LEFT:
			return ScePadButton::SCE_PAD_BUTTON_LEFT;
		case XInputButton::XINPUT_GAMEPAD_DPAD_RIGHT:
			return ScePadButton::SCE_PAD_BUTTON_RIGHT;
		case XInputButton::XINPUT_GAMEPAD_START:
			return ScePadButton::SCE_PAD_BUTTON_OPTIONS;
		case XInputButton::XINPUT_GAMEPAD_BACK:
			return ScePadButton::SCE_PAD_BUTTON_TOUCH_PAD;
		case XInputButton::XINPUT_GAMEPAD_LEFT_THUMB:
			return ScePadButton::SCE_PAD_BUTTON_L3;
		case XInputButton::XINPUT_GAMEPAD_RIGHT_THUMB:
			return ScePadButton::SCE_PAD_BUTTON_R3;
		case XInputButton::XINPUT_GAMEPAD_LEFT_SHOULDER:
			return ScePadButton::SCE_PAD_BUTTON_L1;
		case XInputButton::XINPUT_GAMEPAD_RIGHT_SHOULDER:
			return ScePadButton::SCE_PAD_BUTTON_R1;
		case XInputButton::XINPUT_GAMEPAD_A:
			return ScePadButton::SCE_PAD_BUTTON_CROSS;
		case XInputButton::XINPUT_GAMEPAD_B:
			return ScePadButton::SCE_PAD_BUTTON_CIRCLE;
		case XInputButton::XINPUT_GAMEPAD_X:
			return ScePadButton::SCE_PAD_BUTTON_SQUARE;
		case XInputButton::XINPUT_GAMEPAD_Y:
			return ScePadButton::SCE_PAD_BUTTON_TRIANGLE;
		default:
			return keyMask;
		}
	}

	std::uint32_t InputMap::ScePadOffsetToXInput(std::uint32_t keyMask)
	{
		switch (keyMask) {
		case ScePadButton::SCE_PAD_BUTTON_UP:
			return XInputButton::XINPUT_GAMEPAD_DPAD_UP;
		case ScePadButton::SCE_PAD_BUTTON_DOWN:
			return XInputButton::XINPUT_GAMEPAD_DPAD_DOWN;
		case ScePadButton::SCE_PAD_BUTTON_LEFT:
			return XInputButton::XINPUT_GAMEPAD_DPAD_LEFT;
		case ScePadButton::SCE_PAD_BUTTON_RIGHT:
			return XInputButton::XINPUT_GAMEPAD_DPAD_RIGHT;
		case ScePadButton::SCE_PAD_BUTTON_OPTIONS:
			return XInputButton::XINPUT_GAMEPAD_START;
		case ScePadButton::SCE_PAD_BUTTON_TOUCH_PAD:
			return XInputButton::XINPUT_GAMEPAD_BACK;
		case ScePadButton::SCE_PAD_BUTTON_L3:
			return XInputButton::XINPUT_GAMEPAD_LEFT_THUMB;
		case ScePadButton::SCE_PAD_BUTTON_R3:
			return XInputButton::XINPUT_GAMEPAD_RIGHT_THUMB;
		case ScePadButton::SCE_PAD_BUTTON_L1:
			return XInputButton::XINPUT_GAMEPAD_LEFT_SHOULDER;
		case ScePadButton::SCE_PAD_BUTTON_R1:
			return XInputButton::XINPUT_GAMEPAD_RIGHT_SHOULDER;
		case ScePadButton::SCE_PAD_BUTTON_CROSS:
			return XInputButton::XINPUT_GAMEPAD_A;
		case ScePadButton::SCE_PAD_BUTTON_CIRCLE:
			return XInputButton::XINPUT_GAMEPAD_B;
		case ScePadButton::SCE_PAD_BUTTON_SQUARE:
			return XInputButton::XINPUT_GAMEPAD_X;
		case ScePadButton::SCE_PAD_BUTTON_TRIANGLE:
			return XInputButton::XINPUT_GAMEPAD_Y;
		default:
			return keyMask;
		}
	}

	std::uint32_t InputMap::GamepadMaskToKeycode(std::uint32_t keyMask)
	{
		if (RE::ControlMap::GetSingleton()->pcGamePadMapType == RE::PC_GAMEPAD_TYPE::kOrbis) {
			keyMask = ScePadOffsetToXInput(keyMask);
		}

		switch (keyMask) {
		case XInputButton::XINPUT_GAMEPAD_DPAD_UP:
			return kGamepadButtonOffset_DPAD_UP;
		case XInputButton::XINPUT_GAMEPAD_DPAD_DOWN:
			return kGamepadButtonOffset_DPAD_DOWN;
		case XInputButton::XINPUT_GAMEPAD_DPAD_LEFT:
			return kGamepadButtonOffset_DPAD_LEFT;
		case XInputButton::XINPUT_GAMEPAD_DPAD_RIGHT:
			return kGamepadButtonOffset_DPAD_RIGHT;
		case XInputButton::XINPUT_GAMEPAD_START:
			return kGamepadButtonOffset_START;
		case XInputButton::XINPUT_GAMEPAD_BACK:
			return kGamepadButtonOffset_BACK;
		case XInputButton::XINPUT_GAMEPAD_LEFT_THUMB:
			return kGamepadButtonOffset_LEFT_THUMB;
		case XInputButton::XINPUT_GAMEPAD_RIGHT_THUMB:
			return kGamepadButtonOffset_RIGHT_THUMB;
		case XInputButton::XINPUT_GAMEPAD_LEFT_SHOULDER:
			return kGamepadButtonOffset_LEFT_SHOULDER;
		case XInputButton::XINPUT_GAMEPAD_RIGHT_SHOULDER:
			return kGamepadButtonOffset_RIGHT_SHOULDER;
		case XInputButton::XINPUT_GAMEPAD_A:
			return kGamepadButtonOffset_A;
		case XInputButton::XINPUT_GAMEPAD_B:
			return kGamepadButtonOffset_B;
		case XInputButton::XINPUT_GAMEPAD_X:
			return kGamepadButtonOffset_X;
		case XInputButton::XINPUT_GAMEPAD_Y:
			return kGamepadButtonOffset_Y;
		case 0x9:  // Left Trigger game-defined ID
			return kGamepadButtonOffset_LT;
		case 0xA:  // Right Trigger game-defined ID
			return kGamepadButtonOffset_RT;
		default:
			return kMaxMacros;  // Invalid
		}
	}

	std::uint32_t InputMap::GamepadKeycodeToMask(std::uint32_t keyCode)
	{
		std::uint32_t keyMask;

		switch (keyCode) {
		case kGamepadButtonOffset_DPAD_UP:
			keyMask = XInputButton::XINPUT_GAMEPAD_DPAD_UP;
			break;
		case kGamepadButtonOffset_DPAD_DOWN:
			keyMask = XInputButton::XINPUT_GAMEPAD_DPAD_DOWN;
			break;
		case kGamepadButtonOffset_DPAD_LEFT:
			keyMask = XInputButton::XINPUT_GAMEPAD_DPAD_LEFT;
			break;
		case kGamepadButtonOffset_DPAD_RIGHT:
			keyMask = XInputButton::XINPUT_GAMEPAD_DPAD_RIGHT;
			break;
		case kGamepadButtonOffset_START:
			keyMask = XInputButton::XINPUT_GAMEPAD_START;
			break;
		case kGamepadButtonOffset_BACK:
			keyMask = XInputButton::XINPUT_GAMEPAD_BACK;
			break;
		case kGamepadButtonOffset_LEFT_THUMB:
			keyMask = XInputButton::XINPUT_GAMEPAD_LEFT_THUMB;
			break;
		case kGamepadButtonOffset_RIGHT_THUMB:
			keyMask = XInputButton::XINPUT_GAMEPAD_RIGHT_THUMB;
			break;
		case kGamepadButtonOffset_LEFT_SHOULDER:
			keyMask = XInputButton::XINPUT_GAMEPAD_LEFT_SHOULDER;
			break;
		case kGamepadButtonOffset_RIGHT_SHOULDER:
			keyMask = XInputButton::XINPUT_GAMEPAD_RIGHT_SHOULDER;
			break;
		case kGamepadButtonOffset_A:
			keyMask = XInputButton::XINPUT_GAMEPAD_A;
			break;
		case kGamepadButtonOffset_B:
			keyMask = XInputButton::XINPUT_GAMEPAD_B;
			break;
		case kGamepadButtonOffset_X:
			keyMask = XInputButton::XINPUT_GAMEPAD_X;
			break;
		case kGamepadButtonOffset_Y:
			keyMask = XInputButton::XINPUT_GAMEPAD_Y;
			break;
		case kGamepadButtonOffset_LT:
			keyMask = 0x9;  // Left Trigger game-defined ID
			break;
		case kGamepadButtonOffset_RT:
			keyMask = 0xA;  // Right Trigger game-defined ID
			break;
		default:
			keyMask = 0xFF;  // Invalid
			break;
		}

		if (RE::ControlMap::GetSingleton()->pcGamePadMapType == RE::PC_GAMEPAD_TYPE::kOrbis) {
			keyMask = XInputToScePadOffset(keyMask);
		}

		return keyMask;
	}
}
