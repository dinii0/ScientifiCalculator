#include "framework.h"
#include "ConvertData.h"

#include <sstream>
#include <vector>

LPCWSTR convert_string_to_lpcwstr(const std::string& index_name) {
    int length = MultiByteToWideChar(CP_UTF8, 0, index_name.c_str(), -1, nullptr, 0);
    wchar_t* wideString = new wchar_t[length];
    MultiByteToWideChar(CP_UTF8, 0, index_name.c_str(), -1, wideString, length);

    LPCWSTR lpcwStr = wideString;
    return lpcwStr;
}
TCHAR* convert_string_to_tchar(const std::string& string_name) {
    int length = string_name.length();
    TCHAR* tchar_string = new TCHAR[length + 1];
    std::copy(string_name.begin(), string_name.end(), tchar_string);
    tchar_string[string_name.size()] = '\0';
    return tchar_string;
}
wchar_t* convert_string_to_wchar_t(const std::string& text_warning) {
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, text_warning.c_str(), -1, nullptr, 0);
    wchar_t* wideText = new wchar_t[bufferSize];
    MultiByteToWideChar(CP_UTF8, 0, text_warning.c_str(), -1, wideText, bufferSize);
    return wideText;
}
double convert_tchar_to_double(const TCHAR* numerical_string) {
    std::basic_string<TCHAR> converted_string(numerical_string);
    std::string numeral_string(converted_string.begin(), converted_string.end());
    double result;
    std::istringstream value(numeral_string);
    value >> result;
    return result;
}
std::string convert_tchar_to_string(const TCHAR* string_name) {
    std::basic_string<TCHAR> sentence(string_name);
    std::string str(sentence.begin(), sentence.end());
    return str;
}