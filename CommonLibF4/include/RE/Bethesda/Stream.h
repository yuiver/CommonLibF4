#pragma once

#include "RE/Bethesda/Atomic.h"
#include "RE/Bethesda/BSFixedString.h"
#include "RE/Bethesda/BSTSmartPointer.h"
#include "RE/Bethesda/StreamBase.h"

namespace RE
{
	namespace BSResource
	{
		class CacheDrive;
		class AsyncStream;
		class __declspec(novtable) ICacheDriveOp :
			public BSIntrusiveRefCounted
		{
			virtual ~ICacheDriveOp() = default;
			virtual void OnValid(BSResource::CacheDrive& p_cache_drive) = 0;                           // 142D39FA0
			virtual void OnInit(BSResource::CacheDrive& p_cache_drive) = 0;                            // 142D39FA8
			virtual void OnPacketWrite(BSResource::CacheDrive& p_cache_drive, std::uint64_t a_v) = 0;  // 142D39FB0
			virtual void OnComplete(BSResource::CacheDrive& p_cache_drive) = 0;                        // 142D39FB8
			virtual void OnError(BSResource::CacheDrive& p_cache_drive) = 0;                           // 142D39FC0
			// members
			// align 4
			std::uint32_t unk08;  // 08
		};
		static_assert(sizeof(ICacheDriveOp) == 0x10);

		enum class SeekMode
		{
			kSet = 0,
			kCur = 1,
			kEnd = 2
		};

		class __declspec(novtable) Stream :
			public StreamBase
		{
		public:
			inline static constexpr auto RTTI = { RTTI::BSResource__Stream };
			inline static constexpr auto VTABLE = { VTABLE::BSResource__Stream };

			Stream();
			Stream(const Stream& a_rhs);
			Stream(Stream&& a_rhs);
			Stream(std::uint32_t a_totalSize, bool writable);
			~Stream() override = default;  // 00

			// virtual ErrorCode                   DoOpen() = 0;            // 01
			// virtual void                        DoClose() = 0;           // 02
			// [[nodiscard]] virtual std::uint64_t DoGetKey() const;        // 03 - { return 0; }
			// virtual ErrorCode                   DoGetInfo(Info& a_info); // 04 - { return ErrorCode::kUnsupported; }

			virtual void DoClone(BSTSmartPointer<Stream>& a_out) const = 0;                                                                                    // 05
			virtual ErrorCode DoRead(void* a_buffer, std::uint64_t a_toRead, std::uint64_t& a_read) const = 0;                                                 // 06
			virtual ErrorCode DoReadAt(
				[[maybe_unused]] void* a_buffer, 
				[[maybe_unused]] std::uint64_t a_offset, 
				[[maybe_unused]] std::uint64_t a_toRead, 
				[[maybe_unused]] std::uint64_t& a_read) const;                           // 07
			virtual ErrorCode DoWrite(const void* a_buffer, std::uint64_t a_toWrite, std::uint64_t& a_written) const = 0;                                      // 08
			virtual ErrorCode DoSeek(std::uint64_t a_toSeek, SeekMode a_mode, std::uint64_t& a_sought) const = 0;                                              // 09
			virtual ErrorCode DoSetEndOfStream();                                                                                                              // 142E13608
			virtual ErrorCode DoPrefetchAt([[maybe_unused]] std::uint64_t a_v, [[maybe_unused]] std::uint64_t b_v, [[maybe_unused]] std::uint32_t c_v) const;  // 142E13610
			virtual ErrorCode DoStartTaggedPrioritizedRead(
				[[maybe_unused]] void* a_buf,
				[[maybe_unused]] std::uint64_t a_v,
				[[maybe_unused]] std::uint64_t b_v,
				[[maybe_unused]] std::uint32_t c_v,
				[[maybe_unused]] std::uint32_t volatile* d_v,
				[[maybe_unused]] std::uint32_t& e_v,
				[[maybe_unused]] BSEventFlag* event_flag) const;                                                                                                             // 142E13618
			virtual ErrorCode DoWaitTags(
				[[maybe_unused]] std::uint32_t volatile* a_v, 
				[[maybe_unused]] std::uint32_t b_v, 
				[[maybe_unused]] BSEventFlag* event_flag) const;  // 142E13620
			virtual ErrorCode DoPrefetchAll([[maybe_unused]] std::uint32_t a_v) const;                                                                                                        // 142E13628
			virtual bool DoGetName(BSFixedString& a_dst) const;                                                                                                              // 142E13630
			virtual ErrorCode DoCreateAsync(BSTSmartPointer<AsyncStream>& a_streamOut) const;                                                                                // 142E13638
			virtual bool DoQTaggedPrioritizedReadSupported() const;                                                                                                          // 142E13640
			virtual ErrorCode DoCreateOp([[maybe_unused]] BSTSmartPointer<ICacheDriveOp>& p_op,[[maybe_unused]] char const* name) const;                                                                      // 142E13648
			virtual bool DoGetIsFromArchive() const;                                                                                                                         // 142E13650                            // 0B - { return ErrorCode::kUnsupported; }

			// non virtuals
			bool GetName(BSFixedString& a_dst) const;                                                                                                                                                  //0000000140004410
			ErrorCode Open(bool buffered, bool fullReadHint);                                                                                                                                    //000000014012A830
			ErrorCode Read(void* a_buffer, std::uint64_t a_toRead, std::uint64_t& a_read) const;                                                                                                       // 14012B2E0
			ErrorCode Seek(std::uint64_t a_toSeek, SeekMode a_mode, std::uint64_t& a_sought) const;                                                                                                    //00000001401B8160
			ErrorCode Write(const void* a_buffer, std::uint64_t a_toWrite, std::uint64_t& a_written) const;                                                                                            //00000001402D6200
			ErrorCode ReadAt(void* a_buffer, std::uint64_t a_offset, std::uint64_t a_toRead, std::uint64_t& a_read) const;                                                                             //000000014036DF60
			ErrorCode CreateOp(BSTSmartPointer<ICacheDriveOp>& a_opOut, const char* name) const;                                                                                                       //0000000140D0F080
			ErrorCode CreateAsync(BSTSmartPointer<AsyncStream>& a_streamOut) const;                                                                                                                    //0000000141AF97F0
			ErrorCode PrefetchAll(std::uint32_t a_v) const;                                                                                                                                            //0000000141B6EE50
			ErrorCode PrefetchAt(std::uint64_t a_v, std::uint64_t b_v, std::uint32_t c_v) const;                                                                                                       // 141B7A0A0
			std::uint32_t QFullReadHint() const;                                                                                                                                                       // 141B7A5B0
			bool QTaggedPrioritizedReadSupported() const;                                                                                                                                              // 141B7A780
			ErrorCode StartTaggedPrioritizedRead(void* buf, std::uint64_t a_v, std::uint64_t b_v, std::uint32_t c_V, std::uint32_t volatile* d_v, std::uint32_t& e_v, BSEventFlag* event_flag) const;  // 141B7BB30
			ErrorCode WaitTags(std::uint32_t volatile* a_v, std::uint32_t b_v, BSEventFlag* event_flag) const;                                                                                         // 141B7CAB0
			std::uint32_t QBuffered() const;                                                                                                                                                           // 141B87490
			ErrorCode SetEndOfStream();                                                                                                                                                                // 141B8C9F0
		};

		static_assert(sizeof(Stream) == 0x10);
	}
}
