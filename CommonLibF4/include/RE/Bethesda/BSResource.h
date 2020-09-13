#pragma once

#include "RE/Bethesda/BSFixedString.h"
#include "RE/Bethesda/BSSystemFile.h"
#include "RE/Bethesda/BSTSmartPointer.h"

namespace RE
{
	class BSEventFlag;
}

namespace RE::BSResource
{
	class AsyncStream;
	class ICacheDriveOp;
	class Info;
	class Location;

	enum class ErrorCode : std::uint32_t
	{
		kNone = 0,
		kNotExist = 1,
		kInvalidPath = 2,
		kFileError = 3,
		kInvalidType = 4,
		kMemoryError = 5,
		kBusy = 6,
		kInvalidParam = 7,
		kUnsupported = 8
	};

	enum class SeekMode
	{
		kSet = 0,
		kCurrent = 1,
		kEnd = 2
	};

	struct FileID
	{
	public:
		// members
		std::uint32_t file;  // 0
		std::uint32_t ext;   // 4
	};
	static_assert(sizeof(FileID) == 0x8);

	struct ID :
		public FileID  // 0
	{
	public:
		// members
		std::uint32_t dir;  // 8
	};
	static_assert(sizeof(ID) == 0xC);

	struct LooseFileStreamBase
	{
	public:
		static constexpr auto RTTI{ RTTI::BSResource____LooseFileStreamBase };

		[[nodiscard]] std::string_view GetDirName() const { return dirName; }
		[[nodiscard]] std::string_view GetFileName() const { return fileName; }
		[[nodiscard]] std::string_view GetPrefix() const { return prefix; }

		// members
		BSFixedString prefix;             // 00
		BSFixedString dirName;            // 08
		BSFixedString fileName;           // 10
		BSSystemFile file;                // 18
		BSSystemFileStreamer::ID pathID;  // 28
	};
	static_assert(sizeof(LooseFileStreamBase) == 0x30);

	class __declspec(novtable) StreamBase
	{
	public:
		static constexpr auto RTTI{ RTTI::BSResource__StreamBase };
		static constexpr auto VTABLE{ VTABLE::BSResource__StreamBase };

		virtual ~StreamBase();  // 00

		// add
		virtual ErrorCode DoOpen() { return ErrorCode::kUnsupported; }                                    // 01
		virtual void DoClose() { return; }                                                                // 02
		virtual std::uint64_t DoGetKey() { return static_cast<std::uint64_t>(-1); }                       // 03
		virtual ErrorCode DoGetInfo([[maybe_unused]] Info& a_result) { return ErrorCode::kUnsupported; }  // 04

		std::int64_t DecRef()
		{
			using func_t = decltype(&StreamBase::DecRef);
			REL::Relocation<func_t> func{ REL::ID(373786) };
			return func(this);
		}

		// members
		std::uint32_t totalSize;  // 08
		std::uint32_t flags;      // 0C
	};
	static_assert(sizeof(StreamBase) == 0x10);

