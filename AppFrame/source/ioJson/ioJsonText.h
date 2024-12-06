#pragma once
#include <string>
#include <vector>
#include <map>
#include "nlohmann/json.hpp"

namespace iojson {
	std::wstring ConvertUTF8ToWide(const std::string& utf8);
	std::string ConvertWideToMultiByte(const std::wstring& utf16);
	std::string ConvertString(const std::string& utf8);
}