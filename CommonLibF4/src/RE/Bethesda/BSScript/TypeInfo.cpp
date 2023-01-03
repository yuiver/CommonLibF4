	
#include "RE/Bethesda/BSScript/IComplexType.h"
#include "RE/Bethesda/BSScript/TypeInfo.h"
namespace RE::BSScript
{

  auto TypeInfo::GetRawType() const
		-> RawType
	{
		if (IsComplex()) {
			const auto complex =
				reinterpret_cast<IComplexType*>(
					reinterpret_cast<std::uintptr_t>(data.complexTypeInfo) &
					~static_cast<std::uintptr_t>(1));
			return complex->GetRawType();
		} else {
			return *data.rawType;
		}
	}
}