#include "REL/Relocation.h"

#include "REX/W32/BCRYPT.h"

#include "F4SE/Logger.h"

namespace REL
{
	namespace log = F4SE::log;

	namespace detail
	{
		std::optional<std::string> sha512(std::span<const std::byte> a_data)
		{
			REX::W32::BCRYPT_ALG_HANDLE algorithm;
			if (!REX::W32::NT_SUCCESS(REX::W32::BCryptOpenAlgorithmProvider(&algorithm, REX::W32::BCRYPT_SHA512_ALGORITHM))) {
				log::error("failed to open algorithm provider");
				return std::nullopt;
			}

			const stl::scope_exit delAlgorithm([&]() {
				[[maybe_unused]] const auto success = REX::W32::NT_SUCCESS(REX::W32::BCryptCloseAlgorithmProvider(algorithm));
				assert(success);
			});

			REX::W32::BCRYPT_HASH_HANDLE hash;
			if (!REX::W32::NT_SUCCESS(REX::W32::BCryptCreateHash(algorithm, &hash))) {
				log::error("failed to create hash");
				return std::nullopt;
			}

			const stl::scope_exit delHash([&]() {
				[[maybe_unused]] const auto success = REX::W32::NT_SUCCESS(REX::W32::BCryptDestroyHash(hash));
				assert(success);
			});

			if (!REX::W32::NT_SUCCESS(REX::W32::BCryptHashData(
					hash,
					reinterpret_cast<std::uint8_t*>(const_cast<std::byte*>(a_data.data())),  // does not modify contents of buffer
					static_cast<std::uint32_t>(a_data.size())))) {
				log::error("failed to hash data");
				return std::nullopt;
			}

			std::uint32_t hashLen = 0;
			std::uint32_t discard = 0;
			if (!REX::W32::NT_SUCCESS(REX::W32::BCryptGetProperty(
					hash,
					REX::W32::BCRYPT_HASH_LENGTH,
					reinterpret_cast<std::uint8_t*>(&hashLen),
					sizeof(hashLen),
					&discard))) {
				log::error("failed to get property");
				return std::nullopt;
			}

			std::vector<std::uint8_t> buffer(static_cast<std::size_t>(hashLen));
			if (!REX::W32::NT_SUCCESS(REX::W32::BCryptFinishHash(
					hash,
					buffer.data(),
					static_cast<std::uint32_t>(buffer.size())))) {
				log::error("failed to finish hash");
				return std::nullopt;
			}

			std::string result;
			result.reserve(buffer.size() * 2);
			for (const auto byte : buffer) {
				result += std::format("{:02X}", byte);
			}

			return { std::move(result) };
		}
	}

	void Module::load_segments()
	{
		const auto dosHeader = reinterpret_cast<const REX::W32::IMAGE_DOS_HEADER*>(_base);
		const auto ntHeader = stl::adjust_pointer<REX::W32::IMAGE_NT_HEADERS64>(dosHeader, dosHeader->lfanew);
		const auto sections = REX::W32::IMAGE_FIRST_SECTION(ntHeader);
		const auto size = std::min<std::size_t>(ntHeader->fileHeader.sectionCount, _segments.size());
		for (std::size_t i = 0; i < size; ++i) {
			const auto& section = sections[i];
			const auto it = std::find_if(SEGMENTS.begin(), SEGMENTS.end(), [&](auto&& a_elem) {
				constexpr auto size = std::extent_v<decltype(section.name)>;
				const auto len = std::min(a_elem.size(), size);
				return std::memcmp(a_elem.data(), section.name, len) == 0;
			});
			if (it != SEGMENTS.end()) {
				const auto idx = static_cast<std::size_t>(std::distance(SEGMENTS.begin(), it));
				_segments[idx] = Segment{ _base, _base + section.virtualAddress, section.virtualSize };
			}
		}
	}
}
