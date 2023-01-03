#include "RE/Bethesda/BSScript/Object.h"

namespace RE::BSScript
{
	Object::~Object()
	{
		if (Constructed()) {
			const std::uint32_t size = type ? type->GetVariableCount() : 0;
			for (std::uint32_t i = 0; i < size; ++i) {
				variables[i].reset();
			}

			constructed = false;
			initialized = false;
		}

		auto lock = reinterpret_cast<std::uintptr_t>(lockStructure) & ~static_cast<std::uintptr_t>(1);
		if (lock) {
			stl::atomic_ref l{ lock };
			--l;
		}
	}
}