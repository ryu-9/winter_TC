#include "ioJsonText.h"
#include "DxLib.h"
#include <fstream>


namespace iojson {
	constexpr auto EMPTY_STR = _T("");
	constexpr auto EMPTY_WSTR = L"";

	//!
	//! @fn std::wstring ScriptsData::ConvertUTF8ToWide(const std::string& utf8)
	//! @brief UTF-8 文字コードの std::string を std::wstring(UTF-16) に変換する
	//! @param[in] utf8 UTF-8 文字コードの std::string
	//! @return UTF-16 文字コードの std::wstring
	//!
	std::wstring ConvertUTF8ToWide(const std::string& utf8) {
		if (utf8.empty()) {
			return EMPTY_WSTR;
		}

		const auto in_length = static_cast<int>(utf8.length());
		const auto out_length = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), in_length, 0, 0);

		if (out_length <= 0) {
			return EMPTY_WSTR;
		}

		std::vector<wchar_t> buffer(out_length);

		MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), in_length, &(buffer[0]), out_length);

		std::wstring utf16(buffer.begin(), buffer.end());

		return utf16;
	}

	//!
	//! @fn std::string ScriptsData::ConvertWideToMultiByte(const std::wstring& utf16)
	//! @brief std::wstring(UTF-16) をマルチバイト文字コードの std::string に変換する
	//! @param[in] utf16 UTF-16 文字コードの std::wstring
	//! @return マルチバイト文字コードの std::string
	//!
	std::string ConvertWideToMultiByte(const std::wstring& utf16) {
		if (utf16.empty()) {
			return EMPTY_STR;
		}

		const auto in_length = static_cast<int>(utf16.length());
		const auto out_length = WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), in_length, 0, 0, 0, 0);

		if (out_length <= 0) {
			return EMPTY_STR;
		}

		std::vector<char> buffer(out_length);

		WideCharToMultiByte(CP_ACP, 0, utf16.c_str(), in_length, &(buffer[0]), out_length, 0, 0);

		std::string mbs(buffer.begin(), buffer.end());

		return mbs;
	}

	//!
	//! UTF-8 文字列をマルチバイト文字列に変換して DX ライブラリで扱えるようにする
	//!
	std::string ConvertString(const std::string& utf8) {
		const auto utf16 = ConvertUTF8ToWide(utf8);
		return ConvertWideToMultiByte(utf16);
	}

}