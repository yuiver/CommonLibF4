#pragma once

namespace REL
{
	class IDDB
	{
	private:
		struct mapping_t
		{
			std::uint64_t id;
			std::uint64_t offset;
		};

	public:
		IDDB(const IDDB&) = delete;
		IDDB(IDDB&&) = delete;

		IDDB& operator=(const IDDB&) = delete;
		IDDB& operator=(IDDB&&) = delete;

		[[nodiscard]] static IDDB& get()
		{
			static IDDB singleton;
			return singleton;
		}

		[[nodiscard]] std::size_t id2offset(std::uint64_t a_id) const
		{
			if (_id2offset.empty()) {
				stl::report_and_fail("data is empty"sv);
			}

			const mapping_t elem{ a_id, 0 };
			const auto      it = std::lower_bound(
					 _id2offset.begin(),
					 _id2offset.end(),
					 elem,
					 [](auto&& a_lhs, auto&& a_rhs) {
                    return a_lhs.id < a_rhs.id;
                });
			if (it == _id2offset.end()) {
				stl::report_and_fail("id not found"sv);
			}

			return static_cast<std::size_t>(it->offset);
		}

	protected:
		friend class Offset2ID;

		[[nodiscard]] std::span<const mapping_t> get_id2offset() const noexcept { return _id2offset; }

	private:
		IDDB();
		~IDDB() = default;

		mmio::mapped_file_source   _mmap;
		std::span<const mapping_t> _id2offset;
	};
}
