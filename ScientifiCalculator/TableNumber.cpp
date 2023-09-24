#include "framework.h"

#include "BackgroundMode.h"
#include "TableNumber.h"
#include "ConvertData.h"
#include "DynamicNumbers.h"
#include "WindowButtons.h"
#include "WarningBox.h"

RECT optimiseSize;
bool limit_reached = false;

bool is_limit_reached() {
	return limit_reached;
}
void optimise_rect_position(RECT *clientRect, int number) {
	clientRect->right += number;
	clientRect->top -= number;
	clientRect->bottom += number;
	clientRect->left -= number;
}
std::string remove_zero_traces(double result) {
	std::string string_name = std::to_string(result);

	while (string_name.length()) {
		if (string_name[string_name.length() - 1] == '0') string_name.pop_back();
		else {
			if (string_name[string_name.length() - 1] == '.')
				string_name.pop_back();

			break;
		}
	}
	return string_name;
}
RECT read_table_dimensions(RECT screenSize) {
	optimiseSize.left = screenSize.right / 3;
	optimiseSize.right = screenSize.right - 11;
	optimiseSize.top = screenSize.bottom / 7;
	optimiseSize.bottom = int(screenSize.bottom / 1.9);

	return optimiseSize;
}
TCHAR* read_numbers_vector() {
	std::string buffer_text = "";
	for (int i = 0; i < buttons_numbers_values.size() - 1; i++) {
		buffer_text += buttons_numbers_values.at(i);
		if (buttons_numbers_values.at(i)[0] != '(' && buttons_numbers_values.at(i + 1)[0] != ')') 
			buffer_text += " ";
	}

	buffer_text += buttons_numbers_values.at(buttons_numbers_values.size() - 1);
	const std::string overflow_text = buffer_text;
	return convert_string_to_tchar(overflow_text);
}
void paint_result(HWND hWnd, HDC hdc) {
	double result = get_total_calculation(buttons_numbers_values);
	const std::string overflow_text = remove_zero_traces(result);

	TCHAR* number_decoy = convert_string_to_tchar(overflow_text);
	int text_length = _tcslen(number_decoy);

	SIZE textSize;
	GetTextExtentPoint(hdc, number_decoy, text_length, &textSize);

	int font_size = 125 - (text_length * 1.7);
	if (font_size < 20)
		font_size = 20;

	HFONT font_new = CreateFont(font_size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Trebuchet MS");
	HFONT font_old = static_cast<HFONT>(SelectObject(hdc, font_new));
	SetTextColor(hdc, colors_table[(int)background_info.is_user_dark][COLORING_OBJECT]);

	TextOut(hdc, optimiseSize.left + DIFFERENCIAL_TABLE_SPACE, optimiseSize.bottom - (optimiseSize.bottom / 3.5), number_decoy, text_length);

	SelectObject(hdc, font_old);
	DeleteObject(font_new);

	delete[] number_decoy;
}
void paint_numbers(HWND hWnd, HDC hdc) {
	TCHAR* number_decoy = read_numbers_vector();

	int text_length = _tcslen(number_decoy);

	SIZE textSize;
	GetTextExtentPoint(hdc, number_decoy, text_length, &textSize);
	if (optimiseSize.left + textSize.cx > optimiseSize.right - (DIFFERENCIAL_TABLE_SPACE * 2.2)) {
		delete[] number_decoy;
		if (!limit_reached) {
			send_warning_message("YOU REACHED THE LIMIT YOU CANNOT INSERT ANY NUMBER/SYMBOL!", true);
			limit_reached = true;
		}
		return;
	}

	if(limit_reached)
		send_warning_message("", false);

	limit_reached = false;
	int font_size = 125 - (text_length * 1.7);
	if (font_size < 20)
		font_size = 20;

    HFONT font_new = CreateFont(font_size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Trebuchet MS");
    HFONT font_old = static_cast<HFONT>(SelectObject(hdc, font_new));

    TextOut(hdc, optimiseSize.left + DIFFERENCIAL_TABLE_SPACE, optimiseSize.bottom / 3.5, number_decoy, text_length);

    SelectObject(hdc, font_old);
    DeleteObject(font_new);

	delete[] number_decoy;
}
void paint_table(HWND hWnd, HDC hdc) {
	HBRUSH brush_outline = CreateSolidBrush(colors_table[(int)background_info.is_user_dark][COLORING_OUTLINE]);
	HBRUSH brush_background = CreateSolidBrush(colors_table[(int)background_info.is_user_dark][COLORING_TABLE]);

	FillRect(hdc, &optimiseSize, brush_outline);
	optimise_rect_position(&optimiseSize, -3);
	FillRect(hdc, &optimiseSize, brush_background);
	optimise_rect_position(&optimiseSize, 3);

	DeleteObject(brush_outline);
	DeleteObject(brush_background);

	if (buttons_numbers_values.size()) {
		paint_numbers(hWnd, hdc);

		if (buttons_numbers_values.size() >= 2 && !get_num_opened_brackets() && !is_number_unfinished() && (is_string_number(buttons_numbers_values.at(buttons_numbers_values.size() - 1)) || buttons_numbers_values.at(buttons_numbers_values.size() - 1)[0] == ')'))
			paint_result(hWnd, hdc);
	}
}