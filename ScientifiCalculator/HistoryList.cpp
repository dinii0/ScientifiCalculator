#include "framework.h"
#include "resource.h"
#include "BackgroundMode.h"
#include "HistoryList.h"
#include "TableNumber.h"
#include "ConvertData.h"
#include "ResourceReader.h"

#include <vector>
#include <CommCtrl.h>

HICON save_icon;
RECT history_size;
HWND hWnd_history;
HWND hWnd_scrollbar;
std::vector<std::string> history_equations;
int height_dimension_size;

LRESULT CALLBACK save_history_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	switch (uMsg) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			RECT rect;
			GetClientRect(hWnd, &rect);

			paint_gradientfill_button(hdc, rect);
			DrawIconEx(hdc, 0, 0, save_icon, rect.right, rect.bottom, 0, NULL, DI_NORMAL);
			EndPaint(hWnd, &ps);
			break;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

std::vector<std::string> get_history_list_definition() {
	SIZE size;
	std::string whole_equation = "", equation = "", answer = "";
	HDC hdc = CreateCompatibleDC(NULL);
	std::vector<std::string> vectorial_buffer;
	int equal_pos = 0;

	for (int i = history_equations.size() - 1; i >= 0; i--) {
		if (vectorial_buffer.size() * HISTORY_FONT_HEIGHT >= height_dimension_size) {
			break;
		}

		whole_equation = history_equations.at(i);
		equal_pos = whole_equation.find('=');
		equation = whole_equation.substr(0, equal_pos);
		answer = whole_equation.substr(equal_pos, whole_equation.length() - 1);
		equation.erase(equation.length() - 1, 1);
		whole_equation = equation + "\r\n" + answer;
		vectorial_buffer.push_back(whole_equation);
	}

	DeleteDC(hdc);
	return vectorial_buffer;
}

void update_histroy_list(std::string text) {
	if (history_equations.size() < 10) {
		history_equations.push_back(text);
	}
	else {
		for (int i = 0; i < history_equations.size() - 1; i++)
			history_equations.at(i) = history_equations.at(i + 1);

		history_equations.at(history_equations.size() - 1) = text;
	}

	std::string whole_list = "";
	std::vector<std::string> vectorial_buffer = get_history_list_definition();

	for (int i = vectorial_buffer.size() - 1; i >= 0; i--) {
		whole_list = vectorial_buffer.at(i) + "\r\n\r\n" + whole_list;
	}

	LPCWSTR buffer = convert_string_to_lpcwstr(whole_list);
	SetWindowText(hWnd_history, buffer);
}

void create_history_list(HWND hWnd, int width, int height) {
	hWnd_history = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_RIGHT | ES_READONLY | ES_MULTILINE, 8, 112, (width / 9) + (width / 5), height - (height / 8) - 3, hWnd, (HMENU)NULL, NULL, NULL);
	HWND hWnd_save = CreateWindow(L"BUTTON", L"", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW, width - 110, 45, 40, 40, hWnd, (HMENU)HISTORY_LIST_ID, NULL, NULL);
	SetWindowSubclass(hWnd_save, save_history_proc, 0, 0);
	
	if (!save_icon) {
		Gdiplus::Bitmap* bitmap_background;
		bitmap_background = load_image_from_resource(NULL, IDB_SAVE, L"PNG");
		bitmap_background->GetHICON(&save_icon);
	}
	// Another hardcoded shii:
	history_size.left = 6;
	history_size.top = 110;
	history_size.right = (width / 9) + (width / 5) + 10;
	history_size.bottom = height + 16;

	HFONT text_font = CreateFont(HISTORY_FONT_HEIGHT, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
	SendMessage(hWnd_history, WM_SETFONT, (WPARAM)text_font, TRUE);

	height_dimension_size = history_size.bottom - history_size.top;
}
void paint_history_outerline(HDC hdc) {
	HBRUSH brush_outline = CreateSolidBrush(colors_table[(int)background_info.is_user_dark][COLORING_OUTLINE]);
	FillRect(hdc, &history_size, brush_outline);
	DeleteObject(brush_outline);
}
HBRUSH colorize_history_list(WPARAM wParam) {
	int index_mode = (int)background_info.is_user_dark;
	HDC hdc = (HDC)wParam;

	SetBkColor(hdc, colors_table[index_mode][COLORING_TABLE]);
	SetTextColor(hdc, colors_table[index_mode][COLORING_TEXT]);
	return CreateSolidBrush(colors_table[index_mode][COLORING_TABLE]);
}