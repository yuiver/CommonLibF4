#include "REL/Relocation.h"

#define WIN32_LEAN_AND_MEAN

#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI
#define NOKERNEL
#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

#include <Windows.h>

#include <bcrypt.h>

#define NT_SUCCESS(a_status) (a_status >= 0)

#include "F4SE/Logger.h"

namespace REL
{
	namespace log = F4SE::log;

	namespace detail
	{
		std::optional<std::string> sha512(std::span<const std::byte> a_data)
		{
			::BCRYPT_ALG_HANDLE algorithm;
			if (!NT_SUCCESS(::BCryptOpenAlgorithmProvider(
					&algorithm,
					BCRYPT_SHA512_ALGORITHM,
					nullptr,
					0))) {
				log::error("failed to open algorithm provider");
				return std::nullopt;
			}
			const stl::scope_exit delAlgorithm([&]() {
				[[maybe_unused]] const auto success = NT_SUCCESS(::BCryptCloseAlgorithmProvider(algorithm, 0));
				assert(success);
			});

			::BCRYPT_HASH_HANDLE hash;
			if (!NT_SUCCESS(::BCryptCreateHash(
					algorithm,
					&hash,
					nullptr,
					0,
					nullptr,
					0,
					0))) {
				log::error("failed to create hash");
				return std::nullopt;
			}
			const stl::scope_exit delHash([&]() {
				[[maybe_unused]] const auto success = NT_SUCCESS(::BCryptDestroyHash(hash));
				assert(success);
			});

			if (!NT_SUCCESS(::BCryptHashData(
					hash,
					reinterpret_cast<::PUCHAR>(const_cast<std::byte*>(a_data.data())),  // does not modify contents of buffer
					static_cast<::ULONG>(a_data.size()),
					0))) {
				log::error("failed to hash data");
				return std::nullopt;
			}

			::DWORD hashLen = 0;
			::ULONG discard = 0;
			if (!NT_SUCCESS(::BCryptGetProperty(
					hash,
					BCRYPT_HASH_LENGTH,
					reinterpret_cast<::PUCHAR>(&hashLen),
					sizeof(hashLen),
					&discard,
					0))) {
				log::error("failed to get property");
				return std::nullopt;
			}
			std::vector<::UCHAR> buffer(static_cast<std::size_t>(hashLen));

			if (!NT_SUCCESS(::BCryptFinishHash(
					hash,
					buffer.data(),
					static_cast<::ULONG>(buffer.size()),
					0))) {
				log::error("failed to finish hash");
				return std::nullopt;
			}

			std::string result;
			result.reserve(buffer.size() * 2);
			for (const auto byte : buffer) {
				result += fmt::format("{:02X}", byte);
			}

			return { std::move(result) };
		}
	}

	void Module::load_segments()
	{
		auto dosHeader = reinterpret_cast<const ::IMAGE_DOS_HEADER*>(_base);
		auto ntHeader = stl::adjust_pointer<::IMAGE_NT_HEADERS64>(dosHeader, dosHeader->e_lfanew);
		const auto* sections = IMAGE_FIRST_SECTION(ntHeader);
		const auto size = std::min<std::size_t>(ntHeader->FileHeader.NumberOfSections, _segments.size());
		for (std::size_t i = 0; i < size; ++i) {
			const auto& section = sections[i];
			const auto it = std::find_if(
				SEGMENTS.begin(),
				SEGMENTS.end(),
				[&](auto&& a_elem) {
					constexpr auto size = std::extent_v<decltype(section.Name)>;
					const auto len = std::min(a_elem.size(), size);
					return std::memcmp(a_elem.data(), section.Name, len) == 0;
				});
			if (it != SEGMENTS.end()) {
				const auto idx = static_cast<std::size_t>(std::distance(SEGMENTS.begin(), it));
				_segments[idx] = Segment{ _base, _base + section.VirtualAddress, section.Misc.VirtualSize };
			}
		}
	}
}
