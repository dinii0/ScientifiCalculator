#ifndef CONVERTDATA_H
#define CONVERTDATA_H

#include <string>

LPCWSTR convert_string_to_lpcwstr(const std::string& index_name);
TCHAR* convert_string_to_tchar(const std::string& str);
wchar_t* convert_string_to_wchar_t(const std::string& text_warning);
double convert_tchar_to_double(const TCHAR* numerical_string);
std::string convert_tchar_to_string(const TCHAR* string_name);

#endif