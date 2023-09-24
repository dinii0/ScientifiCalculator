#include <vector>
#include <fstream>
using namespace std;

#include "resource.h"

#include "framework.h"
#include "BackgroundMode.h"
#include "CalculateButtons.h"
#include "ResourceReader.h"

#include <commctrl.h>

BUTTONS_CALCULATOR_CATEGORY buttons_calculator_list[BUTTONS_CALCULATE_TOTAL] = {
    { "Square Root", 1 }, { "Logarithmic with Base 10", 1 }, { "Exponential with Exponent 2", 1 },
    { "Module", 0 }, { "Variation (without repetition)", 2 }, { "Number: 1", 0 }, { "Number: 2", 0 },
    { "Number: 3", 0 }, { "Division", 0 }, { "Greatest Common Factor", -1 }, { "Sigma", 3 }, { "Pi", 3 },
    { "Cubic root", 1 }, { "Logarithmic with Base 5", 1 }, { "Exponential with Exponent 3", 1 },
    { "Binomial Coefficient", 2 }, { "Premutation", 2 }, { "Number: 4", 0 }, { "Number: 5", 0 },
    { "Number: 6", 0 }, { "Multiplying", 0 }, { "Factorial", 1 }, { "Binomial Power", 3 }, { "Sum of powers", 3 },
    { "Fourth root", 1 }, { "Logarithmic with Base 2", 1 }, { "Exponential with Exponent 4", 1 },
    { "Natural logarithmic", 1 }, { "Combinations", 2 }, { "Number: 7", 0 }, { "Number: 8", 0 }, { "Number: 9", 0 },
    { "Substraction", 0 }, { "Cosine", 1 }, { "Sine", 1 }, { "Tangent", 1 }, { "Desired root", 2 },
    { "Logarithmic with desired Base", 2 }, { "Exponential with desired Expontent", 2 }, { "Sign function", 1 },
    { "Matrix determinant", -1 }, { "Opening bracket", 0 }, { "Number: 0", 0 }, { "Closing bracket", 0 },
    { "Addition", 0 }, { "Dot", 0 }, { "Clear equation", 0 }, { "Delete the previous number", 0 }
};

HMODULE hInstance = 0;
int index_button, index_background_mode;

LRESULT CALLBACK buttons_calculator_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT rect;
            GetClientRect(hWnd, &rect);
            index_background_mode = (int)background_info.is_user_dark;
            index_button = GetDlgCtrlID(hWnd) - BUTTONS_CALCULATE_ID;

            HBRUSH hBrush_background = CreateSolidBrush(colors_table[index_background_mode][COLORING_OBJECT]);
            FillRect(hdc, &rect, hBrush_background);
            DeleteObject(hBrush_background);

            Gdiplus::Bitmap* bitmap_background;
            HICON button_icon;
            bitmap_background = load_image_from_resource(hInstance, IDB_BUTTON_PNG2 + index_button, L"PNG");
            bitmap_background->GetHICON(&button_icon);

            DrawIconEx(hdc, 0, 0, button_icon, rect.right, rect.bottom, 0, NULL, DI_NORMAL);

            delete bitmap_background;
            DestroyIcon(button_icon);

            if (buttons_calculator_list[index_button].variable_counter != 0) {
                HBRUSH hBrush_Blink = CreateSolidBrush(colors_table[index_background_mode][COLORING_BLINK]);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush_Blink);

                Ellipse(hdc, rect.right - 15, rect.bottom - 15, rect.right - 5, rect.bottom - 5);

                SelectObject(hdc, hOldBrush);
                DeleteObject(hBrush_Blink);
            }

            EndPaint(hWnd, &ps);
            break;
        }
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void create_calculate_buttons(HWND hWnd, HMODULE hInst, int width, int height) {
    int positionY = (height / 1.7), positionX = BUTTONS_CALCULATE_POSITIONX + BUTTONS_CALCULATE_PIXELS_SEPERATION;
	HWND hWnd_decoy;

	for (int i = 0; i < BUTTONS_CALCULATE_TOTAL; i++) {
		hWnd_decoy = CreateWindow(L"BUTTON", L"X", WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON | BS_OWNERDRAW, positionX, positionY, BUTTONS_CALCULATE_SIZE, BUTTONS_CALCULATE_SIZE, hWnd, (HMENU)(BUTTONS_CALCULATE_ID + i), hInst, NULL);
        SetWindowSubclass(hWnd_decoy, buttons_calculator_proc, 0, 0);

		positionX += BUTTONS_CALCULATE_SIZE + 5;

		if (positionX + BUTTONS_CALCULATE_SIZE >= width) {
			positionX = BUTTONS_CALCULATE_POSITIONX + BUTTONS_CALCULATE_PIXELS_SEPERATION;
			positionY += BUTTONS_CALCULATE_SIZE + 5;
		}
	}

    hInstance = hInst;
}