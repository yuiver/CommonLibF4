#include "REX/W32/BCRYPT.h"

REX_W32_IMPORT(REX::W32::NTSTATUS, BCryptOpenAlgorithmProvider, REX::W32::BCRYPT_ALG_HANDLE*, const wchar_t*, const wchar_t*, std::uint32_t);
REX_W32_IMPORT(REX::W32::NTSTATUS, BCryptCloseAlgorithmProvider, REX::W32::BCRYPT_ALG_HANDLE, std::uint32_t);
REX_W32_IMPORT(REX::W32::NTSTATUS, BCryptCreateHash, REX::W32::BCRYPT_ALG_HANDLE, BCRYPT_HASH_HANDLE*, std::uint8_t*, std::uint32_t, std::uint8_t*, std::uint32_t, std::uint32_t);
REX_W32_IMPORT(REX::W32::NTSTATUS, BCryptDestroyHash, REX::W32::BCRYPT_HASH_HANDLE);
REX_W32_IMPORT(REX::W32::NTSTATUS, BCryptFinishHash, REX::W32::BCRYPT_HASH_HANDLE, std::uint8_t*, std::uint32_t, std::uint32_t);
REX_W32_IMPORT(REX::W32::NTSTATUS, BCryptGetProperty, REX::W32::BCRYPT_HANDLE, const wchar_t*, std::uint8_t*, std::uint32_t, std::uint32_t*, std::uint32_t);
REX_W32_IMPORT(REX::W32::NTSTATUS, BCryptHashData, REX::W32::BCRYPT_HASH_HANDLE, std::uint8_t*, std::uint32_t, std::uint32_t);

namespace REX::W32
{
	NTSTATUS BCryptOpenAlgorithmProvider(BCRYPT_ALG_HANDLE* a_algorithm, const wchar_t* a_id, const wchar_t* a_impl, std::uint32_t a_flags)
	{
		return ::W32_IMPL_BCryptOpenAlgorithmProvider(a_algorithm, a_id, a_impl, a_flags);
	}

	NTSTATUS BCryptCloseAlgorithmProvider(BCRYPT_ALG_HANDLE a_algorithm, std::uint32_t a_flags)
	{
		return ::W32_IMPL_BCryptCloseAlgorithmProvider(a_algorithm, a_flags);
	}

	NTSTATUS BCryptCreateHash(BCRYPT_ALG_HANDLE a_algorithm, BCRYPT_HASH_HANDLE* a_hash, std::uint8_t* a_hashObject, std::uint32_t a_hashObjectSize, std::uint8_t* a_secret, std::uint32_t a_secretSize, std::uint32_t a_flags)
	{
		return ::W32_IMPL_BCryptCreateHash(a_algorithm, a_hash, a_hashObject, a_hashObjectSize, a_secret, a_secretSize, a_flags);
	}

	NTSTATUS BCryptDestroyHash(BCRYPT_HASH_HANDLE a_hash)
	{
		return ::W32_IMPL_BCryptDestroyHash(a_hash);
	}

	NTSTATUS BCryptFinishHash(BCRYPT_HASH_HANDLE a_hash, std::uint8_t* a_output, std::uint32_t a_outputSize, std::uint32_t a_flags)
	{
		return ::W32_IMPL_BCryptFinishHash(a_hash, a_output, a_outputSize, a_flags);
	}

	NTSTATUS BCryptGetProperty(BCRYPT_HANDLE a_object, const wchar_t* a_property, std::uint8_t* a_output, std::uint32_t a_outputSize, std::uint32_t* a_result, std::uint32_t a_flags)
	{
		return ::W32_IMPL_BCryptGetProperty(a_object, a_property, a_output, a_outputSize, a_result, a_flags);
	}

	NTSTATUS BCryptHashData(BCRYPT_HASH_HANDLE a_hash, std::uint8_t* a_input, std::uint32_t a_inputSize, std::uint32_t a_flags)
	{
		return ::W32_IMPL_BCryptHashData(a_hash, a_input, a_inputSize, a_flags);
	}
}
