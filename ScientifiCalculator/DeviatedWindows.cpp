#include "framework.h"
#include "DeviatedWindows.h"
#include "WindowButtons.h"

#include <vector>

HWND input_count_hWnd;

HWND get_input_window_handler() {
	return input_count_hWnd;
}

void destroy_input_windows() {
	for(HWND buffer : text_fields_list) {
		DestroyWindow(buffer);
	}

	text_fields_list.clear();
}
void create_gcf_input_windows(int fields, HWND hWnd) {
	int positionX = 2, positionY = 60;
	HWND object;
	for (int i = 1; i <= fields; i++) {
		object = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_AUTOHSCROLL, positionX, positionY, 35, 20, hWnd, NULL, NULL, NULL);
		text_fields_list.push_back(object);
		positionX += 43;

		if (WINDOW_WIDTH - 20 <= positionX) {
			positionX = 2;
			positionY += 20;
		}
	}
}
void create_matrix_input_windows(int rows, HWND hWnd) {
	int positionX = 5, positionY = 83;
	HWND object;
	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= rows; j++) {
			object = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_AUTOHSCROLL, positionX, positionY, 40, 20, hWnd, NULL, NULL, NULL);
			text_fields_list.push_back(object);
			positionX += 40 + (11 - rows);
		}
		positionX = 5;
		positionY += 20 + rows;
	}
}
void paint_deviated_windows(std::string name_formula, HDC hdc) {
	if (name_formula == "Matrix determinant") {
		TCHAR description_text[] = _T("Inform us how many colomns and rows your matrix is: ");
		TextOut(hdc, 6, 26, description_text, static_cast<int>(_tcslen(description_text)));
	}
	else if(name_formula == "Greatest Common Factor") {
		TCHAR description_text[] = _T("Inform us how many numbers you want to add: ");
		TextOut(hdc, 6, 26, description_text, static_cast<int>(_tcslen(description_text)));
	}
}
void create_deviated_windows(std::string name_formula, HWND hWnd) {
	if (name_formula == "Matrix determinant") input_count_hWnd = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_AUTOHSCROLL, 370, 27, 20, 20, hWnd, (HMENU)MATRIX_COLOMNS_ID, NULL, NULL);
	else if (name_formula == "Greatest Common Factor") input_count_hWnd = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_AUTOHSCROLL, 330, 27, 20, 20, hWnd, (HMENU)GCF_ROWS_ID, NULL, NULL);
}