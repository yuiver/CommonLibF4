#pragma once

#include "F4SE/F4SE.h"
#include "RE/Fallout.h"
#include "REX/W32/DBGHELP.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <memory>
#include <regex>
#include <span>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#define DLLEXPORT extern "C" [[maybe_unused]] __declspec(dllexport)

namespace logger = F4SE::log;
namespace stl = F4SE::stl;
using namespace std::literals;