	class __declspec(novtable) Stream :
		public StreamBase  // 00
	{
	public:
		static constexpr auto RTTI{ RTTI::BSResource__Stream };
		static constexpr auto VTABLE{ VTABLE::BSResource__Stream };

		virtual ~Stream();

		// override
		virtual ErrorCode DoOpen() override { return ErrorCode::kUnsupported; }                                    // 01
		virtual void DoClose() override { return; }                                                                // 02
		virtual std::uint64_t DoGetKey() override { return static_cast<std::uint64_t>(-1); }                       // 03
		virtual ErrorCode DoGetInfo([[maybe_unused]] Info& a_result) override { return ErrorCode::kUnsupported; }  // 04

		// add
		virtual void DoClone([[maybe_unused]] BSTSmartPointer<Stream>& a_result) { return; }                                                                                                                                                                                                                                                                                                                            // 05
		virtual ErrorCode DoRead([[maybe_unused]] void* a_buffer, [[maybe_unused]] std::uint64_t a_bytes, [[maybe_unused]] std::uint64_t& a_read) { return ErrorCode::kUnsupported; }                                                                                                                                                                                                                                   // 06
		virtual ErrorCode DoReadAt([[maybe_unused]] void* a_buffer, [[maybe_unused]] std::uint64_t a_bytes, [[maybe_unused]] std::uint64_t a_pos, [[maybe_unused]] std::uint64_t& a_read) { return ErrorCode::kUnsupported; }                                                                                                                                                                                           // 07
		virtual ErrorCode DoWrite([[maybe_unused]] const void* a_buffer, [[maybe_unused]] std::uint64_t a_bytes, [[maybe_unused]] std::uint64_t& a_write) { return ErrorCode::kUnsupported; }                                                                                                                                                                                                                           // 08
		virtual ErrorCode DoSeek([[maybe_unused]] std::int64_t a_offset, [[maybe_unused]] SeekMode a_seekMode, [[maybe_unused]] std::uint64_t& a_pos) { return ErrorCode::kUnsupported; }                                                                                                                                                                                                                               // 09
		virtual ErrorCode DoSetEndOfStream() { return ErrorCode::kUnsupported; }                                                                                                                                                                                                                                                                                                                                        // 0A
		virtual ErrorCode DoPrefetchAt([[maybe_unused]] std::uint64_t a_bytes, [[maybe_unused]] std::uint64_t a_offset, [[maybe_unused]] std::uint32_t a_priority) { return ErrorCode::kUnsupported; }                                                                                                                                                                                                                  // 0B
		virtual ErrorCode DoStartTaggedPrioritizedRead([[maybe_unused]] void* a_buffer, [[maybe_unused]] std::uint64_t a_bytes, [[maybe_unused]] std::uint64_t a_offset, [[maybe_unused]] std::uint32_t a_priority, [[maybe_unused]] volatile std::uint32_t* a_completionTag, [[maybe_unused]] std::uint32_t& a_completionTagWaitValue, [[maybe_unused]] BSEventFlag* a_eventFlag) { return ErrorCode::kUnsupported; }  // 0C
		virtual ErrorCode DoWaitTags([[maybe_unused]] volatile std::uint32_t* a_completionTag, [[maybe_unused]] std::uint32_t a_completionTagWaitValue, [[maybe_unused]] BSEventFlag* a_eventFlag) { return ErrorCode::kUnsupported; }                                                                                                                                                                                  // 0D
		virtual ErrorCode DoPrefetchAll([[maybe_unused]] std::uint32_t a_priority) { return ErrorCode::kUnsupported; }                                                                                                                                                                                                                                                                                                  // 0E
		virtual bool DoGetName([[maybe_unused]] BSFixedString& a_result) { return false; }                                                                                                                                                                                                                                                                                                                              // 0F
		virtual ErrorCode DoCreateAsync([[maybe_unused]] BSTSmartPointer<AsyncStream>& a_result) { return ErrorCode::kUnsupported; }                                                                                                                                                                                                                                                                                    // 10
		virtual bool DoQTaggedPrioritizedReadSupported() { return false; }                                                                                                                                                                                                                                                                                                                                              // 11
		virtual ErrorCode DoCreateOp([[maybe_unused]] BSTSmartPointer<ICacheDriveOp>& a_result, [[maybe_unused]] char const* a_arg2) { return ErrorCode::kUnsupported; }                                                                                                                                                                                                                                                // 12
		virtual bool DoGetIsFromArchive() { return false; }                                                                                                                                                                                                                                                                                                                                                             // 13
	};
	static_assert(sizeof(Stream) == 0x10);

	[[nodiscard]] ErrorCode GetOrCreateStream(const char* a_fileName, BSTSmartPointer<Stream>& a_result, bool a_writable = false, Location* a_optionalStart = nullptr);
}
