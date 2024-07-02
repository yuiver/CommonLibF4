#include "REL/Relocation.h"

#include "REX/W32/KERNEL32.h"

namespace REL
{
	void safe_write(std::uintptr_t a_dst, const void* a_src, std::size_t a_count)
	{
		std::uint32_t old{ 0 };
		bool          success = REX::W32::VirtualProtect(
            reinterpret_cast<void*>(a_dst), a_count, REX::W32::PAGE_EXECUTE_READWRITE, std::addressof(old));
		if (success) {
			std::memcpy(reinterpret_cast<void*>(a_dst), a_src, a_count);
			success = REX::W32::VirtualProtect(
				reinterpret_cast<void*>(a_dst), a_count, old, std::addressof(old));
		}

		assert(success);
	}

	void safe_fill(std::uintptr_t a_dst, std::uint8_t a_value, std::size_t a_count)
	{
		std::uint32_t old{ 0 };
		bool          success = REX::W32::VirtualProtect(
            reinterpret_cast<void*>(a_dst), a_count, REX::W32::PAGE_EXECUTE_READWRITE, std::addressof(old));
		if (success) {
			std::fill_n(reinterpret_cast<std::uint8_t*>(a_dst), a_count, a_value);
			success = REX::W32::VirtualProtect(
				reinterpret_cast<void*>(a_dst), a_count, old, std::addressof(old));
		}

		assert(success);
	}

	void Module::clear()
	{
		if (_injectedModule) {
			WinAPI::FreeLibrary(_injectedModule);
			_injectedModule = nullptr;
		}
		_base = 0;
		_filename.clear();
		_filePath.clear();
		_runtime = Runtime::F4;
		_version = { 0, 0, 0, 0 };
		for (auto& segment : _segments) {
			segment = {};
		}
		IDDatabase::_instance.clear();
		IDDatabase::_initialized = false;
	}

	IDDatabase IDDatabase::_instance;
}
