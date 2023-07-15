cmake_minimum_required(VERSION 3.21)
include("cmake/common.cmake")
option(F4SE_SUPPORT_XBYAK "Enables trampoline support for Xbyak." OFF)

project(umbrella)

if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
	message(FATAL_ERROR "in-source builds are not allowed")
endif()

function(conditionally_add_subdirectory SRC_DIR)
	if(NOT TARGET "${SUBDIRECTORY}")
		if("${ARGC}" GREATER 1)
			add_subdirectory("${SRC_DIR}" "${ARGV1}")
		else()
			add_subdirectory("${SRC_DIR}")
		endif()
	endif()
endfunction()

include(CTest)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

conditionally_add_subdirectory(AddressLibDecoder)
conditionally_add_subdirectory(AddressLibGen)
conditionally_add_subdirectory(CommonLibF4)
conditionally_add_subdirectory(ExampleProject)
conditionally_add_subdirectory("F4SEStub/runtime" f4se_runtime)
conditionally_add_subdirectory("F4SEStub/loader" f4se_loader)
conditionally_add_subdirectory("F4SEStub/steam_loader" f4se_steam_loader)
conditionally_add_subdirectory(RTTIDump)
conditionally_add_subdirectory(Tests)

# https://gitlab.kitware.com/cmake/cmake/-/issues/24922#note_1371990
if(MSVC_VERSION GREATER_EQUAL 1936 AND MSVC_IDE) # 17.6+
	# When using /std:c++latest, "Build ISO C++23 Standard Library Modules" defaults to "Yes".
	# Default to "No" instead.
	#
	# As of CMake 3.26.4, there isn't a way to control this property
	# (https://gitlab.kitware.com/cmake/cmake/-/issues/24922),
	# We'll use the MSBuild project system instead
	# (https://learn.microsoft.com/en-us/cpp/build/reference/vcxproj-file-structure)
	file(CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/Directory.Build.props" CONTENT [==[
<Project>
  <ItemDefinitionGroup>
    <ClCompile>
      <BuildStlModules>false</BuildStlModules>
    </ClCompile>
  </ItemDefinitionGroup>
</Project>
]==] @ONLY)
endif()