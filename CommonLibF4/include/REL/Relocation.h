#pragma once

#include "F4SE/Trampoline.h"

#include "REL/ID.h"
#include "REL/Module.h"
#include "REL/Offset.h"

#define REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER_IMPL(a_nopropQual, a_propQual, ...)              \
	template <                                                                                    \
		class R,                                                                                  \
		class Cls,                                                                                \
		class... Args>                                                                            \
	struct member_function_pod_type<R (Cls::*)(Args...) __VA_ARGS__ a_nopropQual a_propQual>      \
	{                                                                                             \
		using type = R(__VA_ARGS__ Cls*, Args...) a_propQual;                                     \
	};                                                                                            \
                                                                                                  \
	template <                                                                                    \
		class R,                                                                                  \
		class Cls,                                                                                \
		class... Args>                                                                            \
	struct member_function_pod_type<R (Cls::*)(Args..., ...) __VA_ARGS__ a_nopropQual a_propQual> \
	{                                                                                             \
		using type = R(__VA_ARGS__ Cls*, Args..., ...) a_propQual;                                \
	};

#define REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER(a_qualifer, ...)              \
	REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER_IMPL(a_qualifer, , ##__VA_ARGS__) \
	REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER_IMPL(a_qualifer, noexcept, ##__VA_ARGS__)

#define REL_MAKE_MEMBER_FUNCTION_POD_TYPE(...)                 \
	REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER(, __VA_ARGS__)    \
	REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER(&, ##__VA_ARGS__) \
	REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER(&&, ##__VA_ARGS__)

#define REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER_IMPL(a_nopropQual, a_propQual, ...)              \
	template <                                                                                        \
		class R,                                                                                      \
		class Cls,                                                                                    \
		class... Args>                                                                                \
	struct member_function_non_pod_type<R (Cls::*)(Args...) __VA_ARGS__ a_nopropQual a_propQual>      \
	{                                                                                                 \
		using type = R&(__VA_ARGS__ Cls*, void*, Args...)a_propQual;                                  \
	};                                                                                                \
                                                                                                      \
	template <                                                                                        \
		class R,                                                                                      \
		class Cls,                                                                                    \
		class... Args>                                                                                \
	struct member_function_non_pod_type<R (Cls::*)(Args..., ...) __VA_ARGS__ a_nopropQual a_propQual> \
	{                                                                                                 \
		using type = R&(__VA_ARGS__ Cls*, void*, Args..., ...)a_propQual;                             \
	};

#define REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER(a_qualifer, ...)              \
	REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER_IMPL(a_qualifer, , ##__VA_ARGS__) \
	REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER_IMPL(a_qualifer, noexcept, ##__VA_ARGS__)

#define REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE(...)                 \
	REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER(, __VA_ARGS__)    \
	REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER(&, ##__VA_ARGS__) \
	REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER(&&, ##__VA_ARGS__)

#if defined(FALLOUT_SUPPORT_F4) && (FALLOUT_SUPPORT_VR)
/**
 * Defined to indicate that this build supports both VR and non-VR runtimes.
 */
#	define FALLOUT_SUPPORT_CROSS 1
#endif

#if !defined(FALLOUT_SUPPORT_F4) && !defined(FALLOUT_SUPPORT_VR)
/**
 * A macro which defines a modifier for expressions that vary by FALLOUT Address Library IDs.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build only targets one family of Address Library.
 * </p>
 */
#	define FALLOUT_ADDR constexpr
#else
/**
 * A macro which defines a modifier for expressions that vary by FALLOUT address library IDs.
 *
 * <p>
 * Currently defined as <code>inline</code> to support multiple Address Library ID families dynamically.
 * </p>
 */
#	define FALLOUT_ADDR inline
#endif

#if !defined(FALLOUT_SUPPORT_F4) || (!defined(FALLOUT_SUPPORT_F4) && !defined(FALLOUT_SUPPORT_VR)) || !defined(FALLOUT_SUPPORT_VR)
/**
 * A macro which defines a modifier for expressions that vary by the specific FALLOUT runtime.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build is for only a single runtime.
 * </p>
 */
#	define FALLOUT_REL constexpr

/**
 * A macro which defines a modifier for expressions which may be <code>constexpr</code> when using selective targeting.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build is for only a single runtime.
 * </p>
 */
#	define FALLOUT_REL_CONSTEXPR constexpr
#else
/**
 * A macro which defines a modifier for expressions that vary by the specific FALLOUT runtime.
 *
 * <p>
 * Currently defined as <code>inline</code> to support multiple runtimes dynamically.
 * </p>
 */
#	define FALLOUT_REL inline

/**
 * A macro which defines a modifier for expressions which may be <code>constexpr</code> when using selective targeting.
 *
 * <p>
 * Currently defined as empty to support multiple runtimes.
 * </p>
 */
#	define FALLOUT_REL_CONSTEXPR
#endif

#ifndef FALLOUT_SUPPORT_CROSS
/**
 * A macro which defines a modifier for expressions that vary between FALLOUT 4 and FALLOUT VR.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build is only for VR or non-VR.
 * </p>
 */
#	define FALLOUT_REL_VR constexpr

/**
 * A macro which defines a modifier for expressions which may be <code>constexpr</code> when using selectively VR or non-VR.
 *
 * <p>
 * Currently defined as <code>constexpr</code> since this build is only for VR or non-VR.
 * </p>
 */
#	define FALLOUT_REL_VR_CONSTEXPR constexpr

/**
 * A macro which defines a modifier for functions which may be <code>virtual</code> when using selectively VR or non-VR.
 *
 * <p>
 * Currently defined as <code>virtual</code> since this build is only for VR or non-VR.
 * </p>
 */
#	define FALLOUT_REL_VR_VIRTUAL virtual
#else
/**
 * A macro which defines a modifier for expressions that vary between FALLOUT SE/AE and FALLOUT VR.
 *
 * <p>
 * Currently defined as <code>inline</code> since this build is for both VR and non-VR.
 * </p>
 */
#	define FALLOUT_REL_VR inline

/**
 * A macro which defines a modifier for expressions which may be <code>constexpr</code> when using selectively VR or non-VR.
 *
 * <p>
 * Currently defined as empty since this build is for both VR and non-VR.
 * </p>
 */
#	define FALLOUT_REL_VR_CONSTEXPR

/**
 * A macro which defines a modifier for functions which may be <code>virtual</code> when using selectively VR or non-VR.
 *
 * <p>
 * Currently defined as empty since this build is for both VR and non-VR.
 * </p>
 */
#	define FALLOUT_REL_VR_VIRTUAL
#endif

namespace REL
{
	template <class>
	class Relocation;

	namespace detail
	{
		class memory_map
		{
		public:
			memory_map() noexcept = default;

			memory_map(const memory_map&) = delete;

			memory_map(memory_map&& a_rhs) noexcept :
				_mapping(a_rhs._mapping),
				_view(a_rhs._view)
			{
				a_rhs._mapping = nullptr;
				a_rhs._view = nullptr;
			}

			~memory_map() { close(); }

			memory_map& operator=(const memory_map&) = delete;

			memory_map& operator=(memory_map&& a_rhs) noexcept
			{
				if (this != std::addressof(a_rhs)) {
					_mapping = a_rhs._mapping;
					a_rhs._mapping = nullptr;

					_view = a_rhs._view;
					a_rhs._view = nullptr;
				}
				return *this;
			}

			[[nodiscard]] void* data() noexcept { return _view; }

			bool open(stl::zwstring a_name, std::size_t a_size);

			bool create(stl::zwstring a_name, std::size_t a_size);

			void close();

		private:
			void* _mapping{ nullptr };
			void* _view{ nullptr };
		};

		template <class>
		struct member_function_pod_type;

		REL_MAKE_MEMBER_FUNCTION_POD_TYPE();
		REL_MAKE_MEMBER_FUNCTION_POD_TYPE(const);
		REL_MAKE_MEMBER_FUNCTION_POD_TYPE(volatile);
		REL_MAKE_MEMBER_FUNCTION_POD_TYPE(const volatile);

		template <class F>
		using member_function_pod_type_t = typename member_function_pod_type<F>::type;

		template <class>
		struct member_function_non_pod_type;

		REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE();
		REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE(const);
		REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE(volatile);
		REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE(const volatile);

		template <class F>
		using member_function_non_pod_type_t = typename member_function_non_pod_type<F>::type;

		// https://docs.microsoft.com/en-us/cpp/build/x64-calling-convention

		template <class T>
		struct meets_length_req :
			std::disjunction<
				std::bool_constant<sizeof(T) == 1>,
				std::bool_constant<sizeof(T) == 2>,
				std::bool_constant<sizeof(T) == 4>,
				std::bool_constant<sizeof(T) == 8>>
		{
		};

		template <class T>
		struct meets_function_req :
			std::conjunction<
				std::is_trivially_constructible<T>,
				std::is_trivially_destructible<T>,
				std::is_trivially_copy_assignable<T>,
				std::negation<
					std::is_polymorphic<T>>>
		{
		};

		template <class T>
		struct meets_member_req :
			std::is_standard_layout<T>
		{
		};

		template <class T, class = void>
		struct is_x64_pod :
			std::true_type
		{
		};

		template <class T>
		struct is_x64_pod<
			T,
			std::enable_if_t<
				std::is_union_v<T>>> :
			std::false_type
		{
		};

		template <class T>
		struct is_x64_pod<
			T,
			std::enable_if_t<
				std::is_class_v<T>>> :
			std::conjunction<
				meets_length_req<T>,
				meets_function_req<T>,
				meets_member_req<T>>
		{
		};

		template <class T>
		inline constexpr bool is_x64_pod_v = is_x64_pod<T>::value;

		template <
			class F,
			class First,
			class... Rest>
		decltype(auto) invoke_member_function_non_pod(F&& a_func, First&& a_first, Rest&&... a_rest)  //
			noexcept(std::is_nothrow_invocable_v<F, First, Rest...>)
		{
			using result_t = std::invoke_result_t<F, First, Rest...>;
			alignas(result_t) std::byte result[sizeof(result_t)]{};

			using func_t = member_function_non_pod_type_t<F>;
			auto func = stl::unrestricted_cast<func_t*>(std::forward<F>(a_func));

			return func(std::forward<First>(a_first), std::addressof(result), std::forward<Rest>(a_rest)...);
		}

		std::optional<std::string> sha512(std::span<const std::byte> a_data);
	}

	inline constexpr std::uint8_t NOP = 0x90;
	inline constexpr std::uint8_t RET = 0xC3;
	inline constexpr std::uint8_t INT3 = 0xCC;

	template <class F, class... Args>
	std::invoke_result_t<F, Args...> invoke(F&& a_func, Args&&... a_args)  //
		noexcept(std::is_nothrow_invocable_v<F, Args...>)                  //
		requires(std::invocable<F, Args...>)
	{
		if constexpr (std::is_member_function_pointer_v<std::decay_t<F>>) {
			if constexpr (detail::is_x64_pod_v<std::invoke_result_t<F, Args...>>) {  // member functions == free functions in x64
				using func_t = detail::member_function_pod_type_t<std::decay_t<F>>;
				auto func = stl::unrestricted_cast<func_t*>(std::forward<F>(a_func));
				return func(std::forward<Args>(a_args)...);
			} else {  // shift args to insert result
				return detail::invoke_member_function_non_pod(std::forward<F>(a_func), std::forward<Args>(a_args)...);
			}
		} else {
			return std::forward<F>(a_func)(std::forward<Args>(a_args)...);
		}
	}

	void safe_write(std::uintptr_t a_dst, const void* a_src, std::size_t a_count);

	template <std::integral T>
	void safe_write(std::uintptr_t a_dst, const T& a_data)
	{
		safe_write(a_dst, std::addressof(a_data), sizeof(T));
	}

	template <class T>
	void safe_write(std::uintptr_t a_dst, std::span<T> a_data)
	{
		safe_write(a_dst, a_data.data(), a_data.size_bytes());
	}

	void safe_fill(std::uintptr_t a_dst, std::uint8_t a_value, std::size_t a_count);

	template <class T = std::uintptr_t>
	class Relocation
	{
	public:
		using value_type =
			std::conditional_t<
				std::is_member_pointer_v<T> || std::is_function_v<std::remove_pointer_t<T>>,
				std::decay_t<T>,
				T>;

		constexpr Relocation() noexcept = default;

		explicit constexpr Relocation(std::uintptr_t a_address) noexcept :
			_impl{ a_address } {}

		explicit Relocation(Offset a_offset) :
			_impl{ a_offset.address() } {}

		explicit Relocation(Offset2 a_offset) :
			_impl{ a_offset.address() } {}

		explicit Relocation(ID a_id) :
			_impl{ a_id.address() } {}

		explicit Relocation(ID a_id, std::ptrdiff_t a_offset) :
			_impl{ a_id.address() + a_offset } {}

		explicit Relocation(ID a_id, Offset a_offset) :
			_impl{ a_id.address() + a_offset.offset() } {}

		explicit Relocation(ID a_id, Offset2 a_offset) :
			_impl{ a_id.address() + a_offset.offset() } {}

		explicit Relocation(ID2 a_id) :
			_impl{ a_id.address() } {}

		explicit Relocation(ID2 a_id, std::ptrdiff_t a_offset) :
			_impl{ a_id.address() + a_offset } {}

		explicit Relocation(ID2 a_id, Offset a_offset) :
			_impl{ a_id.address() + a_offset.offset() } {}

		explicit Relocation(ID2 a_id, Offset2 a_offset) :
			_impl{ a_id.address() + a_offset.offset() } {}

		constexpr Relocation& operator=(std::uintptr_t a_address) noexcept
		{
			_impl = a_address;
			return *this;
		}

		Relocation& operator=(Offset a_offset)
		{
			_impl = a_offset.address();
			return *this;
		}

		Relocation& operator=(Offset2 a_offset)
		{
			_impl = a_offset.address();
			return *this;
		}

		Relocation& operator=(ID a_id)
		{
			_impl = a_id.address();
			return *this;
		}

		Relocation& operator=(ID2 a_id)
		{
			_impl = a_id.address();
			return *this;
		}

		template <class U = value_type>
		[[nodiscard]] decltype(auto) operator*() const noexcept
			requires(std::is_pointer_v<U>)
		{
			return *get();
		}

		template <class U = value_type>
		[[nodiscard]] auto operator->() const noexcept
			requires(std::is_pointer_v<U>)
		{
			return get();
		}

		template <class... Args>
		std::invoke_result_t<const value_type&, Args...> operator()(Args&&... a_args) const
			noexcept(std::is_nothrow_invocable_v<const value_type&, Args...>)
			requires(std::invocable<const value_type&, Args...>)
		{
			return REL::invoke(get(), std::forward<Args>(a_args)...);
		}

		[[nodiscard]] constexpr std::uintptr_t address() const noexcept { return _impl; }
		[[nodiscard]] std::size_t              offset() const { return _impl - base(); }

		[[nodiscard]] value_type get() const
			noexcept(std::is_nothrow_copy_constructible_v<value_type>)
		{
			assert(_impl != 0);
			return stl::unrestricted_cast<value_type>(_impl);
		}

		void write(const void* a_src, std::size_t a_count)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			safe_write(address(), a_src, a_count);
		}

		template <std::integral U>
		void write(const U& a_data)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			safe_write(address(), std::addressof(a_data), sizeof(U));
		}

		void write(const std::initializer_list<std::uint8_t> a_data)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			safe_write(address(), a_data.begin(), a_data.size());
		}

		template <class U>
		void write(const std::span<U> a_data)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			safe_write(address(), a_data.data(), a_data.size_bytes());
		}

		template <std::size_t N>
		std::uintptr_t write_branch(const std::uintptr_t a_dst)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			return F4SE::GetTrampoline().write_branch<N>(address(), a_dst);
		}

		template <std::size_t N, class F>
		std::uintptr_t write_branch(const F a_dst)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			return F4SE::GetTrampoline().write_branch<N>(address(), stl::unrestricted_cast<std::uintptr_t>(a_dst));
		}

		template <std::size_t N>
		std::uintptr_t write_call(const std::uintptr_t a_dst)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			return F4SE::GetTrampoline().write_call<N>(address(), a_dst);
		}

		template <std::size_t N, class F>
		std::uintptr_t write_call(const F a_dst)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			return F4SE::GetTrampoline().write_call<N>(address(), stl::unrestricted_cast<std::uintptr_t>(a_dst));
		}

		void write_fill(const std::uint8_t a_value, const std::size_t a_count)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			safe_fill(address(), a_value, a_count);
		}

		template <class U = value_type>
		std::uintptr_t write_vfunc(std::size_t a_idx, std::uintptr_t a_newFunc)
			requires(std::same_as<U, std::uintptr_t>)
		{
			const auto addr = address() + (sizeof(void*) * a_idx);
			const auto result = *reinterpret_cast<std::uintptr_t*>(addr);
			safe_write(addr, a_newFunc);
			return result;
		}

		template <class F>
		std::uintptr_t write_vfunc(std::size_t a_idx, F a_newFunc)
			requires(std::same_as<value_type, std::uintptr_t>)
		{
			return write_vfunc(a_idx, stl::unrestricted_cast<std::uintptr_t>(a_newFunc));
		}

	private:
		// clang-format off
        [[nodiscard]] static std::uintptr_t base() { return Module::get().base(); }
		// clang-format on

		std::uintptr_t _impl{ 0 };
	};

	namespace detail
	{
		namespace characters
		{
			[[nodiscard]] constexpr bool hexadecimal(char a_ch) noexcept
			{
				return ('0' <= a_ch && a_ch <= '9') ||
				       ('A' <= a_ch && a_ch <= 'F') ||
				       ('a' <= a_ch && a_ch <= 'f');
			}

			[[nodiscard]] constexpr bool space(char a_ch) noexcept
			{
				return a_ch == ' ';
			}

			[[nodiscard]] constexpr bool wildcard(char a_ch) noexcept
			{
				return a_ch == '?';
			}
		}

		namespace rules
		{
			namespace detail
			{
				[[nodiscard]] constexpr std::byte hexacharacters_to_hexadecimal(char a_hi, char a_lo) noexcept
				{
					constexpr auto lut = []() noexcept {
						std::array<std::uint8_t, (std::numeric_limits<unsigned char>::max)() + 1> a = {};

						const auto iterate = [&](std::uint8_t a_iFirst, unsigned char a_cFirst,
												 unsigned char a_cLast) noexcept {
							for (; a_cFirst <= a_cLast; ++a_cFirst, ++a_iFirst) {
								a[a_cFirst] = a_iFirst;
							}
						};

						iterate(0, '0', '9');
						iterate(0xA, 'A', 'F');
						iterate(0xa, 'a', 'f');

						return a;
					}();

					return static_cast<std::byte>(
						lut[static_cast<unsigned char>(a_hi)] * 0x10u +
						lut[static_cast<unsigned char>(a_lo)]);
				}
			}

			template <char HI, char LO>
			class Hexadecimal
			{
			public:
				[[nodiscard]] static constexpr bool match(std::byte a_byte) noexcept
				{
					constexpr auto expected = detail::hexacharacters_to_hexadecimal(HI, LO);
					return a_byte == expected;
				}
			};

			static_assert(Hexadecimal<'5', '7'>::match(std::byte{ 0x57 }));
			static_assert(Hexadecimal<'6', '5'>::match(std::byte{ 0x65 }));
			static_assert(Hexadecimal<'B', 'D'>::match(std::byte{ 0xBD }));
			static_assert(Hexadecimal<'1', 'C'>::match(std::byte{ 0x1C }));
			static_assert(Hexadecimal<'F', '2'>::match(std::byte{ 0xF2 }));
			static_assert(Hexadecimal<'9', 'f'>::match(std::byte{ 0x9f }));

			static_assert(!Hexadecimal<'D', '4'>::match(std::byte{ 0xF8 }));
			static_assert(!Hexadecimal<'6', '7'>::match(std::byte{ 0xAA }));
			static_assert(!Hexadecimal<'7', '8'>::match(std::byte{ 0xE3 }));
			static_assert(!Hexadecimal<'6', 'E'>::match(std::byte{ 0x61 }));

			class Wildcard
			{
			public:
				[[nodiscard]] static constexpr bool match(std::byte) noexcept
				{
					return true;
				}
			};

			static_assert(Wildcard::match(std::byte{ 0xB9 }));
			static_assert(Wildcard::match(std::byte{ 0x96 }));
			static_assert(Wildcard::match(std::byte{ 0x35 }));
			static_assert(Wildcard::match(std::byte{ 0xE4 }));

			template <char, char>
			void rule_for() noexcept;

			template <char C1, char C2>
			Hexadecimal<C1, C2>
				rule_for() noexcept
				requires(characters::hexadecimal(C1) && characters::hexadecimal(C2));

			template <char C1, char C2>
			Wildcard rule_for() noexcept
				requires(characters::wildcard(C1) && characters::wildcard(C2));
		}

		template <class... Rules>
		class PatternMatcher
		{
		public:
			static_assert(sizeof...(Rules) >= 1, "must provide at least 1 rule for the pattern matcher");

			[[nodiscard]] constexpr bool match(std::span<const std::byte, sizeof...(Rules)> a_bytes) const noexcept
			{
				std::size_t i = 0;
				return (Rules::match(a_bytes[i++]) && ...);
			}

			[[nodiscard]] bool match(std::uintptr_t a_address) const noexcept
			{
				return this->match(*reinterpret_cast<const std::byte(*)[sizeof...(Rules)]>(a_address));
			}

			void match_or_fail(std::uintptr_t a_address,
				F4SE::stl::source_location    a_loc = F4SE::stl::source_location::current()) const noexcept
			{
				if (!this->match(a_address)) {
					const auto version = Module::get().version();
					stl::report_and_fail(
						std::format(
							"A pattern has failed to match.\n"
							"This means the plugin is incompatible with the current version of the game ({}.{}.{}). "
							"Head to the mod page of this plugin to see if an update is available."sv,
							version[0],
							version[1],
							version[2]),
						a_loc);
				}
			}
		};

		void consteval_error(const char* a_error);

		template <stl::nttp::string S, class... Rules>
		[[nodiscard]] constexpr auto do_make_pattern() noexcept
		{
			if constexpr (S.length() == 0) {
				return PatternMatcher<Rules...>();
			} else if constexpr (S.length() == 1) {
				constexpr char c = S[0];
				if constexpr (characters::hexadecimal(c) || characters::wildcard(c)) {
					consteval_error(
						"the given pattern has an unpaired rule (rules are required to be written in pairs of 2)");
				} else {
					consteval_error("the given pattern has trailing characters at the end (which is not allowed)");
				}
			} else {
				using rule_t = decltype(rules::rule_for<S[0], S[1]>());
				if constexpr (std::same_as<rule_t, void>) {
					consteval_error("the given pattern failed to match any known rules");
				} else {
					if constexpr (S.length() <= 3) {
						return do_make_pattern<S.template substr<2>(), Rules..., rule_t>();
					} else if constexpr (characters::space(S.value_at(2))) {
						return do_make_pattern<S.template substr<3>(), Rules..., rule_t>();
					} else {
						consteval_error("a space character is required to split byte patterns");
					}
				}
			}
		}

		template <class... Bytes>
		[[nodiscard]] consteval auto make_byte_array(Bytes... a_bytes) noexcept
			-> std::array<std::byte, sizeof...(Bytes)>
		{
			static_assert((std::integral<Bytes> && ...), "all bytes must be an integral type");
			return { static_cast<std::byte>(a_bytes)... };
		}
	}

	template <stl::nttp::string S>
	[[nodiscard]] constexpr auto make_pattern() noexcept
	{
		return detail::do_make_pattern<S>();
	}

	static_assert(make_pattern<"40 10 F2 ??">().match(
		detail::make_byte_array(0x40, 0x10, 0xF2, 0x41)));
	static_assert(make_pattern<"B8 D0 ?? ?? D4 6E">().match(
		detail::make_byte_array(0xB8, 0xD0, 0x35, 0x2A, 0xD4, 0x6E)));

	/**
     * Return the correct value of two choices between VR and F4 versions of FALLOUT.
     *
     * <p>
     * This is commonly used to select between relative offsets within a function, when hooking a call instruction.
     * In such cases the function can be identified by its Address Library ID, but the offset within the function
     * may vary between FALLOUT versions. This selection is made at runtime, allowing the same compiled code to run
     * in multiple versions of FALLOUT.
     * </p>
     *
     * @tparam T the type of value to return.
     * @param a_f4AndVR the value to use for SE and VR.
     * @return Either <code>a_f4AndVR</code> if the current runtime is FALLOUT SE or VR, or <code>a_ae</code> if the runtime is AE.
     */
	template <class T>
	[[nodiscard]] FALLOUT_ADDR T Relocate([[maybe_unused]] T&& a_f4AndVR) noexcept
	{
		a_f4AndVR;
	}

	/**
     * Return the correct value of two choices between F4 and VR versions of FALLOUT.
     *
     * <p>
     * This is commonly used to select between relative offsets within a function, when hooking a call instruction.
     * In such cases the function can be identified by its Address Library ID, but the offset within the function
     * may vary between FALLOUT versions. This selection is made at runtime, allowing the same compiled code to run
     * in multiple versions of FALLOUT.
     * </p>
     *
     * @tparam T the type of value to return.
     * @param a_f4 the value to use for F4.
     * @param a_vr the value to use for VR.
     * @return Either <code>a_f4</code> if the current runtime is F4, or <code>a_vr</code> if the runtime is VR
     */
	template <class T>
	[[nodiscard]] FALLOUT_REL T Relocate([[maybe_unused]] T a_f4, [[maybe_unused]] T a_vr) noexcept
	{
#if !defined(FALLOUT_SUPPORT_VR)
		return a_f4;
#elif !defined(FALLOUT_SUPPORT_F4)
		return a_vr;
#else
		switch (Module::get().GetRuntime()) {
		case Module::Runtime::VR:
			return a_vr;
		default:
			return a_f4;
		}
#endif
	}

	namespace detail
	{
		template <class T>
		struct RelocateVirtualHelper
		{
		};

		template <class Ret, class This>
		struct RelocateVirtualHelper<Ret(This*)>
		{
			using this_type = This;
			using return_type = Ret;
			using function_type = Ret(This*);
		};

		template <class Ret, class This, class... Args>
		struct RelocateVirtualHelper<Ret(This*, Args...)>
		{
			using this_type = This;
			using return_type = Ret;
			using function_type = Ret(This*, Args...);
		};

		template <class Ret, class This>
		struct RelocateVirtualHelper<Ret (This::*)()>
		{
			using this_type = This;
			using return_type = Ret;
			using function_type = Ret(This*);
		};

		template <class Ret, class This, class... Args>
		struct RelocateVirtualHelper<Ret (This::*)(Args...)>
		{
			using this_type = This;
			using return_type = Ret;
			using function_type = Ret(This*, Args...);
		};

		template <class Ret, class This>
		struct RelocateVirtualHelper<Ret (This::*)() const>
		{
			using this_type = const This;
			using return_type = Ret;
			using function_type = Ret(const This*);
		};

		template <class Ret, class This, class... Args>
		struct RelocateVirtualHelper<Ret (This::*)(Args...) const>
		{
			using this_type = const This;
			using return_type = Ret;
			using function_type = Ret(const This*, Args...);
		};
	}

	/**
     * Invokes a virtual function in a cross-platform way where the vtable structure is variant across AE/SE and VR runtimes.
     *
     * <p>
     * Some classes in FALLOUT VR add new virtual functions in the middle of the vtable structure, which makes it ABI-incompatible with F4.
     * A naive virtual function call, therefore, cannot work across all runtimes without the plugin being recompiled specifically for VR.
     * This call works with types which have variant vtables to allow a non-virtual function definition to be created in the virtual function's
     * place, and to have that call dynamically lookup the correct function based on the vtable structure expected in the current runtime.
     * </p>
     *
     * @tparam Fn the type of the function being called.
     * @tparam Args the types of the arguments being passed.
     * @param a_f4tableOffset the offset from the <code>this</code> pointer to the vtable with the virtual function in F4.
     * @param a_vrVtableIndex the offset from the <code>this</code> pointer to the vtable with the virtual function in VR.
     * @param a_f4tableIndex the index of the function in the class' vtable in F4.
     * @param a_vrVtableIndex the index of the function in the class' vtable in VR.
     * @param a_f4lf the <code>this</code> argument for the call.
     * @param a_args the remaining arguments for the call, if any.
     * @return The result of the function call.
     */
	template <class Fn, class... Args>
	[[nodiscard]] inline typename detail::RelocateVirtualHelper<Fn>::return_type RelocateVirtual(
		[[maybe_unused]] std::ptrdiff_t a_f4tableOffset, [[maybe_unused]] std::ptrdiff_t a_vrVtableOffset,
		[[maybe_unused]] std::ptrdiff_t a_f4tableIndex, [[maybe_unused]] std::ptrdiff_t a_vrVtableIndex,
		typename detail::RelocateVirtualHelper<Fn>::this_type* a_f4lf, Args&&... a_args)
	{
		return (*reinterpret_cast<typename detail::RelocateVirtualHelper<Fn>::function_type**>(
			*reinterpret_cast<const uintptr_t*>(reinterpret_cast<uintptr_t>(a_f4lf) +
#if !defined(FALLOUT_SUPPORT_VR)
												a_f4tableOffset) +
			a_f4tableIndex
#elif !defined(FALLOUT_SUPPORT_F4)
												a_vrVtableOffset) +
			a_vrVtableIndex
#else
												(Module::IsVR() ? a_vrVtableOffset : a_f4tableOffset)) +
			(Module::IsVR() ? a_vrVtableIndex : a_f4tableIndex)
#endif
				* sizeof(uintptr_t)))(a_f4lf, std::forward<Args>(a_args)...);
	}

	/**
     * Invokes a virtual function in a cross-platform way where the vtable structure is variant across AE/SE and VR runtimes.
     *
     * <p>
     * Some classes in FALLOUT VR add new virtual functions in the middle of the vtable structure, which makes it ABI-incompatible with F4.
     * A naive virtual function call, therefore, cannot work across all runtimes without the plugin being recompiled specifically for VR.
     * This call works with types which have variant vtables to allow a non-virtual function definition to be created in the virtual function's
     * place, and to have that call dynamically lookup the correct function based on the vtable structure expected in the current runtime.
     * </p>
     *
     * <p>
     * This call assumes the vtable to be used is the one at offset 0, i.e. it invokes a virtual function either on the first parent class
     * or the current class.
     * </p>
     *
     * @tparam Fn the type of the function being called.
     * @tparam Args the types of the arguments being passed.
     * @param a_f4tableIndex the index of the function in the class' vtable in F4.
     * @param a_vrVtableIndex the index of the function in the class' vtable in VR.
     * @param a_f4lf the <code>this</code> argument for the call.
     * @param a_args the remaining arguments for the call, if any.
     * @return The result of the function call.
     */
	template <class Fn, class... Args>
	[[nodiscard]] inline typename detail::RelocateVirtualHelper<Fn>::return_type RelocateVirtual(
		std::ptrdiff_t a_f4tableIndex, std::ptrdiff_t a_vrVtableIndex,
		typename detail::RelocateVirtualHelper<Fn>::this_type* a_f4lf, Args&&... a_args)
	{
		return RelocateVirtual<Fn, Args...>(0, 0, a_f4tableIndex, a_vrVtableIndex, a_f4lf,
			std::forward<Args>(a_args)...);
	}

	/**
     * Gets a member variable in a cross-platform way, using runtime-specific memory offsets.
     *
     * <p>
     * This function handles the variant memory structures used in FALLOUT VR as compared to versions of SE.
     * It allows a memory offset relative to the object's base address for SE (and AE) and a separate one for
     * VR. This simplifies the process of creating functions to get member variables that are at different
     * offsets in different runtimes from a single build.
     * </p>
     *
     * @tparam T the type of the member being accessed.
     * @tparam This the type of the target object that has the member.
     * @param a_f4lf the target object that has the member.
     * @param a_f4 the memory offset of the member in FALLOUT F4.
     * @param a_vr the memory offset of the member in FALLOUT VR.
     * @return A reference to the member.
     */
	template <class T, class This>
	[[nodiscard]] inline T& RelocateMember(This* a_f4lf, std::ptrdiff_t a_f4, std::ptrdiff_t a_vr)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(a_f4lf) + Relocate(a_f4, a_vr));
	}

	template <class T, class This>
	[[nodiscard]] inline T& RelocateMember(This* a_f4lf, std::ptrdiff_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(a_f4lf) + offset);
	}

	template <class T, class This>
	[[nodiscard]] inline T& RelocateMemberIf(bool condition, This* a_f4lf, std::ptrdiff_t a, std::ptrdiff_t b)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(a_f4lf) + (condition ? a : b));
	}

	template <class T, class This>
	[[nodiscard]] inline T& RelocateMemberIfNewer(Version v, This* a_f4lf, std::ptrdiff_t older, std::ptrdiff_t newer)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(a_f4lf) +
									 (REL::Module::get().version().compare(v) == std::strong_ordering::less ? older : newer));
	}
}

namespace std
{
	[[nodiscard]] inline std::string to_string(REL::Version a_version)
	{
		return a_version.string("."sv);
	}

#ifdef __cpp_lib_format

	template <class CharT>
	struct formatter<REL::Version, CharT> : formatter<std::string, CharT>
	{
		template <class FormatContext>
		auto format(const REL::Version& a_version, FormatContext& a_ctx)
		{
			return formatter<std::string, CharT>::format(to_string(a_version), a_ctx);
		}
	};

#endif
}

#ifdef FMT_VERSION
namespace fmt
{
	template <class CharT>
	struct formatter<REL::Version, CharT> : formatter<std::string, CharT>
	{
		template <class FormatContext>
		auto format(const REL::Version& a_version, FormatContext& a_ctx)
		{
			return formatter<std::string, CharT>::format(std::to_string(a_version), a_ctx);
		}
	};
}
#endif

#undef REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE
#undef REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER
#undef REL_MAKE_MEMBER_FUNCTION_NON_POD_TYPE_HELPER_IMPL

#undef REL_MAKE_MEMBER_FUNCTION_POD_TYPE
#undef REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER
#undef REL_MAKE_MEMBER_FUNCTION_POD_TYPE_HELPER_IMPL
