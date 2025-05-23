cmake_minimum_required(VERSION 3.21)

option(F4SE_SUPPORT_XBYAK "Enables trampoline support for Xbyak." OFF)
option(ENABLE_FALLOUT_F4 "Enable support for Fallout 4 in the dynamic runtime feature." ON)
option(ENABLE_FALLOUT_NG "Enable support for Nextgen Update Fallout 4 in the dynamic runtime feature." ON)
option(ENABLE_FALLOUT_VR "Enable support for Fallout 4 VR in the dynamic runtime feature." ON)
option(BUILD_TESTS "Enable building of the unit tests." OFF)
message("Options:")
message("\tEnable Xbyak: ${F4SE_SUPPORT_XBYAK}")
message("\tEnable Fallout 4: ${ENABLE_FALLOUT_F4}")
message("\tEnable Nextgen Fallout 4: ${ENABLE_FALLOUT_NG}")
message("\tEnable Fallout 4 VR: ${ENABLE_FALLOUT_VR}")
message("\tBuild Tests: ${BUILD_TESTS}")

if(NOT ENABLE_FALLOUT_F4 AND NOT ENABLE_FALLOUT_NG AND NOT ENABLE_FALLOUT_VR)
	message(FATAL_ERROR "At least one Fallout runtime must be supported by the COMMONLIBF4 build.")
endif()

project(
	CommonLibF4
	LANGUAGES CXX
	VERSION 1.0.0
)

include(CheckIPOSupported)
include(GNUInstallDirs)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "in-source builds are not allowed")
endif()

find_path(RAPIDCSV_INCLUDE_DIRS "rapidcsv.h")
find_package(mmio REQUIRED CONFIG)
find_package(spdlog REQUIRED CONFIG)
find_package(mmio REQUIRED CONFIG)

include(cmake/sourcelist.cmake)

# include(cmake/testlist.cmake)
source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES ${SOURCES}
)

check_ipo_supported(RESULT USE_IPO OUTPUT IPO_OUTPUT)

if(USE_IPO)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION "$<$<CONFIG:RELEASE>:ON>")
endif()

add_library(
	"${PROJECT_NAME}"
	STATIC
	${SOURCES}
	../.clang-format
	CommonLibF4.natvis
)

add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_compile_definitions(
	"${PROJECT_NAME}"
	PUBLIC
	WINVER=0x0601 # windows 7, minimum supported version by fallout 4
	_WIN32_WINNT=0x0601
	"$<$<BOOL:${F4SE_SUPPORT_XBYAK}>:F4SE_SUPPORT_XBYAK=1>"
	"$<$<BOOL:${ENABLE_FALLOUT_F4}>:ENABLE_FALLOUT_F4=1>"
	"$<$<BOOL:${ENABLE_FALLOUT_NG}>:ENABLE_FALLOUT_NG=1>"
	"$<$<BOOL:${ENABLE_FALLOUT_VR}>:ENABLE_FALLOUT_VR=1>"
)

target_compile_features(
	"${PROJECT_NAME}"
	PUBLIC
	cxx_std_23
)

if(MSVC)
	target_compile_options(
		"${PROJECT_NAME}"
		PUBLIC

		# warnings -> errors
		/we4715 # 'function' : not all control paths return a value

		# disable warnings
		/wd4005 # macro redefinition
		/wd4061 # enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
		/wd4200 # nonstandard extension used : zero-sized array in struct/union
		/wd4201 # nonstandard extension used : nameless struct/union
		/wd4265 # 'type': class has virtual functions, but its non-trivial destructor is not virtual; instances of this class may not be destructed correctly
		/wd4266 # 'function' : no override available for virtual member function from base 'type'; function is hidden
		/wd4324 # 'struct_name' : structure was padded due to __declspec(align())
		/wd4371 # 'classname': layout of class may have changed from a previous version of the compiler due to better packing of member 'member'
		/wd4514 # 'function' : unreferenced inline function has been removed
		/wd4582 # 'type': constructor is not implicitly called
		/wd4583 # 'type': destructor is not implicitly called
		/wd4623 # 'derived class' : default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
		/wd4625 # 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
		/wd4626 # 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
		/wd4686 # 'user-defined type' : possible change in behavior, change in UDT return calling convention
		/wd4702 # unreachable code
		/wd4710 # 'function' : function not inlined
		/wd4711 # function 'function' selected for inline expansion
		/wd4820 # 'bytes' bytes padding added after construct 'member_name'
		/wd5026 # 'type': move constructor was implicitly defined as deleted
		/wd5027 # 'type': move assignment operator was implicitly defined as deleted
		/wd5045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
		/wd5053 # support for 'explicit(<expr>)' in C++17 and earlier is a vendor extension
		/wd5204 # 'type-name': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
		/wd5220 # 'member': a non-static data member with a volatile qualified type no longer implies that compiler generated copy / move constructors and copy / move assignment operators are not trivial
	)
endif()

target_include_directories(
	"${PROJECT_NAME}"
	PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
	${RAPIDCSV_INCLUDE_DIRS}
)

target_compile_definitions(
	"${PROJECT_NAME}"
	PUBLIC
)

target_link_libraries(
	"${PROJECT_NAME}"
	PUBLIC
	advapi32.lib
	bcrypt.lib
	d3d11.lib
	d3dcompiler.lib
	dbghelp.lib
	dxgi.lib
	mmio::mmio
	ole32.lib
	spdlog::spdlog
	version.lib
)

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
	include/F4SE/Impl/PCH.h
)

install(
	TARGETS "${PROJECT_NAME}"
	EXPORT "${PROJECT_NAME}-targets"
)

install(
	EXPORT "${PROJECT_NAME}-targets"
	NAMESPACE "${PROJECT_NAME}::"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

configure_file(
	cmake/config.cmake.in
	"${PROJECT_NAME}Config.cmake"
	@ONLY
)

install(
	FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
	DIRECTORY
 	"include/F4SE"
	"include/RE"
	"include/REL"
	"include/REX"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

if(BUILD_TESTS)
	find_package(Catch2 CONFIG REQUIRED)
	include(CTest)
	include(Catch)

	add_executable(
		"${PROJECT_NAME}Tests"
		${TESTS}
	)

	target_include_directories(
		"${PROJECT_NAME}Tests"
		PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
	)

	target_include_directories(
		"${PROJECT_NAME}Tests"
		PRIVATE
		${Catch2_INCLUDE_DIRS}
		${fmt_INCLUDE_DIRS}
		${spdlog_INCLUDE_DIRS}
		${RAPIDCSV_INCLUDE_DIRS}
	)

	target_compile_definitions(
		"${PROJECT_NAME}Tests"
		PUBLIC
		COMMONLIBF4_ENABLE_TESTING=1
	)

	target_link_libraries(
		"${PROJECT_NAME}Tests"
		PRIVATE
		${PROJECT_NAME}
		Catch2::Catch2
	)

	target_precompile_headers(
		"${PROJECT_NAME}Tests"
		PRIVATE
		include/F4SE/Impl/PCH.h
	)

	file(COPY
		tests/REL/version-1-2-72-0.csv
		DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Data/F4SE/Plugins/"
	)

	catch_discover_tests("${PROJECT_NAME}Tests")
	add_test(NAME "${PROJECT_NAME}Tests" COMMAND "${PROJECT_NAME}Tests")
endif()
