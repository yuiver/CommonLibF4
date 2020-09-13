#include "RE/Bethesda/BSResource.h"

namespace RE
{
	namespace BSResource
	{
		BSResource::ErrorCode GetOrCreateStream(const char* a_fileName, BSTSmartPointer<BSResource::Stream>& a_result, bool a_writable, BSResource::Location* a_optionalStart)
		{
			using func_t = decltype(&GetOrCreateStream);
			REL::Relocation<func_t> func{ REL::ID(1143155) };
			return func(a_fileName, a_result, a_writable, a_optionalStart);
		}
	}
}